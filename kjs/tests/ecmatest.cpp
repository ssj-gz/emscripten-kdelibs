/*
 *  This file is part of the KDE libraries
 *
 *  Copyright (C) 2012 Rolf Eike Beer <kde@opensource.sf-tec.de>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 */

#include "ecmatest.h"

#include <kio/global.h>
#include <wtf/HashTraits.h>
#include "JSLock.h"
#include "object.h"
#include "JSVariableObject.h"
#include "Parser.h"

#include <QtCore/QMap>
#include <QtCore/QDebug>

// from khtml/ecma/kjs_binding.cpp"
KJS::UString::UString(const QString &d)
{
  unsigned int len = d.length();
  KJS::UChar *dat = static_cast<KJS::UChar*>(fastMalloc(sizeof(KJS::UChar)*len));
  memcpy(dat, d.unicode(), len * sizeof(KJS::UChar));
  m_rep = KJS::UString::Rep::create(dat, len);
}

QString KJS::UString::qstring() const
{
  return QString((QChar*) data(), size());
}

// from khtml/ecma/debugger/value2string.cpp
QString valueToString(KJS::JSValue* value)
{
    switch(value->type())
    {
        case KJS::NumberType:
        {
            double v = 0.0;
            value->getNumber(v);
            return QString::number(v);
        }
        case KJS::BooleanType:
            return value->getBoolean() ? "true" : "false";
        case KJS::StringType:
        {
            KJS::UString s;
            value->getString(s);
            return '"' + s.qstring() + '"';
        }
        case KJS::UndefinedType:
            return "undefined";
        case KJS::NullType:
            return "null";
        case KJS::ObjectType:
            return "[object " + static_cast<KJS::JSObject*>(value)->className().qstring() +"]";
        case KJS::GetterSetterType:
        case KJS::UnspecifiedType:
        default:
            return QString();
    }
}

// from khtml/ecma/debugger/debugwindow.cpp
static QString exceptionToString(KJS::ExecState* exec, KJS::JSValue* exceptionObj)
{
    QString exceptionMsg = valueToString(exceptionObj);

    // Since we purposefully bypass toString, we need to figure out
    // string serialization ourselves.
    //### might be easier to export class info for ErrorInstance ---

    KJS::JSObject* valueObj = exceptionObj->getObject();
    KJS::JSValue*  protoObj = valueObj ? valueObj->prototype() : 0;

    bool exception   = false;
    bool syntaxError = false;
    if (protoObj == exec->lexicalInterpreter()->builtinSyntaxErrorPrototype())
    {
        exception   = true;
        syntaxError = true;
    }

    if (protoObj == exec->lexicalInterpreter()->builtinErrorPrototype()          ||
        protoObj == exec->lexicalInterpreter()->builtinEvalErrorPrototype()      ||
        protoObj == exec->lexicalInterpreter()->builtinReferenceErrorPrototype() ||
        protoObj == exec->lexicalInterpreter()->builtinRangeErrorPrototype()     ||
        protoObj == exec->lexicalInterpreter()->builtinTypeErrorPrototype()      ||
        protoObj == exec->lexicalInterpreter()->builtinURIErrorPrototype())
    {
        exception = true;
    }

    if (!exception)
        return exceptionMsg;

    // Clear exceptions temporarily so we can get/call a few things.
    // We memorize the old exception first, of course. Note that
    // This is not always the same as exceptionObj since we may be
    //  asked to translate a non-active exception
    KJS::JSValue* oldExceptionObj = exec->exception();
    exec->clearException();

    // We want to serialize the syntax errors ourselves, to provide the line number.
    // The URL is in "sourceURL" and the line is in "line"
    // ### TODO: Perhaps we want to use 'sourceId' in case of eval contexts.
    if (syntaxError)
    {
        KJS::JSValue* lineValue = valueObj->get(exec, "line");
        KJS::JSValue* urlValue  = valueObj->get(exec, "sourceURL");

        int      line = lineValue->toNumber(exec);
        QString  url  = urlValue->toString(exec).qstring();
        exceptionMsg = i18n("Parse error at %1 line %2",
                            url, line + 1);
    }
    else
    {
        // ### it's still not 100% safe to call toString here, even on
        // native exception objects, since someone might have changed the toString property
        // of the exception prototype, but I'll punt on this case for now.
        exceptionMsg = exceptionObj->toString(exec).qstring();
    }
    exec->setException(oldExceptionObj);
    return exceptionMsg;
}

class GlobalImp : public KJS::JSGlobalObject {
public:
  virtual KJS::UString className() const { return "global"; }
};

static GlobalImp* global;
static QString basedir( "" );
static QByteArray testrunner;
static QMap<QByteArray, QByteArray> includes;
static QStringList expectedBroken;	// list of tests we know that will fail

/**
 * load the given file from the harness directory
 * @param fn filename
 * @return if operation succeeded
 *
 * Will load the given file into the "includes" map
 */
static bool loadInclude( const QByteArray &fn )
{
    QFile runnerfile( basedir + QLatin1String( "test/harness/" ) + QString::fromAscii( fn ) );

    if ( !runnerfile.open( QIODevice::ReadOnly ) )
        return false;

    includes[ fn ] = runnerfile.readAll();

    return true;
}

QTEST_KDEMAIN(ECMAscriptTest, NoGUI)

void ECMAscriptTest::initTestCase()
{
    basedir = QString::fromUtf8( qgetenv( "ECMATEST_BASEDIR" ) );

    if ( basedir.isEmpty() )
        qFatal( "ECMATEST_BASEDIR not set" );

    if ( !basedir.endsWith( QLatin1Char( '/' ) ) )
        basedir += QLatin1Char( '/' );

    QVERIFY( loadInclude( "sta.js" ) );
    QVERIFY( loadInclude( "ed.js" ) );

    testrunner = includes[ "sta.js" ] + includes[ "ed.js" ] + '\n';

    const QString brokenFn = QString::fromAscii( qgetenv( "ECMATEST_BROKEN" ) );
    if ( !brokenFn.isEmpty() ) {
        QFile brokenF( brokenFn );
        if ( !brokenF.open( QIODevice::ReadOnly ) ) {
            const QString errmsg = QString::fromAscii( "cannot open " ) + brokenFn;
            QWARN( errmsg.toAscii() );
        } else {
            expectedBroken = QString::fromAscii( brokenF.readAll() ).split( QLatin1Char( '\n' ) )
                                               .filter( QRegExp( "^[^#].*" ) );
        }
    }

    m_passed = 0;
}

static QByteArray getTextProperty( const QByteArray &property, const QByteArray &code )
{
    int from = code.indexOf( property );
    if ( from == -1 )
        return QByteArray();

    from += property.length();
    while ( code[ from ] == ' ' )
        from++;

    int to = code.indexOf( '\n', from );
    if (code[to - 1] == '\r')
        to--;
    // poor mans escaping
    return code.mid( from, to - from ).replace( "\\", "\\\\" ).replace( "\"", "\\\"" );
}

#define ECMATEST_VERIFY( expr ) \
    do { \
        const bool tmp_result = ( expr ); \
        if ( tmp_result ) \
            m_passed++; \
        else \
            m_failed++; \
        if ( knownBroken ) \
            QEXPECT_FAIL(QTest::currentDataTag(), "It is known that KJS doesn't pass this test", Abort); \
        QVERIFY( tmp_result ); \
    } while (0)

static QMap< QByteArray, QByteArray > skips;

void ECMAscriptTest::runAllTests()
{
    static const QByteArray include = "$INCLUDE(\"";

    QFETCH(QString, filename);
    QByteArray expectedError;

    QFile input( filename );

    foreach ( const QByteArray &skip, skips.keys() ) {
        if ( skip == QTest::currentDataTag() )
            QSKIP( skips[ skip ], SkipSingle );
    }

    QVERIFY( input.open( QIODevice::ReadOnly ) );

    const QByteArray testdata = input.readAll();

    QVERIFY( ! testdata.isEmpty() );

    RefPtr<KJS::Interpreter> interp = new KJS::Interpreter(global);

    KJS::Interpreter::setShouldPrintExceptions(true);

    QByteArray testscript;

    // test is expected to fail
    if ( testdata.indexOf( "@negative" ) >= 0 ) {
        expectedError = getTextProperty( "@negative", testdata );
        if ( expectedError.isEmpty() )
            expectedError = ".";
    }

    int from = 0;
    while ( ( from = testdata.indexOf( include, from ) ) >= 0 ) {
        int endq = testdata.indexOf( "\"", from + include.length() );
        QVERIFY( endq >= 0 );

        const QByteArray includeFile = testdata.mid( from + include.length(), endq - from - include.length() );

        if ( ! includes.contains( includeFile ) )
            QVERIFY( loadInclude( includeFile ) );

        testscript += includes[ includeFile ];
        from = endq;
    }

    testscript += testrunner;

    testscript += testdata;

    const QFileInfo info( input );

    const QString scriptutf = QString::fromUtf8( testscript.constData() );

    KJS::Completion completion = interp->evaluate(info.fileName().toAscii().constData(), 0, scriptutf);

    const bool knownBroken = expectedBroken.contains( QString::fromAscii( QTest::currentDataTag() ) );

    if ( expectedError.isEmpty() ) {
        ECMATEST_VERIFY( completion.complType() != KJS::Throw );
    } else {
        if ( knownBroken && completion.complType() != KJS::Throw ) {
            QEXPECT_FAIL(QTest::currentDataTag(), "It is known that KJS doesn't pass this test", Abort);
            m_failed++;
        }

        QCOMPARE( completion.complType(), KJS::Throw );
        QVERIFY( completion.value() != NULL );

        const QString eMsg = exceptionToString( interp->execState(), completion.value() );

        if ( expectedError == "^((?!NotEarlyError).)*$" ) {
            ECMATEST_VERIFY( eMsg.indexOf( "NotEarlyError" ) == -1 );
        } else if ( expectedError == "." ) {
            // means "every exception passes
        } else {
            ECMATEST_VERIFY( eMsg.indexOf( expectedError ) >= 0 );
        }
    }
}

void ECMAscriptTest::runAllTests_data()
{
    global = new GlobalImp();

    QTest::addColumn<QString>( "filename" );

    const QStringList js( QLatin1String( "*.js" ) );
    const QStringList all( QLatin1String( "*" ) );
    const QString chapter = QString::fromAscii( qgetenv( "ECMATEST_CHAPTER" ) );

    if ( !chapter.isEmpty() )
        QWARN( "===> Testing chapter " + chapter.toAscii() );

    // some tests fail when the suite is run as a whole
    if ( chapter.isEmpty() || chapter.startsWith("ch15") ) {
        const QByteArray endlessLoop = "this test causes an endless loop, avoid it for the moment";
        skips[ "S15.1.2.3_A6" ] = endlessLoop;
        skips[ "S15.1.3.1_A2.5_T1" ] = endlessLoop;
        skips[ "S15.1.3.2_A2.4_T1" ] = endlessLoop;
        skips[ "S15.1.3.2_A2.5_T1" ] = endlessLoop;
        skips[ "15.2.3.4-4-1" ] = "this test causes a crash when run as part of the whole suite";
    }

    QDirIterator it( basedir + QLatin1String("test/suite/") + chapter, QDirIterator::Subdirectories);
    while ( it.hasNext() ) {
        it.next();

        const QFileInfo info = it.fileInfo();

        if ( !info.isFile() )
            continue;

        QString filename = info.fileName();

        filename.chop(3); // .js

        QTest::newRow( filename.toAscii() ) << info.absoluteFilePath();
    }
}

void ECMAscriptTest::cleanup()
{
    global->clearProperties();
}

void ECMAscriptTest::cleanupTestCase()
{
    qDebug() << "passed testcases:" << m_passed << "failed testcases:" << m_failed;
}

#include "ecmatest.moc"

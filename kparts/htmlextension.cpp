/* This file is part of the KDE project
   Copyright (C) 2010 David Faure <faure@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
 */

#include "htmlextension.h"
#include <kglobal.h>
#include "part.h"

using namespace KParts;

KParts::HtmlExtension::HtmlExtension(KParts::ReadOnlyPart* parent)
    : QObject(parent), d(0)
{
}

KParts::HtmlExtension::~HtmlExtension()
{
}

HtmlExtension * KParts::HtmlExtension::childObject( QObject *obj )
{
    return KGlobal::findDirectChild<KParts::HtmlExtension *>(obj);
}

class SelectorInterface::ElementPrivate : public QSharedData
{
public:
    QString tag;
    QHash<QString, QString> attributes;
};

SelectorInterface::Element::Element()
    : d(new ElementPrivate)
{
}

SelectorInterface::Element::Element(const SelectorInterface::Element& other)
    : d(other.d)
{
}

SelectorInterface::Element::~Element()
{
}

void SelectorInterface::Element::setTagName(const QString& tag)
{
    d->tag = tag;
}

QString SelectorInterface::Element::tagName() const
{
    return d->tag;
}

void SelectorInterface::Element::setAttribute(const QString& name, const QString& value)
{
    d->attributes[name] = value; // insert or replace
}

QStringList SelectorInterface::Element::attributeNames() const
{
    return d->attributes.keys();
}

QString SelectorInterface::Element::attribute(const QString& name, const QString& defaultValue) const
{
    return d->attributes.value(name, defaultValue);
}
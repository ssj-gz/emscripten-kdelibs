/* This file is part of the KDE libraries
   Copyright (C) 2007 Bernhard Loos <nhuh.put@web.de>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "kstandarddirs.h"

#include <config-prefix.h>
#include <config.h>
#include <QFile>
#include <QDir>

QString KStandardDirs::installPath(const char *type)
{
    Q_ASSERT(type != NULL);
#ifdef EMSCRIPTEN_NATIVE
    QString PSEUDO_INSTALL_ROOT = QDir::currentPath() + QString::fromAscii("/");  
#else
    QString PSEUDO_INSTALL_ROOT = QString::fromAscii("/");  
#endif

    switch (type[0]) {
        case 'a':
            if (strcmp("apps", type) == 0)
                return QFile::decodeName(QString(QString((PSEUDO_INSTALL_ROOT + QString::fromAscii("share/applnk/")))).toAscii());
            break;
        case 'c':
            if (strcmp("config", type) == 0)
                return QFile::decodeName(QString((PSEUDO_INSTALL_ROOT + QString::fromAscii("share/config/"))).toAscii());
            break;
        case 'k':
            if (strcmp("kcfg", type) == 0)
                return QFile::decodeName(QString((PSEUDO_INSTALL_ROOT + QString::fromAscii("share/config.kcfg/"))).toAscii());
            if (strcmp("kdedir", type) == 0)
                return QFile::decodeName(QString((PSEUDO_INSTALL_ROOT + QString::fromAscii("/"))).toAscii());
            break;
        case 'd':
            if (strcmp("data", type) == 0)
                return QFile::decodeName(QString((PSEUDO_INSTALL_ROOT + QString::fromAscii("share/apps/"))).toAscii());
            break;
        case 'e':
            if (strcmp("exe", type) == 0)
                return QFile::decodeName(QString((PSEUDO_INSTALL_ROOT + QString::fromAscii("bin/"))).toAscii());
            break;
        case 'h':
            if (strcmp("html", type) == 0)
                return QFile::decodeName(QString((PSEUDO_INSTALL_ROOT + QString::fromAscii("share/doc/html/"))).toAscii());
            break;
        case 'i':
            if (strcmp("icon", type) == 0)
                return QFile::decodeName(QString((PSEUDO_INSTALL_ROOT + QString::fromAscii("share/icons/"))).toAscii());
            if (strcmp("include", type) == 0)
                return QFile::decodeName(QString((PSEUDO_INSTALL_ROOT + QString::fromAscii("include/"))).toAscii());
            break;
        case 'l':
            if (strcmp("lib", type) == 0)
                return QFile::decodeName(QString((PSEUDO_INSTALL_ROOT + QString::fromAscii("lib/"))).toAscii());
            if (strcmp("libexec", type) == 0)
                return QFile::decodeName(QString((PSEUDO_INSTALL_ROOT + QString::fromAscii("/lib/kde4/libexec/"))).toAscii());
            if (strcmp("locale", type) == 0)
                return QFile::decodeName(QString((PSEUDO_INSTALL_ROOT + QString::fromAscii("share/locale/"))).toAscii());
            break;
        case 'm':
            if (strcmp("module", type) == 0)
                return QFile::decodeName(QString((PSEUDO_INSTALL_ROOT + QString::fromAscii("lib/kde4/"))).toAscii());
            if (strcmp("mime", type) == 0)
                return QFile::decodeName(QString((PSEUDO_INSTALL_ROOT + QString::fromAscii("share/mimelnk/"))).toAscii());
            break;
        case 'q':
            if (strcmp("qtplugins", type) == 0)
                return QFile::decodeName(QString((PSEUDO_INSTALL_ROOT + QString::fromAscii("lib/kde4/plugins/"))).toAscii());
            break;
        case 's':
            if (strcmp("services", type) == 0)
                return QFile::decodeName(QString((PSEUDO_INSTALL_ROOT + QString::fromAscii("share/kde4/services/"))).toAscii());
            if (strcmp("servicetypes", type) == 0)
                return QFile::decodeName(QString((PSEUDO_INSTALL_ROOT + QString::fromAscii("share/kde4/servicetypes/"))).toAscii());
            if (strcmp("sound", type) == 0)
                return QFile::decodeName(QString((PSEUDO_INSTALL_ROOT + QString::fromAscii("share/sounds/"))).toAscii());
            break;
        case 't':
            if (strcmp("templates", type) == 0)
                return QFile::decodeName(QString((PSEUDO_INSTALL_ROOT + QString::fromAscii("share/templates/"))).toAscii());
            break;
        case 'w':
            if (strcmp("wallpaper", type) == 0)
                return QFile::decodeName(QString((PSEUDO_INSTALL_ROOT + QString::fromAscii("share/wallpapers/"))).toAscii());
            break;
        case 'x':
            if (strcmp("xdgconf-menu", type) == 0)
                return QFile::decodeName(QString((PSEUDO_INSTALL_ROOT + QString::fromAscii("etc/xdg/menus/"))).toAscii());
            if (strcmp("xdgdata-apps", type) == 0)
                return QFile::decodeName(QString((PSEUDO_INSTALL_ROOT + QString::fromAscii("share/applications/kde4/"))).toAscii());
            if (strcmp("xdgdata-dirs", type) == 0)
                return QFile::decodeName(QString((PSEUDO_INSTALL_ROOT + QString::fromAscii("share/desktop-directories/"))).toAscii());
            break;
    }
    return QString();
}

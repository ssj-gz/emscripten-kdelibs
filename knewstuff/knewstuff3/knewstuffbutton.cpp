/*
    knewstuff3/ui/knewstuffbutton.cpp.
    Copyright (c) 2004 Aaron J. Seigo <aseigo@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "knewstuffbutton.h"

#include <kiconloader.h>
#include <klocale.h>
#include <kicon.h>

#include "knewstuff3/client.h"

namespace KNS3
{

Button::Button(const QString& what,
               const QString& providerList,
               const QString& resourceType,
               QWidget* parent)
        : KPushButton(parent),
        d(0),
        m_providerList(providerList),
        m_type(resourceType),
        m_engine(0)
{
    setButtonText(what);
    init();
}

Button::Button(QWidget* parent)
        : KPushButton(parent),
        d(0),
        m_engine(0)
{
    setButtonText(i18n("Download New Stuff"));
    init();
}

void Button::init()
{
    setIcon(KIcon("get-hot-new-stuff"));
    connect(this, SIGNAL(clicked()), SLOT(showDialog()));
}

void Button::setButtonText(const QString& what)
{
    setText(i18n("Download New %1", what));
}

void Button::setProviderList(const QString& providerList)
{
    m_providerList = providerList;
}

void Button::setResourceType(const QString& resourceType)
{
    m_type = resourceType;
}

void Button::showDialog()
{
    emit aboutToShowDialog();

    // FIXME

    emit dialogFinished();
}

}

#include "knewstuffbutton.moc"
/*
    Copyright 2010 Rafael Fernández López <ereslibre@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#include "udevmanager.h"

#include "udev.h"
#include "udevdevice.h"
#include "../shared/rootdevice.h"

#include <QtCore/QSet>
#include <QtCore/QDebug>

using namespace Solid::Backends::UDev;
using namespace Solid::Backends::Shared;

class UDevManager::Private
{
public:
    Private();
    ~Private();

    UdevQt::Client *const m_client;
    QSet<Solid::DeviceInterface::Type> m_supportedInterfaces;
};

UDevManager::Private::Private()
    : m_client(new UdevQt::Client)
{
}

UDevManager::Private::~Private()
{
    delete m_client;
}

UDevManager::UDevManager(QObject *parent)
    : d(new Private)
{
    d->m_supportedInterfaces << Solid::DeviceInterface::GenericInterface
                             << Solid::DeviceInterface::Processor
                             << Solid::DeviceInterface::Camera
                             << Solid::DeviceInterface::PortableMediaPlayer
                             << Solid::DeviceInterface::Button;
}

UDevManager::~UDevManager()
{
    delete d;
}

QString UDevManager::udiPrefix() const
{
    return QString::fromLatin1(UDEV_UDI_PREFIX);
}

QSet<Solid::DeviceInterface::Type> UDevManager::supportedInterfaces() const
{
    return d->m_supportedInterfaces;
}

QStringList UDevManager::allDevices()
{
    QStringList res;
    const UdevQt::DeviceList deviceList = d->m_client->allDevices();
    foreach (const UdevQt::Device &device, deviceList) {
        res << udiPrefix() + device.sysfsPath();
    }
    return res;
}

QStringList UDevManager::devicesFromQuery(const QString &parentUdi,
                                          Solid::DeviceInterface::Type type)
{
    // TODO: actually use the query parameters
    return allDevices();
}

QObject *UDevManager::createDevice(const QString &udi_)
{
    if (udi_ == udiPrefix()) {
        RootDevice *const device = new RootDevice(UDEV_UDI_PREFIX);
        device->setProduct(tr("Devices"));
        device->setDescription(tr("Devices declared in your system"));
        device->setIcon("folder-remote"); // TODO: CHANGE ICON
        return device;
    }
    const QString udi = udi_.right(udi_.size() - udiPrefix().size());
    return new UDevDevice(d->m_client->deviceBySysfsPath(udi));
}
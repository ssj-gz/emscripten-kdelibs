/*
   This file is part of the KDE project

   Copyright 2010 Paulo Romulo Alves Barros <paulo.romulo@kdemail.net>

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

#ifndef SOLID_BACKENDS_UPNP_STORAGEACCESS_H
#define SOLID_BACKENDS_UPNP_STORAGEACCESS_H

#include <solid/ifaces/storageaccess.h>
#include <solid/backends/upnp/upnpdeviceinterface.h>

namespace Solid
{
namespace Backends
{
namespace UPnP
{
  
  class UPnPStorageAccess : public Solid::Backends::UPnP::UPnPDeviceInterface, virtual public Solid::Ifaces::StorageAccess
  {
    Q_OBJECT
    Q_INTERFACES(Solid::Ifaces::StorageAccess)
    
    public:
      explicit UPnPStorageAccess(UPnPDevice* device);
      
      virtual ~UPnPStorageAccess();
      
      virtual bool isAccessible() const;
      
      virtual QString filePath() const;
      
      virtual bool setup();
      
      virtual bool teardown();
      
    Q_SIGNALS:
      void accessibilityChanged(bool accessible, const QString &udi);
      
      void setupDone(Solid::ErrorType error, QVariant data, const QString &udi);
      
      void teardownDone(Solid::ErrorType error, QVariant data, const QString &udi);   
  };

}
}
}

#endif // SOLID_BACKENDS_UPNP_STORAGEACCESS_H
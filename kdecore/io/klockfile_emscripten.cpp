/*
   This file is part of the KDE libraries
   Copyright (c) 2007 Ralf Habacker <ralf.habacker@freenet.de>

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

#include "klockfile.h"
#include "kcomponentdata.h"
#include "kdebug.h"


class KLockFile::Private
{
public:
    Private(const KComponentData &c)
        : componentData(c)
    {
    }

    static int debugArea() { static int s_area = KDebug::registerArea("kdecore (KLockFile)"); return s_area; }

    QString file;
    bool isLocked;
    int staleTime;
    KComponentData componentData;
};

KLockFile::KLockFile(const QString &file, const KComponentData &componentData)
    : d(new Private(componentData))
{
    d->file = file;
    d->isLocked = false;
    d->staleTime = 0;
}

KLockFile::~KLockFile()
{
    unlock();
    delete d;
}

int
KLockFile::staleTime() const
{
  return d->staleTime;
}


void
KLockFile::setStaleTime(int _staleTime)
{
  d->staleTime = _staleTime;
}

KLockFile::LockResult 
KLockFile::lock(LockFlags options)
{
    d->isLocked = true;
    return LockOK;
}

bool 
KLockFile::isLocked() const
{
    return d->isLocked;
}

void 
KLockFile::unlock()
{
    if (d->isLocked)
    {
         d->isLocked = false;
    }
}

bool 
KLockFile::getLockInfo(int &pid, QString &hostname, QString &appname)
{
  return false;
}

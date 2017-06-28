/*
 * Copyright 2000-2015 Rochus Keller <mailto:rkeller@nmr.ch>
 *
 * This file is part of the CARA (Computer Aided Resonance Assignment,
 * see <http://cara.nmr.ch/>) NMR Application Framework (NAF) library.
 *
 * The following is the license that applies to this copy of the
 * library. For a license to use the library under conditions
 * other than those described here, please email to rkeller@nmr.ch.
 *
 * GNU General Public License Usage
 * This file may be used under the terms of the GNU General Public
 * License (GPL) versions 2.0 or 3.0 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in
 * the packaging of this file. Please review the following information
 * to ensure GNU General Public Licensing requirements will be met:
 * http://www.fsf.org/licensing/licenses/info/GPLv2.html and
 * http://www.gnu.org/copyleft/gpl.html.
 */

#include "PtrBinding.h"
#include <QHash>
#include <QReadWriteLock>
using namespace Lua;

typedef QMultiHash<void*, void**> GuardHash;
Q_GLOBAL_STATIC(GuardHash, guardHash)
Q_GLOBAL_STATIC(QReadWriteLock, guardHashLock)

void PtrBindingBase::addGuard(void **ptr)
{
    if (!*ptr)
        return;
    GuardHash *hash = guardHash();
    if (!hash) {
        *ptr = 0;
        return;
    }
    QWriteLocker locker(guardHashLock());
    hash->insert(*ptr, ptr);
}

void PtrBindingBase::removeGuard(void **ptr)
{
    if (!*ptr)
        return;
    GuardHash *hash = guardHash();
    if (!hash)
        return;
    QWriteLocker locker(guardHashLock());
    GuardHash::iterator it = hash->find(*ptr);
    const GuardHash::iterator end = hash->end();
    for (; it.key() == *ptr && it != end; ++it) {
        if (it.value() == ptr) {
            (void) hash->erase(it);
            break;
        }
    }
}

void PtrBindingBase::changeGuard(void **ptr, void *o)
{
    GuardHash *hash = guardHash();
    if (!hash) {
        *ptr = 0;
        return;
    }
    QWriteLocker locker(guardHashLock());
    if (*ptr) {
        GuardHash::iterator it = hash->find(*ptr);
        const GuardHash::iterator end = hash->end();
        for (; it.key() == *ptr && it != end; ++it) {
            if (it.value() == ptr) {
                (void) hash->erase(it);
                break;
            }
        }
    }
    *ptr = o;
    if (*ptr)
        hash->insert(*ptr, ptr);
}

void PtrBindingBase::notifyGuard(void* o)
{
    // set all QPointers for this object to zero
    GuardHash *hash = ::guardHash();
    if( hash )
    {
        QWriteLocker locker( guardHashLock() );
        GuardHash::iterator it = hash->find(o);
        const GuardHash::iterator end = hash->end();
        while( it.key() == o && it != end )
        {
            *it.value() = 0;
            it = hash->erase(it);
        }
    }
}


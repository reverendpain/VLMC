/*****************************************************************************
 * GUIProjectManager.h: Handle the GUI part of the project managing
 *****************************************************************************
 * Copyright (C) 2008-2014 VideoLAN
 *
 * Authors: Hugo Beauzée-Luyssen <hugo@beauzee.fr>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifndef GUIPROJECTMANAGER_H
#define GUIPROJECTMANAGER_H

#include "Project/ProjectCallbacks.h"

class GUIProjectManager : public IProjectUiCb
{
public:
    virtual bool        shouldLoadBackupFile();
    virtual bool        shouldDeleteOutdatedBackupFile();
    virtual QString     getProjectFileDestination( const QString &defaultPath );
    virtual SaveMode    shouldSaveBeforeClose();
};

#endif // GUIPROJECTMANAGER_H

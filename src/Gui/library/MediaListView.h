/*****************************************************************************
 * MediaListView.h:
 *****************************************************************************
 * Copyright (C) 2008-2014 VideoLAN
 *
 * Authors: Thomas Boquet <thomas.boquet@gmail.com>
 *          Hugo Beauzée-Luyssen <beauze.h@vlmc.org>
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

#ifndef MEDIALISTVIEW_H
#define MEDIALISTVIEW_H

#include "ListViewController.h"

#include <QHash>
#include <QUuid>

class   Clip;
class   Media;
class   MediaContainer;
class   MediaCellView;
class   Project;
class   StackViewController;

class   QWidget;

class MediaListView : public ListViewController
{
    Q_OBJECT

public:
    typedef     QHash<QUuid, MediaCellView*>    MediaList;

    MediaListView( StackViewController *nav );
    virtual ~MediaListView();

    const MediaList                 &mediaList() const;
    void                            setMediaContainer( MediaContainer* container );

private:
    StackViewController             *m_nav;
    QUuid                           m_currentUuid;
    MediaList                       m_cells;
    QUuid                           m_lastUuidClipListAsked;
    MediaContainer                  *m_mediaContainer;

public slots:
    void        cellSelection( const QUuid &uuid );
    void        showSubClips( const QUuid &uuid );
    void        clear();

private slots:
    void        removeClip( const Clip* );
    /**
     *  \brief  Called when the media container unload a clip
     *
     *  This slot will be triggered when a Clip is removed from the media container.
     */
    void        __clipRemoved( const QUuid& );
    void        newClipLoaded( Clip *clip );

    void        projectLoading( Project* project );

signals:
    void        clipSelected( Clip* );
    /// Used when the user clicks the deletion arrow.
    void        clipRemoved( const QUuid& );
};
#endif // MEDIALISTVIEW_H

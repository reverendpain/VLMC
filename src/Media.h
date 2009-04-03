/*****************************************************************************
 * Media.h: Generic class for media handling
 *****************************************************************************
 * Copyright (C) 2008-2009 the VLMC team
 *
 * Authors: Hugo Beauzee-Luyssen <hugo@vlmc.org>
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

#ifndef MEDIA_H
#define MEDIA_H

#include <QList>
#include <QString>

#include "VLCMedia.h"
#include "VLCInstance.h"
#include "VLCMediaPlayer.h"

/**
  * Generic class for media handling.
  */
class       Media
{
public:
    virtual ~Media();

    void                loadMedia( const QString& mrl );
    virtual void        play();
    void                addParam( const QString& param );
    void                setupMedia();

protected:
    //Protected constructor so we can't use a Media without its sub-implementation
    Media( LibVLCpp::Instance* instance, const QString& mrl );

    LibVLCpp::Instance*         m_instance;
    LibVLCpp::Media*            m_vlcMedia;
    LibVLCpp::MediaPlayer*      m_vlcMediaPlayer;
    QString                     m_mrl;
    QList<QString>              m_parameters;
};

#endif // MEDIA_H

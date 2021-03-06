/*****************************************************************************
 * About.cpp: About dialog
 *****************************************************************************
 * Copyright (C) 2008-2014 VideoLAN
 *
 * Authors: Christophe Courtaut <christophe.courtaut@gmail.com>
 *          Rohit Yadav <rohityadav89@gmail.com>
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

#include "About.h"
#include "config.h"

#include <QApplication>
#include <QFile>
#include <QPlainTextEdit>
#include <QString>
#include <QtGlobal>

About::About( QWidget *parent ) : QDialog( parent )
{
    m_ui.setupUi( this );
    m_ui.tabWidget->setCurrentIndex( 0 );

    setText( ":/text/AUTHORS", m_ui.plainTextEditAuthors );
    setText( ":/text/THANKS", m_ui.plainTextEditThanks );
    setText( ":/text/TRANSLATORS", m_ui.plainTextEditTranslators );
    setText( ":/text/COPYING", m_ui.plainTextEditLicense );

    m_ui.labelTitle->setText(
        m_ui.labelTitle->text().arg( PROJECT_VERSION, CODENAME ) );

    m_ui.labelBuild->setText(
        m_ui.labelBuild->text().arg( HOSTNAME, SYSNAME,
                                     QT_VERSION_STR, qVersion() ) );

    m_ui.labelCopyright->setText(
        m_ui.labelCopyright->text().arg( PROJECT_COPYRIGHT, PROJECT_CONTACT,
                                         ORG_WEBSITE ) );

    connect( qApp, SIGNAL( aboutToQuit() ), this, SLOT( deleteLater() ) );
}

void
About::changeEvent( QEvent *e )
{
    QDialog::changeEvent( e );
    switch ( e->type() )
    {
        case QEvent::LanguageChange:
            m_ui.retranslateUi( this );
            m_ui.labelTitle->setText(
                m_ui.labelTitle->text().arg( PROJECT_VERSION, CODENAME ) );
            m_ui.labelBuild->setText(
                m_ui.labelBuild->text().arg( HOSTNAME, SYSNAME,
                                             QT_VERSION_STR, qVersion() ) );
            m_ui.labelCopyright->setText(
                m_ui.labelCopyright->text().arg( PROJECT_COPYRIGHT,
                                                 PROJECT_CONTACT, ORG_WEBSITE ) );
            break;
        default:
            break;
    }
}

void
About::setText( const QString& filename, QPlainTextEdit* widget )
{
    QFile file( filename );

    if ( file.open( QIODevice::ReadOnly | QIODevice::Text ) )
        widget->insertPlainText( QString::fromUtf8( file.readAll() ) );

    widget->moveCursor( QTextCursor::Start );
}

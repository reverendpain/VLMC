/*****************************************************************************
 * ImportController.cpp
 *****************************************************************************
 * Copyright (C) 2008-2010 VideoLAN
 *
 * Authors: Geoffroy Lacarriere <geoffroylaca@gmail.com>
 *          Thomas Boquet <thomas.boquet@gmail.com>
 *          Hugo Beauzée-Luyssen <hugo@vlmc.org>
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

#include "ImportController.h"
#include "ui_ImportController.h"

#include "Clip.h"
#include "ClipMetadataDisplayer.h"
#include "ClipRenderer.h"
#include "Library.h"
#include "Media.h"
#include "MediaCellView.h"
#include "MediaListView.h"
#include "MetaDataManager.h"
#include "PreviewWidget.h"
#include "TagWidget.h"

#include <QFileSystemModel>
#include <QPalette>
#include <QSettings>
#include <QTime>
#include <QTimer>

#include <QtDebug>

ImportController::ImportController(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::ImportController),
    m_nbMediaToLoad( 0 ),
    m_nbMediaLoaded( 0 )
{
    m_ui->setupUi(this);
    //The renderer will be deleted by the PreviewWidget
    m_clipRenderer = new ClipRenderer;
    m_ui->previewContainer->setRenderer( m_clipRenderer );
    m_stackNav = new StackViewController( m_ui->stackViewContainer );
    m_temporaryMedias = new MediaContainer;
    m_mediaListView = new MediaListView( m_stackNav, m_temporaryMedias );
//    m_tag = new TagWidget( m_ui->tagContainer, 6 );
    m_filesModel = new QFileSystemModel( this );
    m_stackNav->pushViewController( m_mediaListView );

    m_nameFilters << Media::AudioExtensions.split(' ', QString::SkipEmptyParts)
            << Media::VideoExtensions.split(' ', QString::SkipEmptyParts)
            << Media::ImageExtensions.split(' ', QString::SkipEmptyParts);
    m_filesModel->setFilter( QDir::AllDirs | QDir::AllEntries | QDir::NoDotAndDotDot );
    m_filesModel->setNameFilters( m_nameFilters );
    m_filesModel->setRootPath( "/" );
    m_filesModel->setNameFilterDisables( false );
    restoreCurrentPath();
    m_filesModel->sort( 0 );

    m_ui->treeView->setModel( m_filesModel );
    m_ui->treeView->setHeaderHidden( true );
    m_ui->treeView->setCurrentIndex( m_filesModel->index( m_currentlyWatchedDir ) );
    m_ui->treeView->setExpanded( m_ui->treeView->currentIndex() , true );
    m_ui->treeView->header()->setStretchLastSection( false );
    m_ui->treeView->header()->setResizeMode( QHeaderView::ResizeToContents );
    m_ui->treeView->setColumnHidden( 1, true );
    m_ui->treeView->setColumnHidden( 2, true );
    m_ui->treeView->setColumnHidden( 3, true );

    m_ui->forwardButton->setEnabled( true );

    m_ui->progressBar->setHidden( true );
    m_ui->errorLabelImg->hide();
    m_ui->errorLabel->hide();

    connect( m_ui->treeView, SIGNAL( clicked( QModelIndex ) ),
             this, SLOT( treeViewClicked( QModelIndex ) ) );
    connect( m_ui->treeView, SIGNAL( doubleClicked( QModelIndex ) ),
             this, SLOT( treeViewDoubleClicked( QModelIndex ) ) );
    connect( m_ui->forwardButton, SIGNAL( clicked() ),
             this, SLOT( forwardButtonClicked() ) );

    connect( this, SIGNAL( clipSelected( Clip* ) ),
             m_clipRenderer, SLOT( setClip( Clip* ) ) );
    connect( m_mediaListView, SIGNAL( clipSelected( Clip* ) ),
             this, SLOT( clipSelection( Clip* ) ) );
    connect( m_mediaListView, SIGNAL( clipRemoved( const QUuid& ) ),
             m_clipRenderer, SLOT( clipUnloaded( const QUuid& ) ) );

    connect( MetaDataManager::getInstance(), SIGNAL( failedToCompute( Media* ) ),
             this, SLOT( failedToLoad( Media* ) ) );
}

ImportController::~ImportController()
{
    delete m_ui;
    delete m_stackNav;
//    delete m_tag;
}

void
ImportController::changeEvent( QEvent *e )
{
    QDialog::changeEvent( e );
    switch ( e->type() )
    {
        case QEvent::LanguageChange:
            m_ui->retranslateUi( this );
            break;
        default:
            break;
    }
}

void
ImportController::clipSelection( Clip* clip )
{
    const QUuid& uuid = clip->uuid();
    if ( m_currentUuid == uuid )
        return ;
    m_ui->metadataContainer->setWatchedClip( clip );
    m_clipRenderer->stop();
    m_currentUuid = uuid;
//    m_tag->clipSelected( clip );
    emit clipSelected( clip );
}

void
ImportController::importMedia( const QString &filePath )
{
    if ( Library::getInstance()->mediaAlreadyLoaded( filePath ) == true ||
         m_temporaryMedias->mediaAlreadyLoaded( filePath ) == true )
        return ;

    Media*  media = m_temporaryMedias->addMedia( filePath );
    if ( media == NULL )
    {
        qCritical() << "An error occurred while loading media:" << filePath;
        return ;
    }
    Clip*   clip = new Clip( media );
    media->setBaseClip( clip );
    m_temporaryMedias->addClip( clip );
    ++m_nbMediaToLoad;
    m_ui->progressBar->setMaximum( m_nbMediaToLoad );
}

void
ImportController::importDir( const QString &path )
{
    QDir            dir( path );
    QFileInfoList   files = dir.entryInfoList( m_nameFilters, QDir::NoDotAndDotDot | QDir::Readable
                                               | QDir::AllEntries );

    foreach ( QFileInfo fInfo, files )
    {
        if ( fInfo.isDir() == true )
            importDir( fInfo.absoluteFilePath() );
        else
            importMedia( fInfo.absoluteFilePath() );
    }
}

void
ImportController::forwardButtonClicked()
{
    QModelIndex     index = m_ui->treeView->selectionModel()->currentIndex();
    QString         filePath = m_filesModel->fileInfo( index ).filePath();

    if ( !m_filesModel->isDir( index ) )
        importMedia( filePath );
    else
        importDir( filePath );
}

void
ImportController::treeViewClicked( const QModelIndex& index )
{
    if ( m_filesModel->isDir( index ) )
    {
        m_currentlyWatchedDir = m_filesModel->filePath( index );
        saveCurrentPath();
    }
    m_ui->forwardButton->setEnabled( true );
}

void
ImportController::treeViewDoubleClicked( const QModelIndex& index )
{
    if ( m_filesModel->isDir( index ) == false )
        forwardButtonClicked();
}

void
ImportController::reject()
{
    m_clipRenderer->stop();
    m_mediaListView->clear();
    m_temporaryMedias->clear();
    collapseAllButCurrentPath();
    m_clipRenderer->setClip( NULL );
    done( Rejected );
}

void
ImportController::accept()
{
    m_mediaListView->clear();
    m_clipRenderer->stop();
    collapseAllButCurrentPath();
    foreach ( Clip* clip, m_temporaryMedias->clips().values() )
        Library::getInstance()->addClip( clip );
    m_temporaryMedias->removeAll();
    m_clipRenderer->setClip( NULL );
    done( Accepted );
}

void
ImportController::collapseAllButCurrentPath()
{
    m_ui->treeView->collapseAll();
    QStringList paths;
    for ( QDir directory( m_currentlyWatchedDir ); !directory.isRoot(); directory.cdUp() )
        paths.prepend( directory.absolutePath() );
    while ( paths.count() > 0 )
    {
        m_ui->treeView->setCurrentIndex( m_filesModel->index( paths.takeFirst() ) );
        m_ui->treeView->setExpanded( m_ui->treeView->currentIndex() , true );
    }
    m_ui->forwardButton->setEnabled( true );
}

void
ImportController::saveCurrentPath()
{
    QSettings s;
    s.setValue( "ImportPreviouslySelectPath", m_currentlyWatchedDir );
    s.sync();
}

void
ImportController::restoreCurrentPath()
{
    QSettings s;
    QVariant path = s.value( "ImportPreviouslySelectPath", QDir::homePath() );
    if ( QFile::exists( path.toString() ) == false )
        path = QDir::homePath();
    QFileInfo info = path.toString();
    m_currentlyWatchedDir = info.absoluteFilePath();
}

void
ImportController::mediaLoaded()
{
    ++m_nbMediaLoaded;
    if ( m_nbMediaLoaded == m_nbMediaToLoad )
    {
        m_nbMediaLoaded = 0;
        m_nbMediaToLoad = 0;
        m_ui->progressBar->hide();
    }
    else
    {
        if ( m_nbMediaToLoad > 3 )
            m_ui->progressBar->show();
        m_ui->progressBar->setValue( m_nbMediaLoaded );
    }

}

void
ImportController::failedToLoad( Media *media )
{
    m_ui->errorLabel->setText( tr( "Failed to load %1").arg(
            media->fileInfo()->fileName() ) );
    m_ui->errorLabelImg->show();
    m_ui->errorLabel->show();
    QTimer::singleShot( 3000, this, SLOT( hideErrors() ) );
    m_temporaryMedias->deleteClip( media->baseClip()->uuid() );
}

void
ImportController::hideErrors()
{
    m_ui->errorLabelImg->hide();
    m_ui->errorLabel->hide();
}

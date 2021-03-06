/*****************************************************************************
 * SettingsManager.cpp: Backend settings manager
 *****************************************************************************
 * Copyright (C) 2008-2014 VideoLAN
 *
 * Authors: Ludovic Fauvet <etix@l0cal.com>
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

#ifndef GENERALPAGE_H
#define GENERALPAGE_H

#include "ui_GeneralPage.h"

class GeneralPage : public QWizardPage
{
    Q_OBJECT
public:
    GeneralPage( QWidget *parent = 0 );

protected:
    virtual void        changeEvent( QEvent *e );
    virtual int         nextId() const;
    virtual void        initializePage();
    virtual bool        validatePage();
    virtual void        cleanupPage();

private:
    void                setValidity( bool status );

private slots:
    void                openWorkspaceDirectory();
    void                updateProjectLocation();

private:
    Ui::GeneralPage     ui;
    QPalette            pValid;
    QPalette            pInvalid;
    bool                m_valid;
};

#endif // GENERALPAGE_H

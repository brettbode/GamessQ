/*
 * Copyright (C) 2008 Iowa State University 
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "job_options_dialog.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "job_options_dialog.h"
#include "resources.h"
#include <wx/filename.h>

////@begin XPM images
////@end XPM images


/*!
 * JobOptionsDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( JobOptionsDialog, wxDialog )


/*!
 * JobOptionsDialog event table definition
 */

BEGIN_EVENT_TABLE( JobOptionsDialog, wxDialog )

////@begin JobOptionsDialog event table entries
////@end JobOptionsDialog event table entries

END_EVENT_TABLE()


/*!
 * JobOptionsDialog constructors
 */

JobOptionsDialog::JobOptionsDialog()
{
    Init();
}

JobOptionsDialog::JobOptionsDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * JobOptionsDialog creator
 */

bool JobOptionsDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin JobOptionsDialog creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    SetIcon(GetGamessQIcon());
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end JobOptionsDialog creation
    return true;
}


/*!
 * JobOptionsDialog destructor
 */

JobOptionsDialog::~JobOptionsDialog()
{
////@begin JobOptionsDialog destruction
////@end JobOptionsDialog destruction
}


/*!
 * Member initialisation
 */

void JobOptionsDialog::Init()
{
////@begin JobOptionsDialog member initialisation
    procsSpinCtrl = NULL;
////@end JobOptionsDialog member initialisation
}


/*!
 * Control creation for JobOptionsDialog
 */

void JobOptionsDialog::CreateControls()
{    
////@begin JobOptionsDialog content construction
    JobOptionsDialog* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("Processors:"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
    itemBoxSizer3->Add(itemStaticText4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    procsSpinCtrl = new wxSpinCtrl( itemDialog1, ID_SPINCTRL1, wxT("1"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 256, 1 );
    itemBoxSizer3->Add(procsSpinCtrl, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer6 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer6, 0, wxGROW|wxALL, 5);
    wxButton* itemButton7 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer6->AddButton(itemButton7);

    wxButton* itemButton8 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer6->AddButton(itemButton8);

    itemStdDialogButtonSizer6->Realize();

////@end JobOptionsDialog content construction
}


/*!
 * Should we show tooltips?
 */

bool JobOptionsDialog::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap JobOptionsDialog::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin JobOptionsDialog bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end JobOptionsDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon JobOptionsDialog::GetIconResource( const wxString& name )
{
    return wxNullIcon;
}

/*!
 * \return The value of the "Number of Processors" field
 */
int JobOptionsDialog::GetNumProcessors()
{
	return procsSpinCtrl->GetValue();
}


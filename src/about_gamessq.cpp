/////////////////////////////////////////////////////////////////////////////
// Name:        about_gamessq.cpp
// Purpose:     
// Author:      
// Modified by: 
// Created:     Thu 03 Jul 2008 01:44:43 PM CDT
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "about_gamessq.h"
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

#include "about_gamessq.h"

////@begin XPM images
////@end XPM images


/*!
 * AboutGamessQ type definition
 */

IMPLEMENT_DYNAMIC_CLASS( AboutGamessQ, wxDialog )


/*!
 * AboutGamessQ event table definition
 */

BEGIN_EVENT_TABLE( AboutGamessQ, wxDialog )

////@begin AboutGamessQ event table entries
////@end AboutGamessQ event table entries

END_EVENT_TABLE()


/*!
 * AboutGamessQ constructors
 */

AboutGamessQ::AboutGamessQ()
{
    Init();
}

AboutGamessQ::AboutGamessQ( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * AboutGamessQ creator
 */

bool AboutGamessQ::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin AboutGamessQ creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end AboutGamessQ creation
    return true;
}


/*!
 * AboutGamessQ destructor
 */

AboutGamessQ::~AboutGamessQ()
{
////@begin AboutGamessQ destruction
////@end AboutGamessQ destruction
}


/*!
 * Member initialisation
 */

void AboutGamessQ::Init()
{
////@begin AboutGamessQ member initialisation
////@end AboutGamessQ member initialisation
}


/*!
 * Control creation for AboutGamessQ
 */

void AboutGamessQ::CreateControls()
{    
////@begin AboutGamessQ content construction
    AboutGamessQ* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxStaticText* itemStaticText3 = new wxStaticText( itemDialog1, wxID_STATIC, _("GamessQ 1.2.1"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText3->SetFont(wxFont(20, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Sans")));
    itemBoxSizer2->Add(itemStaticText3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("A queue manager for GAMESS"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemStaticText4, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("Written by Jason Ekstrand\nComissioned by Dr. Brett Bode\nCopyright (C) 2008 Iowa State University\n\nGamessQ homepage: http://www.msg.chem.iastate.edu/GAMESS/GamessQ/\n\nBug reports should be sent to: MMP.Guy@gmail.com\n\nThis program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.\n\nThis program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.\n\nYou should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.\n\nToolbar icons for this program were taken from the Tango Desktop Project at <http://tango.freedesktop.org> and are redistributed in accordance with the Creative Commons Attribution Share-Alike License which can be found at <http://creativecommons.org/licenses/by-sa/2.5/>\n"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
    itemStaticText5->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxNORMAL, false, wxT("Sans")));
    itemStaticText5->Wrap(480);
    itemBoxSizer2->Add(itemStaticText5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer6 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer6, 0, wxALIGN_RIGHT|wxALL, 5);
    wxButton* itemButton7 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer6->AddButton(itemButton7);

    itemStdDialogButtonSizer6->Realize();

////@end AboutGamessQ content construction
}


/*!
 * Should we show tooltips?
 */

bool AboutGamessQ::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap AboutGamessQ::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin AboutGamessQ bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end AboutGamessQ bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon AboutGamessQ::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin AboutGamessQ icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end AboutGamessQ icon retrieval
}

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
    itemStaticText3->SetFont(wxFont(20, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans")));
    itemBoxSizer2->Add(itemStaticText3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("A queue manager for GAMESS"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemStaticText4, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("Written by Jason Ekstrand\nComissioned by Dr. Brett Bode\nCopyright (C) 2008 Iowa State University"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
    itemStaticText5->SetFont(wxFont(8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Sans")));
    itemStaticText5->Wrap(480);
    itemBoxSizer2->Add(itemStaticText5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer6, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText7 = new wxStaticText( itemDialog1, wxID_STATIC, _("GamessQ homepage:"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
    itemStaticText7->SetFont(wxFont(8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Sans")));
    itemBoxSizer6->Add(itemStaticText7, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxHyperlinkCtrl* itemHyperlinkCtrl8 = new wxHyperlinkCtrl( itemDialog1, ID_HYPERLINKCTRL, _("http://www.msg.chem.iastate.edu/GAMESS/GamessQ/"), wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE|wxHL_ALIGN_CENTRE );
    itemHyperlinkCtrl8->SetFont(wxFont(8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Sans")));
    itemBoxSizer6->Add(itemHyperlinkCtrl8, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer9, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText10 = new wxStaticText( itemDialog1, wxID_STATIC, _("For questions, comments or bugs create an issue at:"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
    itemStaticText10->SetFont(wxFont(8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Sans")));
    itemBoxSizer9->Add(itemStaticText10, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxHyperlinkCtrl* itemHyperlinkCtrl11 = new wxHyperlinkCtrl( itemDialog1, ID_HYPERLINKCTRL1, _("https://github.com/brettbode/GamessQ/issues"), wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE|wxHL_ALIGN_CENTRE );
    itemHyperlinkCtrl11->SetFont(wxFont(8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Sans")));
    itemBoxSizer9->Add(itemHyperlinkCtrl11, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText12 = new wxStaticText( itemDialog1, wxID_STATIC, _("This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.\n\nThis program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.\n\nYou should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.\n\nToolbar icons for this program were taken from the Tango Desktop Project at <http://tango.freedesktop.org> and are redistributed in accordance with the Creative Commons Attribution Share-Alike License which can be found at <http://creativecommons.org/licenses/by-sa/2.5/>"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
    itemStaticText12->SetFont(wxFont(8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Sans")));
    itemStaticText12->Wrap(480);
    itemBoxSizer2->Add(itemStaticText12, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer13 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer13, 0, wxALIGN_RIGHT|wxALL, 5);
    wxButton* itemButton14 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer13->AddButton(itemButton14);

    itemStdDialogButtonSizer13->Realize();

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

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
#pragma implementation "confirmoverwrite.h"
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

#include "confirm_overwrite.h"
#include <wx/artprov.h>
#include <wx/filename.h>
#include "resources.h"

////@begin XPM images
////@end XPM images


/*!
 * ConfirmOverwrite type definition
 */

IMPLEMENT_DYNAMIC_CLASS( ConfirmOverwrite, wxDialog )


/*!
 * ConfirmOverwrite event table definition
 */

BEGIN_EVENT_TABLE( ConfirmOverwrite, wxDialog )

////@begin ConfirmOverwrite event table entries
////@end ConfirmOverwrite event table entries

END_EVENT_TABLE()


/*!
 * ConfirmOverwrite constructors
 */

ConfirmOverwrite::ConfirmOverwrite()
{
    Init();
}

ConfirmOverwrite::ConfirmOverwrite( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * ConfirmOverwrite creator
 */

bool ConfirmOverwrite::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    SetIcon(GetGamessQIcon());
    Centre();
    return true;
}


/*!
 * ConfirmOverwrite destructor
 */

ConfirmOverwrite::~ConfirmOverwrite()
{
////@begin ConfirmOverwrite destruction
////@end ConfirmOverwrite destruction
}


/*!
 * Member initialisation
 */

void ConfirmOverwrite::Init()
{
////@begin ConfirmOverwrite member initialisation
    mTextBox = NULL;
////@end ConfirmOverwrite member initialisation
}


/*!
 * Control creation for ConfirmOverwrite
 */

void ConfirmOverwrite::CreateControls()
{    
////@begin ConfirmOverwrite content construction
    ConfirmOverwrite* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticBitmap* itemStaticBitmap4 = new wxStaticBitmap( itemDialog1, wxID_STATIC, itemDialog1->GetBitmapResource(wxT("message/wxQuestion")), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemStaticBitmap4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("The following files will be overwritten. Are you Sure?"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText5->SetFont(wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans")));
    itemStaticText5->Wrap(420);
    itemBoxSizer3->Add(itemStaticText5, 0, wxGROW|wxALL, 5);

    mTextBox = new wxTextCtrl( itemDialog1, ID_TEXTCTRL3, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxHSCROLL );
    itemBoxSizer2->Add(mTextBox, 1, wxGROW|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer7 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer7, 0, wxALIGN_RIGHT|wxALL, 5);
    wxButton* itemButton8 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer7->AddButton(itemButton8);

    wxButton* itemButton9 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer7->AddButton(itemButton9);

    itemStdDialogButtonSizer7->Realize();

////@end ConfirmOverwrite content construction
}


/*!
 * Should we show tooltips?
 */

bool ConfirmOverwrite::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap ConfirmOverwrite::GetBitmapResource( const wxString& name )
{
	return GetGamessQBitmap(name);
}

/*!
 * Get icon resources
 */

wxIcon ConfirmOverwrite::GetIconResource( const wxString& name )
{
    return wxNullIcon;
}

/*! Creates a new "Confirm Overwrite" dialog for the given files.
 *
 * \param parent	Parent window
 * \param filenames	wxArrayString containing the names of the files being
 *					overwritten
 */
ConfirmOverwrite::ConfirmOverwrite(wxWindow *parent, wxArrayString &filenames)
{
    Init();
	Create( parent, ID_CONFIRMOVERWRITE, SYMBOL_CONFIRMOVERWRITE_TITLE,
			SYMBOL_CONFIRMOVERWRITE_POSITION, SYMBOL_CONFIRMOVERWRITE_SIZE,
			SYMBOL_CONFIRMOVERWRITE_STYLE );

	int num = (int) filenames.Count();
	wxString text;
	for (int i = 0; i < num; i ++) {
		text << filenames.Item(i) << wxT('\n');
	}
	mTextBox->WriteText(text);
}


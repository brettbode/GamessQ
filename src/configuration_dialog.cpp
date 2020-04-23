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
#pragma implementation "configuration_dialog.h"
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

#include "configuration_dialog.h"
#include "resources.h"
#include "gamessq_common.h"
#include <wx/filename.h>

////@begin XPM images
////@end XPM images


#include <wx/filename.h>
#include "log_viewer.h"

/*!
 * ConfigurationDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( ConfigurationDialog, wxDialog )


/*!
 * ConfigurationDialog event table definition
 */

BEGIN_EVENT_TABLE( ConfigurationDialog, wxDialog )

////@begin ConfigurationDialog event table entries
    EVT_BUTTON( ID_SPOOL_BROWSE, ConfigurationDialog::OnSpoolBrowseClick )

    EVT_BUTTON( ID_GAMESS_BROWSE, ConfigurationDialog::OnGamessBrowseClick )

    EVT_BUTTON( ID_LOGS, ConfigurationDialog::OnLogsClick )

    EVT_BUTTON( wxID_OK, ConfigurationDialog::OnOkClick )

////@end ConfigurationDialog event table entries

END_EVENT_TABLE()


/*!
 * ConfigurationDialog constructors
 */

ConfigurationDialog::ConfigurationDialog()
{
    Init();
}

ConfigurationDialog::ConfigurationDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * ConfigurationDialog creator
 */

bool ConfigurationDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    SetIcon(GetGamessQIcon());
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
    return true;
}


/*!
 * ConfigurationDialog destructor
 */

ConfigurationDialog::~ConfigurationDialog()
{
////@begin ConfigurationDialog destruction
////@end ConfigurationDialog destruction
}


/*!
 * Member initialisation
 */

void ConfigurationDialog::Init()
{
////@begin ConfigurationDialog member initialisation
    refreshFreqSpinCtrl = NULL;
    mSpoolDir = NULL;
    mGamessPath = NULL;
////@end ConfigurationDialog member initialisation
}


/*!
 * Control creation for ConfigurationDialog
 */

void ConfigurationDialog::CreateControls()
{    
////@begin ConfigurationDialog content construction
    ConfigurationDialog* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer3, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer3->Add(itemBoxSizer4, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("Refresh Frequency:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer4->Add(itemStaticText5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    refreshFreqSpinCtrl = new wxSpinCtrl( itemDialog1, ID_SPINCTRL2, wxT("10"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 3600, 10 );
    itemBoxSizer4->Add(refreshFreqSpinCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText7 = new wxStaticText( itemDialog1, wxID_STATIC, _("Seconds"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer4->Add(itemStaticText7, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

	wxStaticText* itemStaticText8a = new wxStaticText( itemDialog1, wxID_STATIC, _("Paths should not contain spaces or non-ASCII characters!"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
    itemBoxSizer3->Add(itemStaticText8a, 1, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText8 = new wxStaticText( itemDialog1, wxID_STATIC, _("Spool Directory:"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
    itemBoxSizer3->Add(itemStaticText8, 1, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer3->Add(itemBoxSizer9, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    mSpoolDir = new wxTextCtrl( itemDialog1, ID_TEXTCTRL1, wxT(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer9->Add(mSpoolDir, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxButton* itemButton11 = new wxButton( itemDialog1, ID_SPOOL_BROWSE, _("Browse..."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer9->Add(itemButton11, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

    wxStaticText* itemStaticText12 = new wxStaticText( itemDialog1, wxID_STATIC, _("GAMESS Path:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemStaticText12, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxBoxSizer* itemBoxSizer13 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer3->Add(itemBoxSizer13, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    mGamessPath = new wxTextCtrl( itemDialog1, ID_TEXTCTRL, wxT(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer13->Add(mGamessPath, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxButton* itemButton15 = new wxButton( itemDialog1, ID_GAMESS_BROWSE, _("Browse..."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer13->Add(itemButton15, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

    wxButton* itemButton16 = new wxButton( itemDialog1, ID_LOGS, _("View Queue Logs"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemButton16, 0, wxALIGN_RIGHT|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer17 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer17, 0, wxGROW|wxALL, 5);
    wxButton* itemButton18 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer17->AddButton(itemButton18);

    wxButton* itemButton19 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer17->AddButton(itemButton19);

    itemStdDialogButtonSizer17->Realize();

////@end ConfigurationDialog content construction
}


/*! \return True
 */

bool ConfigurationDialog::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap ConfigurationDialog::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin ConfigurationDialog bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end ConfigurationDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon ConfigurationDialog::GetIconResource( const wxString& name )
{
    return wxNullIcon;
}


/*! Sets the value of the "Refresh Frequency" field
 *
 * \param refreshFrequency New value
 */
void ConfigurationDialog::SetRefreshFrequency(int refreshFrequency)
{
	refreshFreqSpinCtrl->SetValue(refreshFrequency);
}

/*! \return The value of the "Refresh Frequency" field.
 */
int ConfigurationDialog::GetRefreshFrequency()
{
	return refreshFreqSpinCtrl->GetValue();
}

/*! Sets the value of the "Spool Directory" field
 *
 * \param spoolDir New value
 */
void ConfigurationDialog::SetSpoolDir(wxString spoolDir)
{
	if (spoolDir.Len()>0) {
		if (ValidatePath(spoolDir))
			mSpoolDir->SetValue(spoolDir);
		else
			wxLogError(wxT("Non-allowed characters in the path. Please retry!"));
	}
}

/*! \return The value of the "Spool Directory" field.
 */
wxString ConfigurationDialog::GetSpoolDir()
{
	return mSpoolDir->GetValue();
}

/*! Sets the value of the "GAMESS Directory" field
 *
 * \param gamessPath New value
 */
void ConfigurationDialog::SetGamessPath(wxString gamessPath)
{
	if (gamessPath.Len()>0) {
		if (ValidatePath(gamessPath))
			mGamessPath->SetValue(gamessPath);
		else
			wxLogError(wxT("Non-allowed characters in the path. Please retry!"));
	}
}

/*! \return The value of the "GAMESS Directory" field.
 */
wxString ConfigurationDialog::GetGamessPath()
{
	return mGamessPath->GetValue();
}

/*! Launches a directory selection dialog, and sets the value of the "Spool
 * Directory" field to the result.
 */

void ConfigurationDialog::OnSpoolBrowseClick( wxCommandEvent& event )
{
	wxDirDialog *dialog = new wxDirDialog(this, wxT("Select Spool Directory"),
			GetSpoolDir(), wxDD_NEW_DIR_BUTTON);
	if (dialog->ShowModal() == wxID_OK) {
		SetSpoolDir(dialog->GetPath());
	}
	delete dialog;
}


/*! Launches a directory selection dialog, and sets the value of the "GAMESS
 * Directory" field to the result.
 */

void ConfigurationDialog::OnGamessBrowseClick( wxCommandEvent& event )
{
	wxDirDialog *dialog = new wxDirDialog(this, wxT("Select GAMESS Directory"),
			GetGamessPath(), wxDD_DEFAULT_STYLE);
	if (dialog->ShowModal() == wxID_OK)
		SetGamessPath(dialog->GetPath());
	delete dialog;
}


/*! Creates a new LogViewer with only the server log file, and displays it.
 */

void ConfigurationDialog::OnLogsClick( wxCommandEvent& event )
{
	wxFileName filename(mSpoolDir->GetValue(), QUEUE_LOG_FILENAME);
	wxArrayString arr;
	arr.Add(filename.GetFullPath());
	LogViewer *viewer = new LogViewer(this, arr);
	viewer->Show();
}


/*!
 * Relays the OK click event to the parent window.
 */

void ConfigurationDialog::OnOkClick( wxCommandEvent& event )
{
	if (!ValidatePath(mSpoolDir->GetValue())) {
		wxLogError(wxT("Invalid characters in the spool path. Please try again."));
		return;
	}
	if (!ValidatePath(mGamessPath->GetValue())) {
		wxLogError(wxT("Invalid characters in the path to GAMESS. Please try again."));
		return;
	}
	wxCommandEvent okEvent = wxCommandEvent(wxEVT_COMMAND_MENU_SELECTED,
			ID_SETUP_OK);
#if wxCHECK_VERSION(3, 0, 0)
    GetParent()->ProcessWindowEvent(okEvent);
#else
    GetParent()->AddPendingEvent(okEvent);
#endif
	Show(false);
}

bool ValidatePath(const wxString & test) {
	return (test.IsAscii() && !(test.Contains(" ")));
}

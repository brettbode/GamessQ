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

#include "log_viewer.h"

#include <wx/filename.h>
#include <wx/button.h>
#include <wx/ffile.h>

#include "gamessq_common.h"
#include "resources.h"

BEGIN_EVENT_TABLE(LogViewer, wxFrame)
	EVT_CLOSE(LogViewer::OnClose)
    EVT_BUTTON(wxID_OK, LogViewer::OnOkClick)
    EVT_BUTTON(wxID_REFRESH, LogViewer::OnRefreshClick)
END_EVENT_TABLE()

/*!
 * Creates a new LogViewer object with the given parent window and list of
 * filenames.
 *
 * \param parent	Parent window
 * \param filenames	List of files to open
 */
LogViewer::LogViewer(wxWindow *parent, wxArrayString filenames)
{
	wxString title = wxT("Log File Viewer");
	if (filenames.Count() == 1) {
		wxFileName name(filenames.Item(0));
		title << wxT(": ") << name.GetName();
	}
	Create(parent, -1, title, wxDefaultPosition, wxSize(600, 480),
			wxDEFAULT_FRAME_STYLE | wxRESIZE_BORDER);
    SetIcon(GetGamessQIcon());
//	SetBackgroundColour(wxNullColour);

	if (filenames.Count() == 0) {
		return;
	}

	mSizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(mSizer);

	mNotebook = new wxNotebook(this, -1, wxDefaultPosition, wxDefaultSize,
				wxNB_TOP);
	mSizer->Add(mNotebook, 1, wxEXPAND | wxALL, 5);

	mLogPages = new LogPage*[filenames.Count()];
	mNumPages = (int) filenames.Count();
	for (int i = 0; i < mNumPages; i ++) {
		mLogPages[i] = new LogPage(this, filenames.Item(i));
		mNotebook->AddPage(mLogPages[i]->mPage, mLogPages[i]->mName);
	}

	wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	mSizer->Add(buttonSizer, 0, wxALIGN_RIGHT);
    wxButton* refreshButton = new wxButton( this, wxID_REFRESH, wxT("Refresh"),
			wxDefaultPosition, wxDefaultSize, 0 );
	buttonSizer->Add(refreshButton, 0, wxALL, 5);
    wxButton* okButton = new wxButton( this, wxID_OK, wxT("&OK"),
			wxDefaultPosition, wxDefaultSize, 0 );
	buttonSizer->Add(okButton, 0, wxALL, 5);
	Refresh();
}

LogViewer::~LogViewer()
{
	// delete all the pages
	for (int i = 0; i < mNumPages; i ++) {
		delete mLogPages[i];
	}
	delete mLogPages;
}

/*!
 * Goes through all the pages and refreshs them, if the file that page
 * displayed has been deleted, the page is then deleted. If only one page is
 * left, the wxNotebook is removed, so only the text control remains in the
 * wineow. If all the pages have been removed, the windows is closed.
 */
void LogViewer::Refresh()
{
	for (int i = 0; i < mNumPages;) {
		if (mLogPages[i]->Refresh()) {
			i ++;
		} else {
			mNotebook->DeletePage(i);
			delete mLogPages[i];
			mNumPages --;
			for (int j = i; j < mNumPages; j++) {
				mLogPages[j] = mLogPages[j + 1];
			}
		}
	}

	if (mNumPages == 0) {
//		Close();
	} else if (mNotebook && mNumPages == 1) {
		mLogPages[0]->mTextCtrl->Reparent(this);
		delete mNotebook;
		mNotebook = NULL;
		mSizer->Prepend(mLogPages[0]->mTextCtrl, 1, wxEXPAND | wxALL, 5);
	}
}

/*!
 * Calls Refresh()
 *
 * \param event The button click event
 */
void LogViewer::OnRefreshClick(wxCommandEvent &event)
{
	Refresh();
}

/*!
 * Function called when someone clicks the "OK" button.
 * 
 * Closes the window.
 *
 * \param event	The button click event
 */
void LogViewer::OnOkClick(wxCommandEvent &event)
{
	Close();
}

/*!
 * Function called when the window is closed.
 *
 * Schedules the window to be destroyed, so that it can be constructed on the
 * heap and left.
 *
 * \param event The close event
 */
void LogViewer::OnClose(wxCloseEvent &event)
{
	Destroy();
}

/*
 * LogPage implementation
 */

/*!
 * Creates a new LogPage object with the appropriate controls and sizers.
 *
 * \note The text control is not filled until a call to Refresh().
 */
LogViewer::LogPage::LogPage(LogViewer *parent, wxString filename)
{
	wxFileName name(filename);
	mName = name.GetName();
	mFilename = filename;
	mSizer = new wxBoxSizer(wxVERTICAL);
	mPage = new wxPanel(parent->mNotebook);

	mTextCtrl = new wxTextCtrl(mPage, -1, wxT(""), wxDefaultPosition,
			wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE | wxHSCROLL);
	mTextCtrl->SetFont(wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL,
			wxFONTWEIGHT_NORMAL));
	mPage->SetSizer(mSizer);
	mSizer->Add(mTextCtrl, 1, wxEXPAND);
}

/*!
 * Loads the file into the text control.
 *
 * \return True upon success, False otherwise.
 */
bool LogViewer::LogPage::Refresh()
{
	if (! wxFileExists(mFilename)) {
		return false;
	}
	// Hiding the text control while refreshing it ill speed things up slightly
	mTextCtrl->Show(false);
	if (! mTextCtrl->LoadFile(mFilename)) {
		return false;
	}
	mTextCtrl->Show(true);
	return true;
}

/*!
 * Detaches the text control from the sizer, so that it doesn't get deleted
 * along with everything else.
 */
void LogViewer::LogPage::DetachTextCtrl()
{
	mSizer->Detach(mTextCtrl);
}


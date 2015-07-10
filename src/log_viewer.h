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

#ifndef __LOG_VIEWER_H__
#define __LOG_VIEWER_H__

#include <wx/frame.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/event.h>
#include <wx/arrstr.h>

//! Log file viewer
/*!
 * This class provides a tabbed file viewer. When the object is constructed,
 * you pass it a wxArrayString containing one or more filenames. Each one of
 * these files is then opened in a separate tab.
 */
class LogViewer : public wxFrame
{
	DECLARE_EVENT_TABLE()

	public:
		//! Constructor
		LogViewer(wxWindow *parent, wxArrayString filenames);

		//! Destructor
		~LogViewer();

	private:
		//! Log page
		/*!
		 * This class represents a page in the log file viewer. It acts as a
		 * container for the wxWidgets display elements.
		 */
		class LogPage {
			public:
				//! Constructor
				LogPage(LogViewer *parent, wxString filename);

				//! Refresh the page
				bool Refresh();

				//! Detatch the text control from the sizer
				void DetachTextCtrl();

				//! The wxPanel object
				wxPanel *mPage;

				//! The wxTextCtrl object
				wxTextCtrl *mTextCtrl;

				//! The full path
				wxString mFilename;

				//! The name of the file (no path or extension)
				wxString mName;

			private:
				//! The main sizer object
				wxBoxSizer *mSizer;
		};

		//! Refresh all the pages
		void Refresh();

		//! "Refresh event handler
		void OnRefreshClick(wxCommandEvent &event);

		//! "OK" event handler
		void OnOkClick(wxCommandEvent &event);

		//! Window close event handler
		void OnClose(wxCloseEvent &event);

		//! Number of pages
		int mNumPages;

		//! Array of pointers to the LogPage objects for all the pages
		LogPage **mLogPages;

		//! The main wxSizer object
		wxBoxSizer *mSizer;

		//! the wxNotebook object
		wxNotebook *mNotebook;
};

#endif /* __LOG_VIEWER_H__ */

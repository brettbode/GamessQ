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

#include <wx/filename.h>
#include <wx/artprov.h>
#include <wx/stdpaths.h>
#include "resources.h"

wxBitmap GetGamessQBitmap(wxString name)
{
	if (name == wxT("menu/wxSave")) {
		return wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_MENU);
	} else if (name == wxT("menu/wxSaveAs")) {
		return wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS, wxART_MENU);
	} else if (name == wxT("message/wxQuestion")) {
		return wxArtProvider::GetBitmap(wxART_QUESTION, wxART_MESSAGE_BOX);
	}
    wxStandardPathsBase & gStdPaths = wxStandardPaths::Get();
#if defined __WINDOWS__
	name = gStdPaths.GetDataDir() + wxT("\\") + name;
	return wxBitmap(name, wxBITMAP_TYPE_PNG);
#elif defined __WXMAC__
    wxFileName filename(name);
	name = wxFileName(gStdPaths.GetExecutablePath()).GetPath() +
			wxT("/../Resources/") + filename.GetFullName();
	return wxBitmap(name, wxBITMAP_TYPE_PNG);
#else
#ifdef __WXGTK20__
	if (name == wxT("icons/add-16.png")) {
		return wxArtProvider::GetBitmap(wxT("gtk-add"), wxART_MENU);
	} else if (name == wxT("icons/add-24.png")) {
		return wxArtProvider::GetBitmap(wxT("gtk-add"), wxART_TOOLBAR);
	} else if (name == wxT("icons/cancel-16.png")) {
		return wxArtProvider::GetBitmap(wxT("gtk-cancel"), wxART_MENU);
	} else if (name == wxT("icons/cancel-24.png")) {
		return wxArtProvider::GetBitmap(wxT("gtk-cancel"), wxART_TOOLBAR);
	} else if (name == wxT("icons/clear-16.png")) {
		return wxArtProvider::GetBitmap(wxT("gtk-clear"), wxART_MENU);
	} else if (name == wxT("icons/clear-24.png")) {
		return wxArtProvider::GetBitmap(wxT("gtk-clear"), wxART_TOOLBAR);
	} else if (name == wxT("icons/pause-16.png")) {
		return wxArtProvider::GetBitmap(wxT("gtk-media-pause"), wxART_MENU);
	} else if (name == wxT("icons/pause-24.png")) {
		return wxArtProvider::GetBitmap(wxT("gtk-media-pause"), wxART_TOOLBAR);
	} else if (name == wxT("icons/refresh-16.png")) {
		return wxArtProvider::GetBitmap(wxT("gtk-refresh"), wxART_MENU);
	} else if (name == wxT("icons/refresh-24.png")) {
		return wxArtProvider::GetBitmap(wxT("gtk-refresh"), wxART_TOOLBAR);
	} else if (name == wxT("icons/remove-16.png")) {
		return wxArtProvider::GetBitmap(wxT("gtk-remove"), wxART_MENU);
	} else if (name == wxT("icons/remove-24.png")) {
		return wxArtProvider::GetBitmap(wxT("gtk-remove"), wxART_TOOLBAR);
	} else if (name == wxT("icons/resume-16.png")) {
		return wxArtProvider::GetBitmap(wxT("gtk-media-play"), wxART_MENU);
	} else if (name == wxT("icons/resume-24.png")) {
		return wxArtProvider::GetBitmap(wxT("gtk-media-play"), wxART_TOOLBAR);
	} else if (name == wxT("icons/resume-24.png")) {
		return wxArtProvider::GetBitmap(wxT("gtk-media-play"), wxART_TOOLBAR);
	}
#endif /* __WXGTK20__ */
	wxFileName filename(name);
	name = wxT(DATA_DIR) + (wxT("/icons/") + filename.GetFullName());
	return wxBitmap(name, wxBITMAP_TYPE_PNG);
#endif
}

wxIcon GetGamessQIcon()
{
    wxStandardPathsBase & gStdPaths = wxStandardPaths::Get();
#if defined __WINDOWS__
	return wxIcon(gStdPaths.GetDataDir() +
			wxT("\\gamessq.exe"), wxBITMAP_TYPE_ICO, 16, 16);
#elif defined __WXMAC__
    return wxIcon(wxFileName(gStdPaths.GetExecutablePath()).GetPath() +
			wxT("/../Resources/gamessq-16.png"), wxBITMAP_TYPE_PNG);
#else /* Linux */
	return wxIcon(wxString(wxT(DATA_DIR)) + wxT("/icons/gamessq-16.png"),
			wxBITMAP_TYPE_PNG);
#endif
}

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

#ifndef __GAMESSQ_COMMON_H__
#define __GAMESSQ_COMMON_H__

#include <wx/log.h>
#include <wx/string.h>

/*! Log System Error
 *
 * Simple macro for logging system errors, outputs given string, then a collon
 * and then the system error message.
 */
#define LOG_ERROR(x) wxLogError(wxString(wxT(x)) + wxT(": ") + wxSysErrorMsg())

/*! Network Commands
 *
 * Defines all the commands used by the client and server to communitate
 */
enum {
#ifndef __WINDOWS__
	SOCK_CONNECTED,
	SOCK_VERIFY,
	SOCK_COMMAND,
	SOCK_REQUEST,
	SOCK_TRUE,
	SOCK_FALSE,
#endif
	JOB_REQUEST_NUMBER = 'A', // non-standard symbols might cause problems
	JOB_REQUEST_IDS,
	JOB_REQUEST_NAME,
	JOB_REQUEST_STATUS,
	JOB_REQUEST_PROCS,
	JOB_REQUEST_FILENAME,
	REQUEST_SPOOLDIR,
	REQUEST_GAMESSDIR,
	REQUEST_ACTIVE,
	JOB_COMMAND_ADD,
	JOB_COMMAND_REMOVE,
	JOB_COMMAND_PAUSE,
	JOB_COMMAND_RESUME,
	JOB_COMMAND_CANCEL,
	JOB_COMMAND_SET_NAME,
	JOB_COMMAND_SAVE,
	COMMAND_SPOOLDIR,
	COMMAND_PAUSE_ALL,
	COMMAND_RESUME_ALL,
	COMMAND_GAMESSDIR,
	COMMAND_CLEAR
};

#ifdef __WXMAC__
#define DEFAULT_GAMESS_PATH	"../../../gamess"
#endif /* __WXMAC__ */

#ifdef __WINDOWS__
#define DEFAULT_GAMESS_PATH "WinGAMESS"
#endif /* __WINDOWS__ */


#ifdef __WINDOWS__

//! Name of the DDE service
#define WX_SERVICE			wxT("gamessq")

//! The DDE topic
#define WX_TOPIC			wxT("GamessQ")

//! The name of the backend executable
#define GAMESSQD_EXEC_NAME	wxT("gamessqd.exe")

#else /* __WINDOWS__ */

//! The name of the backend executable
#define GAMESSQD_EXEC_NAME	wxT("gamessqd")

//! The filename for the unix domain socket
#define SOCKETNAME			"/tmp/gamessq"

#endif /* __WINDOWS__ */

//! Application name used for config stuff (registry entries, .files etc.)
#define WX_CONFIG_APPNAME	wxT("gamessq")

//! Name of the saved queue data file
#define QUEUE_DAT_FILENAME	wxT("queue.dat")

//! Name of the server log file
#define QUEUE_LOG_FILENAME	wxT("queue.log")

//! General function to validate that a path does not contain any spaces or other nonallowed characters.
bool ValidatePath(const wxString & t);

#endif /* __GAMESSQ_COMMON_H__ */


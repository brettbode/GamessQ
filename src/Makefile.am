#
# Copyright (C) 2008 Iowa State University 
#
# This program is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
# details.
#
# You should have received a copy of the GNU General Public License along with
# this program.  If not, see <http://www.gnu.org/licenses/>.
#

if WINDOWS
if HAS_WINDRES
.rc.o:
	$(WINDRES) $^ -o $@
endif
endif

bin_PROGRAMS = \
	gamessq gamessqd

gamessq_SOURCES = \
	resources.cpp				resources.h				\
	job.cpp						job.h					\
	confirm_overwrite.cpp		confirm_overwrite.h		\
	configuration_dialog.cpp	configuration_dialog.h	\
	job_options_dialog.cpp		job_options_dialog.h	\
	log_viewer.cpp				log_viewer.h			\
	job_queue_manager.cpp		job_queue_manager.h		\
	gamessq_frame.cpp			gamessq_frame.h			\
	gamessq.cpp					gamessq.h				\
	about_gamessq.cpp			about_gamessq.h			\
	gamessq_client.h			gamessq_common.h

gamessqd_SOURCES = \
	job.cpp						job.h					\
	gamessqd.cpp				gamessqd.h

if WINDOWS
gamessq_SOURCES += $(gamessq_DEFAULT_SOURCES)			\
	gamessq_windows_client.cpp	gamessq_windows_client.h \
	windows_icon.rc

gamessqd_SOURCES += $(gamessqd_DEFAULT_SOURCES)			\
	gamessqd_windows.cpp		gamessqd_windows.h		\
	windows_job.cpp				windows_job.h			\
	log_viewer.cpp				log_viewer.h			\
	resources.cpp				resources.h				\
	windows_icon.rc

else
gamessq_SOURCES += $(gamessq_DEFAULT_SOURCES)			\
	gamessq_posix_client.cpp	gamessq_posix_client.h

gamessqd_SOURCES += $(gamessqd_DEFAULT_SOURCES)			\
	gamessqd_posix.cpp			gamessqd_posix.h		\
	posix_job.cpp				posix_job.h

endif

AM_CXXFLAGS = $(WX_CXXFLAGS)

gamessq_LDADD = $(WX_LIBS)

gamessqd_LDADD = $(WX_LIBS)

nobase_dist_pkgdata_DATA= \
	icons/add-16.png		icons/add-24.png		\
	icons/cancel-16.png		icons/cancel-24.png		\
	icons/clear-16.png		icons/clear-24.png		\
	icons/pause-16.png		icons/pause-24.png		\
	icons/refresh-16.png	icons/refresh-24.png	\
	icons/remove-16.png		icons/remove-24.png		\
	icons/resume-16.png		icons/resume-24.png		\
	icons/gamessq.svg		icons/gamessq-16.png

EXTRA_DIST = \
	icons/gamessq.ico		icons/GamessQ.icns


# Copyright (C) 2012 Assaf Gordon <gordon@cshl.edu>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

bin_PROGRAMS = idevicedata_addressbook \
		idevicedata_messages

idevicedata_addressbook_SOURCES = idd_addressbook.cpp \
				  AddressBook/Database.h AddressBook/Database.cpp \
				  AddressBook/Memory.h AddressBook/Memory.cpp \
				  AddressBook/ExportHTML.h AddressBook/ExportHTML.cpp \
				  AddressBook/ExportSummary.h AddressBook/ExportSummary.cpp \
				  AddressBook/AddressBook.h AddressBook/AddressBook.cpp \
				  common/idd_common.cpp common/idd_common.h

idevicedata_messages_SOURCES = idd_messages.cpp \
			       Messages/MessagesChatRecord.h \
			       Messages/MessagesChatRecordLoader.h Messages/MessagesChatRecordLoader.cpp \
			       Messages/MessagesHandleRecord.h \
			       Messages/MessagesHandleRecordLoader.h Messages/MessagesHandleRecordLoader.cpp \
			       Messages/MessagesMessageRecord.h \
			       Messages/MessagesMessageRecordLoader.h Messages/MessagesMessageRecordLoader.cpp \
			       common/idd_common.cpp common/idd_common.h

AUTOMAKE_OPTIONS = subdir-objects
AM_CPPFLAGS = -I$(top_srcdir)/src -I$(top_srcdir)/src/common $(SQLITE3_CFLAGS) $(libcrypto_CFLAGS)
AM_LDFLAGS = $(SQLITE3_LDFLAGS) $(libcrypto_LIBS)

## Add the following if you need a boost
# AM_CPPFLAGS = $(BOOST_CPPFLAGS)
# AM_LDFLAGS = $(BOOST_PROGRAM_OPTIONS_LDFLAGS)
# LIBS = $(BOOST_PROGRAM_OPTIONS_LIBS)



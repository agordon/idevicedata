/*
   Common utility functions for iDD (iDeviceData)
 */
#ifndef __IDD_COMMON_H__
#define __IDD_COMMON_H__

#include <string>
#include <sqlite3.h>

std::string sqlite3_get_text_column(sqlite3_stmt* stmt,int column);

/* Given a domain (e.g. "CameraRollDomain")
   and a file path, calculates the SHA1 hash of the filename. */
std::string sha1_iOS_file(const std::string& domain, const std::string& file);

std::string sha1hex(const std::string& text);

void validate_idevice_backup_directory(const char* dir);

/* Returns NON-ZERO if 'file' is an existing regular file */
int is_valid_regular_file(const char* file);

/* Returns NON-ZERO if 'dir' is a valid directory */
int is_valid_directory(const char* dir);

#endif

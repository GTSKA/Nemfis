#pragma once

#include "_NEMFIS_UTILITY_.h"

#ifndef UTILTY_FILE_MANAGER_H
#define UTILTY_FILE_MANAGER_H

#define UTILTY_FILE_MANAGER_VERSION 0.1

class CFileManager_
{
public:

	CFileManager_(void);
	~CFileManager_(void);

	HRESULT Create(LPWSTR sFile);
	LPWSTR GetFileName();
	LPWSTR GetFilePath();
	LPWSTR GetFullFilePath();
	LPWSTR GetFileExt();
	UINT GetFileSize();

private:

	LPWSTR	m_fileName;
	LPWSTR	m_fileDrive;
	LPWSTR	m_filePath;
	LPWSTR	m_fullFilePath;
	LPWSTR	m_fileExtention;
	UINT	m_fileSize;

};

#endif

//--------------------------------------------------------------------------------------
//Get the current path for the executable file
//--------------------------------------------------------------------------------------
void GetExePath(STRSAFE_LPWSTR strExePath, UINT strExePathSize);

//--------------------------------------------------------------------------------------
//Search a file in realtion with the application module
//--------------------------------------------------------------------------------------
HRESULT FindFile(STRSAFE_LPWSTR strDestPath, int cchDest, LPCWSTR strFilename);

//--------------------------------------------------------------------------------------
//Search a file in a set of typical directories
//--------------------------------------------------------------------------------------
bool FindFileTypicalDirs(WCHAR* strSearchPath, int cchSearch, LPCWSTR strLeaf, WCHAR* strExePath, WCHAR* strExeName);

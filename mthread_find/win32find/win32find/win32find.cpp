// win32find.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>

void _check_invariant_conditions(int argc, char *argv[])
{
	if (argc == 1 || argc > 3)
	{
		fprintf(stderr, "usage: %s [starting point...] [expression]\n", argv[0]);
		exit(-1); // argument length
	}
	if (strlen(argv[1]) > MAX_PATH)
	{
		fprintf(stderr, "argument length: path cannot exceed %d characters\n", MAX_PATH);
		exit(-1);
	}
	if (argc > 3 && strlen(argv[2]) > FILENAME_MAX)
	{
		fprintf(stderr, "argument length: expression cannot exceed %d characters\n", FILENAME_MAX);
		exit(-1);
	}
}

void _search(wchar_t base_path[], wchar_t filename[])
{
	LPWIN32_FIND_DATA hFile = (LPWIN32_FIND_DATA)GlobalAlloc(GMEM_ZEROINIT, sizeof(WIN32_FIND_DATA));
	LPWIN32_FIND_DATA hDirectory = (LPWIN32_FIND_DATA) GlobalAlloc(GMEM_ZEROINIT, sizeof(WIN32_FIND_DATA));
	
	HANDLE findFileResult = NULL;
	HANDLE findDirectoryResult = NULL;

	wchar_t wildcard[MAX_PATH] = L"";

	size_t bp_sz = wcsnlen_s(base_path, MAX_PATH);

	StringCchCat(wildcard, bp_sz + 1, base_path);
	StringCchCat(wildcard, bp_sz + 2, L"*");

	findDirectoryResult = FindFirstFile((LPCWSTR)wildcard, hDirectory);

	if (findDirectoryResult == INVALID_HANDLE_VALUE)
	{
		DWORD dw_error_code = 0;

		dw_error_code = GetLastError();

		if (dw_error_code != ERROR_FILE_NOT_FOUND)
		{
			_tprintf(TEXT("FindFirstFile failed %d\n"), dw_error_code);
		}
	}

	do 
	{
		if (hDirectory->dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY
			&& wcscmp(hDirectory->cFileName, L".") != 0
			&& wcscmp(hDirectory->cFileName, L"..") != 0)
		{
			wchar_t new_bp[MAX_PATH] = L"";

			size_t fn_sz = wcsnlen_s(hDirectory->cFileName, FILENAME_MAX);

			StringCchCat(new_bp, bp_sz + 1, base_path);
			StringCchCat(new_bp, bp_sz + fn_sz + 1, hDirectory->cFileName);
			StringCchCat(new_bp, bp_sz + fn_sz + 2, L"\\");
			
			_search(new_bp, filename);
		}
		
	} while (FindNextFile(findDirectoryResult, hDirectory));

	wchar_t combined_filename[MAX_PATH] = TEXT("");

	StringCchCat(combined_filename, bp_sz + 1, base_path);
	StringCchCat(combined_filename, bp_sz + 2, TEXT("\\"));
	StringCchCat(combined_filename, bp_sz + wcsnlen_s(filename, FILENAME_MAX) + 2, filename);
	
	findFileResult = FindFirstFile((LPCWSTR)combined_filename, hFile);
	
	if (findFileResult == INVALID_HANDLE_VALUE)
	{
		DWORD dw_error_code = 0;

		dw_error_code = GetLastError();

		if (dw_error_code != ERROR_FILE_NOT_FOUND)
		{
			_tprintf(TEXT("FindFirstFile failed %d\n"), dw_error_code);
			return;
		}

		return;
	}

	do 
	{
		wchar_t fpn[MAX_PATH] = L"";
		StringCchCat(fpn, bp_sz + 1, base_path);
		StringCchCat(fpn, bp_sz + 2, L"\\");
		StringCchCat(fpn, bp_sz + wcsnlen_s(hFile->cFileName, FILENAME_MAX) + 2, hFile->cFileName);

		_tprintf(TEXT("%s\n"), fpn);
	} while (FindNextFile(findFileResult, hFile));

	FindClose(findFileResult);
}

int main(int argc, char *argv[])
{
	_check_invariant_conditions(argc, argv);

	wchar_t base_path[MAX_PATH] = L"";
	wchar_t file_name[FILENAME_MAX] = L"";

	if (argc == 2)
	{
		wchar_t fn[FILENAME_MAX];
		mbstowcs_s(NULL, fn, argv[1], FILENAME_MAX);

		StringCchCat(base_path, sizeof(wchar_t) * 3, L".\\");
		StringCchCat(file_name, FILENAME_MAX * sizeof(wchar_t), fn);
	}
	else if (argc == 3)
	{
		wchar_t fn[FILENAME_MAX * sizeof(wchar_t)];
		mbstowcs_s(NULL, fn, argv[2], FILENAME_MAX);

		wchar_t bp[MAX_PATH * sizeof(wchar_t)];
		mbstowcs_s(NULL, bp, argv[1], FILENAME_MAX);

		StringCchCat(base_path, MAX_PATH * sizeof(wchar_t), bp);
		StringCchCat(file_name, FILENAME_MAX * sizeof(wchar_t), fn);
	}

	_search(base_path, file_name);

	return 0;
}
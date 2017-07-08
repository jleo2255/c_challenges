#include <windows.h>
#include <tchar.h>
#include <string.h>

#include "accctrl.h"
#include "aclapi.h"
#include "fileowner.h"

static int _set_user(HANDLE *h_file, char **user);

owner_info_t *get_fileowner(char *filename)
{
	HANDLE h_file = NULL;
    owner_info_t *o_info = (owner_info_t *)GlobalAlloc(GMEM_FIXED, sizeof(owner_info_t));
    o_info->user = NULL;
    o_info->group = NULL;

	h_file = CreateFile(
		TEXT("."),
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS,
		NULL);		

	if (h_file == INVALID_HANDLE_VALUE)
	{
		DWORD dw_error_code = 0;

		dw_error_code = GetLastError();
		_tprintf(TEXT("CreateFile error = %d\n"), dw_error_code);
		return NULL;
	}

    _set_user(h_file, &(o_info->user));

    return o_info;
}

int destroy_fileowner(owner_info_t *o_info)
{
    if (o_info->user != NULL)
    {
        HGLOBAL g_handle = GlobalFree(o_info->user);

        if (g_handle != NULL)
        {
            DWORD dw_error_code = GetLastError();

            _tprintf(TEXT("GlobalFree error = %d\n"), dw_error_code);
            return -1;
        }
        o_info->user = NULL;
    }

    if (o_info->group != NULL)
    {
        HGLOBAL g_handle = GlobalFree(o_info->group);

        if (g_handle != NULL)
        {
            DWORD dw_error_code = GetLastError();

            _tprintf(TEXT("GlobalFree error = %d\n"), dw_error_code);
            return -1;
        }
        o_info->group = NULL;
    }
 
    if (o_info != NULL)
    {
        HGLOBAL g_handle = GlobalFree(o_info);

        if (g_handle != NULL)
        {
            DWORD dw_error_code = GetLastError();

            _tprintf(TEXT("GlobalFree error = %d\n"), dw_error_code);
            return -1;
        }
    }

    return 0;
}

static int _set_user(HANDLE *h_file, char **user)
{
    DWORD dw_rtn_code = 0;
	PSID p_sid_owner = NULL;
	PSECURITY_DESCRIPTOR p_sd = NULL;
    BOOL b_rtn_bool = TRUE;
    LPTSTR account_name = NULL, domain_name = NULL;
    DWORD dw_account_name = 1, dw_domain_name = 1;
    SID_NAME_USE e_use = SidTypeUnknown;

	// get the owner SID of the file
    dw_rtn_code = GetSecurityInfo(
        h_file,
        SE_FILE_OBJECT,
        OWNER_SECURITY_INFORMATION,
        &p_sid_owner,
        NULL, NULL, NULL,
        &p_sd
    );
    
    if (dw_rtn_code != ERROR_SUCCESS)
    {
        DWORD dw_error_code = 0;

        dw_error_code = GetLastError();
        _tprintf(TEXT("GetSecurityInfo error = %d\n"), dw_error_code);
        return -1;
    }

    b_rtn_bool = LookupAccountSid(
        NULL,
        p_sid_owner,
        account_name,
        (LPDWORD) &dw_account_name,
        domain_name,
        (LPDWORD) &dw_domain_name,
        &e_use
    );

    account_name = (LPTSTR) GlobalAlloc(GMEM_FIXED, dw_account_name);

    if (account_name == NULL)
    {
        DWORD dw_error_code = 0;

        dw_error_code = GetLastError();
        _tprintf(TEXT("GlobalAlloc error = %d\n"), dw_error_code);
        return -1;
    }

    domain_name = (LPTSTR) GlobalAlloc(GMEM_FIXED, dw_domain_name);

    if (domain_name == NULL)
    {
        DWORD dw_error_code = 0;

        dw_error_code = GetLastError();
        _tprintf(TEXT("GlobalAlloc error = %d\n"), dw_error_code);
        return -1;
    }

    b_rtn_bool = LookupAccountSid(
        NULL,                       // name of local or remote computer
        p_sid_owner,                // security identifier
        account_name,               // account name buffer
        (LPDWORD) &dw_account_name, // account name buffer size
        domain_name,                // domain name buffer
        (LPDWORD) &dw_domain_name,  // domain name buffer size
        &e_use                      // SID type
    );
    
    if (b_rtn_bool == FALSE)
    {
        DWORD dw_error_code = 0;

        dw_error_code = GetLastError();
        if (dw_error_code == ERROR_NONE_MAPPED)
        {
            _tprintf(TEXT("Account owner not found for specified SID.\n"));
        }
        else 
        {
            _tprintf(TEXT("LookupAccountSid error %d\n"), dw_error_code);
        }

        return -1;
    }
    else
    {
        *user = (char *) GlobalAlloc(GMEM_ZEROINIT, dw_domain_name + 1 + dw_account_name);
        strncpy(*user, domain_name, dw_domain_name);
        strncat(*user, "/", 1); 
        strncat(*user, account_name, dw_account_name);
    }

    return 0;
}

char *_get_group_name(char *filename)
{
    PSID p_sid_group = NULL;
}
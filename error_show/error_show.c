#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main(int argc, char **argv)
{
    char buf[1000];

    fputs("Enter an error code integer: ", stdout);
    fflush(stdout);

    if(fgets(buf, sizeof(buf), stdin) != NULL)
    {
        char *end = NULL;

        errno = 0;

        long temp = strtol(buf, &end, 10);

        if(end != buf && // strtol sets endptr to nptr if no digits exist in char buf
            errno != ERANGE && // error will be set to ERANGE if not a long 
            temp && temp >= INT_MIN && temp <= INT_MAX) // we want int, not long, so check for valid range
        {
            int error_code_int = (int)temp;
            DWORD system_locale = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);
            HLOCAL h_locale = NULL;

            BOOL f_ok = FormatMessage(
                FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS |
                FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL,
                error_code_int, system_locale, (PTSTR) &h_locale,
                0, NULL);

            if(f_ok && (h_locale != NULL))
            {
                printf("%s", (LPCTSTR) h_locale);
            }
        }
    } 
   
    return 0;
}
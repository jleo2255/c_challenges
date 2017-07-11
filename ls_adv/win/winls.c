#include <dirent.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <time.h>
#include <unistd.h>
#include <windows.h>

#include <sys/stat.h>
#include <sys/types.h>

#include "fileowner.h"

static const struct option long_options[] = 
{
	{
		"long", no_argument, 0, 'l'
	},
	{
		"all", no_argument, 0, 'a'
	},
	{
		"directory", no_argument, 0, 'd'
	},
	{
		0, 0, 0, 0
	}
};

typedef struct 
{
	int l;
	int a;
	int d;
} ls_opts;

int _print_ls(ls_opts options);

int _print_permissions(char *file_name);

int main(int argc, char *argv[])
{
	char *optstr = "lad";
	int long_opts_i = 0;
	ls_opts ls_options = {
		.l = -1, .a = -1, .d = -1	
	};
	
	while(1)
	{
		int c = getopt_long(argc, argv, optstr, long_options, &long_opts_i);
		if (c == -1) break;
		
		switch(c)
		{
			case 'l':
				ls_options.l = 1;
				break;
			case 'a':
				ls_options.a = 1;
				break;
			case 'd':
				ls_options.d = 1;
				break;
			default:
				putchar(c);
				fprintf(stderr, "usage: %s [-lad]\n", argv[0]);
				break;
		}
	}

	_print_ls(ls_options);

	return 0;
}

static int _set_permission_info(struct stat file_stat, char perms[])
{
	perms[0] = ((S_ISDIR(file_stat.st_mode)) ? 'd' : '-');
	perms[1] = ((file_stat.st_mode & S_IRUSR) ? 'r' : '-');
	perms[2] = ((file_stat.st_mode & S_IWUSR) ? 'w' : '-');
	perms[3] = ((file_stat.st_mode & S_IXUSR) ? 'x' : '-');
	perms[4] = '\0';

	return 0;
}

int _print_ls(ls_opts options)
{
	DIR *dir_p;  
	struct stat file_stat;
	struct dirent *dirent_p;


	if ((dir_p = opendir(".")) == NULL) 
	{
		perror("opendir");
		exit(-1);
	}

	while ((dirent_p = readdir(dir_p)) != NULL)
	{
		if (options.a != 1 && (dirent_p->d_name)[0] == '.') continue;

		if (options.l > 0 || options.d > 0)
		{
			if (stat(dirent_p->d_name, &file_stat) < 0) return -1;

			if (options.d > 0 && !S_ISDIR(file_stat.st_mode)) continue;
			
			if (options.l > 0)
			{
				owner_info_t *o_info = get_fileowner(dirent_p->d_name);
				char perms[5];
				char *m_time = ctime(&(file_stat.st_mtime));
				m_time[strcspn(m_time, "\n")] = 0;

				_set_permission_info(file_stat, perms);

				_tprintf(TEXT("%s\t  %d\t  %s\t  %s\t  %d\t  %s\t"), 
					perms, (int)file_stat.st_nlink,
					o_info->user, o_info->group,
					(int) file_stat.st_size, m_time);

				destroy_fileowner(o_info);
			}
			
		}
		printf("%s\n", dirent_p->d_name);
	}
	
	closedir(dir_p);

	return 0;
}	

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

int _print_permission_info(struct stat file_stat)
{
	printf((S_ISDIR(file_stat.st_mode)) ? "d" : "-");
	printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
	printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
	printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");

	return 0;
}

int _print_grp_info(struct stat file_stat)
{
	struct group *grp;

	//if ((grp = getgrgid(file_stat.st_gid)) == NULL) return -1;
	//printf("%s", grp->gr_name);	 

	return 0;
}

int _print_user_info(struct stat file_stat)
{
	struct passwd *usr;

	//if ((usr = getpwuid(file_stat.st_uid)) == NULL) return -1;
	//printf("%s", usr->pw_name);	 

	return 0;
}

int _print_modified_time(struct stat file_stat)
{
	char *m_time = ctime(&(file_stat.st_mtime));
	m_time[strcspn(m_time, "\n")] = 0;

	printf("%s", m_time);

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

				_print_permission_info(file_stat);
				printf("\t");
				printf("%d", (int)file_stat.st_nlink);
				printf("\t");
				//_print_user_info(file_stat);
				printf("%s", o_info->user);
				printf("\t");
				_print_grp_info(file_stat);
				printf("\t");
				printf("%d", (int)file_stat.st_size);
				printf("\t");
				_print_modified_time(file_stat);
				printf("\t");

				destroy_fileowner(o_info);
			}
			
		}
		printf("%s\n", dirent_p->d_name);
	}
	
	closedir(dir_p);

	return 0;
}	

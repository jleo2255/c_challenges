#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>

void _check_invariant_conditions(int argc, char *argv[])
{
	if (argc == 1 || argc > 3)
	{
		fprintf(stderr, "usage: %s [starting point...] [expression]\n", argv[0]);
		exit(-1); // argument length
	}
	if (strlen(argv[1]) > PATH_MAX)
	{
		fprintf(stderr, "argument length: path cannot exceed %d characters\n", PATH_MAX);
		exit(-1);
	}
	if (argc > 3 && strlen(argv[2]) > NAME_MAX)
	{
		fprintf(stderr, "argument length: expression cannot exceed %d characters\n", NAME_MAX);
		exit(-1);
	}
}

int _search(char *base_path, char* filename, int (*compare) (const char *key1, const char *key2))
{
	DIR *dir_p;
	struct dirent *dirent_p;
	struct stat file_stat;

	if ((dir_p = opendir(base_path)) == NULL)
	{
		perror("opendir");
		exit(-1);
	}

	char combined_path[PATH_MAX - NAME_MAX] = "";
	strcat(combined_path, base_path);
	strcat(combined_path, "/");

	while ((dirent_p = readdir(dir_p)) != NULL)
	{
		if (strcmp(dirent_p->d_name, ".") == 0) continue;
		if (strcmp(dirent_p->d_name, "..") == 0) continue;

		char combined_filename[PATH_MAX] = "";
		strcat(combined_filename, combined_path);
		strcat(combined_filename, dirent_p->d_name);

		if (lstat(combined_filename, &file_stat) == -1) 
		{
			perror("stat");
		}

		if (compare != NULL && (compare(dirent_p->d_name, filename) == 0))
		{
			printf("%s\n", combined_filename);
		}

		if (S_ISDIR(file_stat.st_mode))	
		{
			_search(combined_filename, filename, strcmp);
		}
	}

	closedir(dir_p);	
}

int main(int argc, char *argv[])
{
	_check_invariant_conditions(argc, argv);

	char base_path[PATH_MAX];
	char file_name[NAME_MAX];

	if (argc == 2)
	{
		base_path[0] = '.';
		base_path[1] = '\0';
		strcpy(file_name, argv[1]);
	}
	else if (argc == 3)
	{
		strcpy(base_path, argv[1]);
		strcpy(file_name, argv[2]);
	}

	_search(base_path, file_name, strcmp);

	return 0;
}



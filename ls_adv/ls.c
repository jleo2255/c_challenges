#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

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

void _print_ls(ls_opts options);

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

void _print_ls(ls_opts options)
{
	DIR *dir_p;  
	struct dirent *dirent_p;

	if ((dir_p = opendir(".")) == NULL) 
	{
		perror("opendir");
		exit(-1);
	}

	while ((dirent_p = readdir(dir_p)) != NULL)
	{
		printf("%s\n", dirent_p->d_name);
	}
	
	closedir(dir_p);
}	

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

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

int main(int argc, char *argv[])
{
	char *optstr = "lad";
	int long_opts_i = 0;

	while(1)
	{
		int c = getopt_long(argc, argv, optstr, long_options, &long_opts_i);
		if (c == -1) break;
		
		switch(c)
		{
			case 'l':
				putchar(c);
				printf("%s\n", long_options[long_opts_i].name);
				break;
			case 'a':
				putchar(c);
				printf("%s\n", long_options[long_opts_i].name);
				break;
			case 'd':
				putchar(c);
				printf("%s\n", long_options[long_opts_i].name);
				break;
			default:
				putchar(c);
				fprintf(stderr, "usage: %s [-lad]\n", argv[0]);
				break;
		}
	}

	return 0;
}


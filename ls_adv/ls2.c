#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	char *optstr;
	if((optstr = (char *)malloc(sizeof(char) * 30)) == NULL) return -1;

	while(getopt(argc, argv, ))
}


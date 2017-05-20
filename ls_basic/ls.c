#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main(int argc, char **argv)
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

	return(0);
}

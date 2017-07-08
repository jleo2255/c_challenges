#ifndef _FILEOWNER_H
#define _FILEOWNER_H

typedef struct owner_info
{
    char *user;
    char *group;
} owner_info_t;

owner_info_t *get_fileowner(char *filename);

int destroy_fileowner(owner_info_t *o_info);

#endif
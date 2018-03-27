#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

char *get_config_value(const char *file_path, const char *key)
{
    FILE *fp;
    char line[256];
    char buf[256];
    char val[256];
    char *cptr;
    char *lptr;

    fp = fopen(file_path, "r");

    if (!fp)
        return NULL;

    while (fgets(line, 256, fp)) {
        memset(buf, 0, 256);
        memset(val, 0, 256);

        for (lptr = line; *lptr == ' '; lptr++)
            ;
        if (*lptr == '#' || *lptr == '\n' || *lptr == '\0')
            continue;

        cptr = strchr(lptr, '=');

        if (cptr) {
            strncpy(buf, lptr, cptr - lptr);
            strncpy(val, cptr + 1, strlen(lptr) - (cptr - lptr + 1));

            cptr = strpbrk(val, "\n# ");
            if (cptr)
                *cptr = 0;

            if (strcmp(buf, key) == 0) {
                break;
            }
        }
    }

    fclose(fp);

    cptr = strdup(val);

    return cptr;
}

pid_t get_pid(const char *file_path)
{
    FILE *fp;
    char line[256];
    int i = 0;
    pid_t pid;

    fp = fopen(file_path, "r");
    if (!fp)
        return -1;

    while (fgets(line, 256, fp)) {
        for (i = 0; i < sizeof(line); i++) {
            if (!isdigit(line[i])) {
                line[i] = '\0';
                break;
            }
        }
    }

    fclose(fp);

    if (i > 0)
        pid = atoi(line);
    else
        pid = -1;

    return pid;
}

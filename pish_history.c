#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "pish.h"

static char pish_history_path[1024] = {'\0'};

/*
 * Set history file path to ~/.pish_history.
 */
static void set_history_path()
{
    const char *home = getpwuid(getuid())->pw_dir;
    strncpy(pish_history_path, home, 1024);
    strcat(pish_history_path, "/.pish_history");
}

void add_history(const struct pish_arg *arg)
{
    // set history path if needed
    if (!(*pish_history_path)) {
        set_history_path();
    }

    /* 
     * - open (and create if needed) history file at pish_history_path.
     * - write out the command stored in `arg`; argv values are separated
     *   by a space.
     */
    FILE *f = fopen(pish_history_path, "a");
    if (!f) {
        perror("Error opening history file");
        return;
    }
    for (int i = 0; i < arg->argc; ++i) {
        if (i == arg->argc - 1) {
            fprintf(f, "%s", arg->argv[i]);
        } else {
            fprintf(f, "%s ", arg->argv[i]);
        }
    }
    fprintf(f, "\n");
    fclose(f);
}

void print_history()
{
    // set history path if needed
    if (!(*pish_history_path)) {
        set_history_path();
    }

    // read history file and print with index */
    FILE *f = fopen(pish_history_path, "r");
    if (!f) {
        perror("Error opening history file");
        return;
    }
    char line[1024];
    int num = 1;
    while (fgets(line, sizeof(line), f)) {
        printf("%d %s", num, line);
        num++;
    }
    fclose(f);
}

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <libgen.h>
#include <string.h>
#include <sys/stat.h>

#include "env.h"

int find_id(char *filename, int id) {
    return find_id(filename, id + 1);
}

void trash(char *path) {
    char *filename = NULL;
    /* Maybe make this the size instead
     * of the length, and remove the +1
     * in the malloc() call?
     */
    size_t new_path_len;
    char *new_path = NULL;
    int err;

    /* Create info file here. It *must*
     * be atomic!
     *
     * Paths in the info file must be
     * written in URI notation.
     */
    
    filename = basename(path);
    new_path_len = strlen(g_trash_files) + strlen(filename);
    new_path = malloc(new_path_len + 1); /* +1 for null termination */

    strcat(new_path, g_trash_files);
    strcat(new_path, filename);

    printf("%s\n", new_path);
    err = rename(path, new_path);
    free(new_path);
    if (err != 0) {
        fprintf(stderr, "Error trashing '%s': %s\n", path, strerror(errno));
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Invalid number of arguments\n");
        exit(EXIT_FAILURE);
    }

    init_trash();

    for (int i = 1; i < argc; i++) {
        trash(argv[i]);
    }

    free(g_trash_files);
    free(g_trash_info);

    exit(EXIT_SUCCESS);
}

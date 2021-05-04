#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "env.h"

int mkpath(char* file_path, mode_t mode) {
    char *p = NULL;
    /* Why is the assert needed? */
    assert(file_path && *file_path);
    for (p = strchr(file_path + 1, '/'); p; p = strchr(p + 1, '/')) {
        *p = '\0';
        if (mkdir(file_path, mode)) {
            if (errno != EEXIST) {
                *p = '/';
                return -1;
            }
        }
        *p = '/';
    }
    return 0;
}

void init_trash(void) {
    char *xdg_data_home = NULL;
    char *trash_files = NULL;
    char *trash_info = NULL;

    xdg_data_home = getenv("XDG_DATA_HOME");
    if (xdg_data_home == NULL) {
        xdg_data_home = XDG_DATA_HOME_DEFAULT;
    }

    /* +1 for null termination */
    trash_files = malloc(strlen(xdg_data_home) + strlen(TRASH_FILES) + 1);
    /* Believe it or not, simply
     * concatenating the user's home
     * directory with the trash folder
     * location doesn't cause any
     * problems.
     *
     * `/foo//bar`
     * is the same as
     * `/foo/bar`
     */
    strcpy(trash_files, xdg_data_home);
    strcat(trash_files, TRASH_FILES);

    trash_info = malloc(strlen(xdg_data_home) + strlen(TRASH_INFO) + 1);
    strcpy(trash_info, xdg_data_home);
    strcat(trash_info, TRASH_INFO);

    /* "The access rights, access time,
     * modification time and extended
     * attributes (if any) for a
     * file/directory in $trash/files
     * SHOULD be the same as the
     * file/directory had before getting
     * trashed."
     *
     * The code below does not adhere to
     * the standard.
     */
    if (mkdir(trash_files, 0600) || mkdir(trash_info, 0600)) {
        fprintf(stderr, "Could not create trash folders: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    free(trash_files);
    free(trash_info);
}

/* can: move files to the trashcan.
 * Copyright (C) 2021  Sawyer Shepherd
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */ 

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#include "env.h"

int mkpath(char* file_path, __mode_t mode) {
    char *p = NULL;
    /* This assert is needed to insure that both the pointer and the string
    pointed to are not NULL */
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

struct TrashPaths *init_trash(void) {
    char *xdg_data_home = NULL;
    char *trash_files = NULL;
    char *trash_info = NULL;
    struct TrashPaths *trash_paths = NULL;

    xdg_data_home = getenv("XDG_DATA_HOME");
    if (xdg_data_home == NULL) {
        size_t xdg_data_home_len;
        char *home_dir = getenv("HOME");

        xdg_data_home_len = strlen(home_dir) + strlen(XDG_DATA_HOME_DEFAULT);
        /* +1 for null termination */
        xdg_data_home = malloc(xdg_data_home_len + 1);
        strcat(xdg_data_home, XDG_DATA_HOME_DEFAULT);
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
    if (mkpath(trash_files, 0770) || mkpath(trash_info, 0770)) {
        fprintf(stderr, "Could not create trash folders: %s\n", strerror(errno));
	free(trash_files);
	free(trash_info);
        exit(EXIT_FAILURE);
    }

    trash_paths->files = trash_files;
    trash_paths->info = trash_info;
    return trash_paths;
}

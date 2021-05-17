/* can: move files to the trashcan.
 * Copyright (C) 2021  Sawyer Shepherd 
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
#include <stddef.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "trash.h"
#include "env.h"

size_t intlen(int n) {
    size_t digits;
    for (digits = 0; n / 10 > 0; digits++, n /= 10);
    return digits;
}

/* Find a way to make this more compact */
char *reserve_trash_name(char *filename){
    int count;
    char *trash_info_name = NULL;
    int duplicate = 0;
    int err;

    trash_info_name = filename;
    /* If the info file does not already exist and is not busy, create the file
    atomically. */
    for (count = 1, err = open(trash_info_name, O_CREAT|O_EXCL); !err && errno == EEXIST; count++) {
        int trash_info_name_len;
        count++;
        if (!duplicate) {
	    duplicate = 1;
	    /* TRASH_DUPLICATE_SEPARATOR is a period, and should probably stay
	    that way, but you can change it if you want. */
	    trash_info_name_len = strlen(trash_info_name) + strlen(TRASH_DUPLICATE_SEPARATOR);
	    /* +1 for null termination */
	    realloc(trash_info_name, trash_info_name_len + 1);
	    strcat(trash_info_name, TRASH_DUPLICATE_SEPARATOR);
        } else {
	    char *count_string = NULL;
	    /* TODO: Find the strlen of an integer that changes */
	    /* Use sprintf */
	    
	    trash_info_name_len = strlen(trash_info_name) + dyn_strlen();
	    /* +1 for null termination */
	    realloc(trash_info_name, trash_info_name_len + 1);
	    strcat(trash_info_name, ((char*) count));
	}

    }
    return trash_info_name;
}


/* Perhaps this should return some indication of successful completion, such as
 * -1/0 or an array of which files were successfully trashed.
 */
void trash(char *path, struct TrashPaths *trash_paths) {
    char *filename = NULL;
    /* Maybe make this the size instead
     * of the length, and remove the +1
     * in the malloc() call?
     */
    size_t new_path_len;
    char *new_path = NULL;
    int err;

    /* Paths in the info file must be
     * written in URI notation.
     */
    
    filename = basename(path);
    new_path_len = strlen(trash_paths->files) + strlen(filename);
    new_path = malloc(new_path_len + 1); /* +1 for null termination */

    strcat(new_path, trash_paths->info);
    strcat(new_path, filename);

    printf("%s\n", new_path);
    err = rename(path, new_path);
    free(new_path);
    if (err != 0) {
        fprintf(stderr, "Error trashing '%s': %s\n", path, strerror(errno));
    }
}

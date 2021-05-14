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

#include <env.h>

void trash(char *path, struct TrashPaths *trash_paths) {
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

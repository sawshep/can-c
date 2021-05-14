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
#include <libgen.h>
#include <sys/stat.h>
#include <unistd.h>

#include "arg.h"
#incldue "env.h"
#include "trash.h"

int find_id(char *filename, int id) {
    return find_id(filename, id + 1);
}

int main(int argc, char *argv[]) {
    struct ArgInfo *arg_info;
    struct TrashInfo *trash_info;

    arg_info = parse_args(argc, argv);
    if (arg_info == NULL) {
      exit(EXIT_FAILURE);
    }

    trash_info = init_trash();
    if (trash_info == NULL) {
      exit(EXIT_FAILURE);
    }

    {
        int i;
	for (i = 0; i < sizeof()/sizeof(arg_info.files[0]), i++) {
	    trash(arg_info.files[i], trash_info);
	}
    }

    free(trash_info);
    exit(EXIT_SUCCESS);
}

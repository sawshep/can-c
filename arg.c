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

#include <stddef.h>

#include "arg.h"

struct ArgInfo *parse_args(int argc, char *argv[]) {
    int c;
    enum Mode mode;
    struct ArgInfo *arg_info = NULL;

    while ((c = getopt(argc, argv, "fuv")) != -1) {
      switch (c) {
      case 'f':
	    mode = INFO_MODE;
	break;
      case 'u':
	    mode = UNTRASH_MODE;
	break;
      case '?':
	return NULL;
      default:
	    mode = INFO_MODE;
      }
    }
      arg_info->mode = mode;
    
    return arg_info;
}

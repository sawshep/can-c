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

#ifndef CAN_ARG_H
enum Mode { TRASH_MODE, INFO_MODE, UNTRASH_MODE };

enum Mode DEFAULT_MODE = TRASH_MODE;

struct ArgInfo {
    enum Mode mode;
    char *files[];
    /*bool verbose;*/
};

struct ArgInfo *parse_args(int argc, char *argv[]);
#define CAN_ARG_H
#endif

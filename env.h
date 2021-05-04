#ifndef ENV_H
#define ENV_H

const char* XDG_DATA_HOME_DEFAULT = "~/.local/share";

#define TRASH_FILES             "/Trash/files/"
#define TRASH_INFO              "/Trash/info/"

extern void init_trash(void);

#endif

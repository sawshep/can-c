#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <libgen.h>
#include <string.h>

#define TRASH_FILES "/.local/share/Trash/files/"
#define TRASH_INFO "/.local/share/Trash/info/"

/* I don't think it's _too_ bad to have
 * these as globals. Ideally, tilde
 * expansion would just work in paths,
 * but alas, it doesn't, so I can't just
 * have a constant starting with a
 * tilde.
 *
 * There is the `wordexp()` function to
 * do just that, but that doesn't solve
 * the problem of globals.
 */
static char *g_home_dir;
static char *g_trash_files;
static char *g_trash_info;

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

void init_dirs() {
    int err;

    if (g_home_dir == NULL) {
        fprintf(stderr, "Could not find home directory\n");
        exit(EXIT_FAILURE);
    }

    /* These will _usually_ set errno to
     * the same the same thing, but I
     * might want to improve this just
     * in case file permission on the
     * trash directories differ and
     * cause errors.
     */
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
    mkdir(g_trash_files, 0600);
    mkdir(g_trash_info, 0600);

    switch (errno) {
        case EEXIST:
            break;
        case 0:
            /* The trash directories
             * should be created without
             * any warnings or delay.
             *
             * Perhaps I can include
             * this line in a verbose
             * mode.
             */
            /*fprintf(stderr, "Creating trash folders\n");*/
            break;
        default:
            fprintf(stderr, "Failed to initialize trash directories: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
    }
}

void init_globals() {
    size_t g_trash_files_size;
    size_t g_trash_info_size;

    g_home_dir = getenv("HOME");

    /* +1 for null termination */
    g_trash_files_size = strlen(g_home_dir) + strlen(TRASH_FILES) + 1;
    g_trash_files = malloc(g_trash_files_size);
    if (g_trash_files == NULL) {
        fprintf(stderr, "%s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
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
    strcpy(g_trash_files, g_home_dir);
    strcat(g_trash_files, TRASH_FILES);

    /* Same as above, but for a
     * different directory.
     */
    g_trash_info_size = strlen(g_home_dir) + strlen(TRASH_FILES) + 1;
    g_trash_info = malloc(g_trash_info_size);
    if (g_trash_info == NULL) {
        fprintf(stderr, "%s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    strcpy(g_trash_info, g_home_dir);
    strcat(g_trash_info, TRASH_INFO);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Invalid number of arguments\n");
        exit(EXIT_FAILURE);
    }

    init_globals();
    init_dirs();

    for (int i = 1; i < argc; i++) {
        trash(argv[i]);
    }

    free(g_trash_files);
    free(g_trash_info);

    exit(EXIT_SUCCESS);
}

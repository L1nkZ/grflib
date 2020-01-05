#define __STDC_WANT_LIB_EXT2__ 1 /* for strdup */

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <grf/grf.h>
#include <grf/grfsupport.h>

#define MAX_PATH_LEN 260

/*
 * Recursively `mkdir(path, mode)`
 * Expects POSIX paths
 */
static int mkdirp(const char *file_path, mode_t mode) {
    if (NULL == file_path) {
        return -1;
    }

    char *const parent = strdup(file_path);
    if (parent == NULL) {
        goto fail;
    }

    char *p = parent + strlen(parent);
    while ('/' != *p && p != parent) {
        p--;
    }
    *p = '\0';

    // Make parent dir
    if (p != parent && 0 != mkdirp(parent, mode)) {
        goto fail;
    }
    free(parent);

// Make this one if parent has been made
#ifdef _WIN32
    // http://msdn.microsoft.com/en-us/library/2fkk4dzw.aspx
    int rc = mkdir(file_path);
#else
    int rc = mkdir(file_path, mode);
#endif

    return 0 == rc || EEXIST == errno ? 0 : -1;

fail:
    free(parent);
    return -1;
}

static int create_parent_directories(const char *file_path) {
    char *const parent_directory = strdup(file_path);
    if (parent_directory == NULL) {
        return 0;
    }

    char *sep_pos = strrchr(parent_directory, '/');
    if (sep_pos != NULL) {
        *sep_pos = '\0';
        if (mkdirp(parent_directory, 0755) < 0) {
            free(parent_directory);
            return 0;
        }
    }
    free(parent_directory);

    return 1;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s GRF_FILE_PATH OUTPUT_DIRECTORY\n", argv[0]);
        return 1;
    }

    GrfError err = {0};
    int ret;

    Grf *p_grf = grf_open(argv[1], "rb", &err);
    if (p_grf == NULL) {
        printf("Cannot open GRF file: %s\n", grflib_strerror(err));
        ret = 1;
        goto cleanup;
    }

    char output_file_path[MAX_PATH_LEN] = {0};
    for (uint32_t i = 0; i < p_grf->nfiles; i++) {
        // Append the relative part of the path to the given output directory
        snprintf(output_file_path, sizeof(output_file_path) - 1, "%s/%s",
                 argv[2], p_grf->files[i].name);

        // Normalize path and create missing directories
        grflib_normalize_path(output_file_path, output_file_path);
        if (create_parent_directories(output_file_path) == 0) {
            perror("Cannot create directory");
            ret = 1;
            goto cleanup;
        }

        // Finally, extract the file
        if (grf_index_extract(p_grf, i, output_file_path, &err) == 0) {
            printf("Cannot write file to disk: %s\n", grflib_strerror(err));
            ret = 1;
            goto cleanup;
        }
    }

    ret = 0;

cleanup:
    if (p_grf != NULL) {
        grf_close(p_grf);
    }

    return ret;
}

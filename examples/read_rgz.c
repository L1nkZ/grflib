#include <stdio.h>

#include <grf/rgz.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s RGZ_FILE_PATH\n", argv[0]);
        return 1;
    }

    RgzError err = {0};
    int ret;

    Rgz *p_rgz = rgz_open(argv[1], &err);
    if (p_rgz == NULL) {
        printf("Cannot open RGZ file: %s\n", grflib_strerror(err));
        ret = 1;
        goto cleanup;
    }

    printf("RGZ '%s':\n", p_rgz->filename);
    printf("Number of files: %u\n", p_rgz->nfiles);

    ret = 0;

cleanup:
    if (p_rgz != NULL) {
        rgz_close(p_rgz);
    }

    return ret;
}

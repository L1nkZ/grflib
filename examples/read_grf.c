#include <stdio.h>

#include <grf/grf.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s GRF_FILE_PATH\n", argv[0]);
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

    printf("GRF '%s':\n", p_grf->filename);
    printf("Version: %X\n", p_grf->version);
    printf("Number of files: %u\n", p_grf->nfiles);

    ret = 0;

cleanup:
    if (p_grf != NULL) {
        grf_close(p_grf);
    }

    return ret;
}

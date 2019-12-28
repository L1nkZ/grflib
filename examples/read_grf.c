#include <stdio.h>

#include <grf/grf.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s GRF_FILE_PATH\n", argv[0]);
        return 1;
    }

    GrfError err = {};
    int ret;

    Grf* p_grf = grf_callback_open(argv[1], "r", &err, NULL);
    if (p_grf == NULL) {
        printf("Cannot open GRF file. Error type: %d\n", err.type);
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
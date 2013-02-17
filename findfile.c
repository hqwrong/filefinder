#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>

struct fname_ref {
    struct fname_ref * next;
    char fname[128];
};

static int
_ftw_cb(const char *fpath, const struct stat *sb, int typeflag) {
    if (typeflag == FTW_F || typeflag == FTW_D) {
        struct fname_ref * ref = malloc(struct fname_ref);
        memcpy(ref->fname, fpath, sizeof(fpath) + 1);
        head.ne
        return 0;
    }
    return 1;
}

const char ** findfile(const char * pathname) {
    ftw(pathname, _ftw_cb, 128);
}

int
main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("usage: ff filename\n");
        return 1;
    }

    head = malloc(struct fname_ref);
    head->next = NULL;
    
    findfile(argv[1]);
    return 0;
}

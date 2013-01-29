#include <stdlib.h>
#include <stdio.h>
#include <ftw.h>
#include <assert.h>
#include <string.h>

struct fname {
    char name[128];
};

struct fname_pool {
    struct fname * fref;
    int cnt;
    int cap;
};

struct fname_pool * pool_ref;

static int
ftw_callback(const char* fpath, const struct stat *sb, int typeflag) {
    if (typeflag == FTW_F || typeflag == FTW_D) {
        if (pool_ref->cnt == pool_ref->cap) {
            pool_ref->cap *= 2;
            pool_ref->fref = realloc(pool_ref->fref, sizeof(struct fname) * pool_ref->cap);
        }
        struct fname * ref = &(pool_ref->fref[pool_ref->cnt]);
        memcpy(ref->name, fpath, strlen(fpath) + 1);
        pool_ref->cnt++;
        return 0;
    }
    return 1;
}

static void
findfile(const char * pathname) {
    ftw(pathname, ftw_callback, 128);
}

static int
match(const char* name, const char * pattern) {
    int nsz = strlen(name);
    int psz = strlen(pattern);
    int i;
    
    if (nsz < psz) {
        return 0;
    }

    for(i=0; i<=nsz-psz; i++) {
        if (strncmp(pattern, name + i, psz) == 0) {
            return 1;
        }
    }
    return 0;
}

static int
filter(const struct fname_pool * pool, const char * pattern, char * result[], int sz) {
    int i = 0;
    int j = 0;
    for (;i<pool->cnt;i++) {
        if (match(pool->fref[i].name, pattern)) {
            result[j] = pool->fref[i].name;
            j++;
            if (sz == j) {
                break;
            }
        }
    }
    return j;
}

int main(int argc, char * argv[]) {
    int i;
    
    if (argc < 3) {
        printf("usage %s filename pattern", argv[0]);
        return 1;
    }

    pool_ref = malloc(sizeof(struct fname_pool));
    pool_ref->fref = malloc(sizeof(struct fname) * 1024);
    pool_ref->cap = 1024;
    pool_ref->cnt = 0;
    findfile(argv[1]);

    char * result[6];
    int cnt = filter(pool_ref, argv[2], result, sizeof(result) / sizeof(char *));

    printf("cnt %d\n", cnt);
    for (i=0;i<cnt;++i) {
        printf("%s\n",result[i]);
    }
}

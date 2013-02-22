#include <stdlib.h>
#include <stdio.h>
#include <ftw.h>
#include <assert.h>
#include <string.h>
#include <gtk/gtk.h>

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
has_substr(const char * str, const char* substr) {
    int sz = strlen(str);
    int subsz = strlen(substr);
    if (sz < subsz)
        return 0;
    
    for(int i=0; i<=sz-subsz; i++) {
        if (strncmp(substr, str + i, subsz) == 0) {
            return 1;
        }
    }

    return 0;
}

static int
match(const char* name, const char * pattern) {
    int sz = strlen(pattern);
    char subpattern[sz+1][64];
    char tmp[sz+1];
    strncpy(tmp, pattern, sz+1);
    char * substr = strtok(tmp, " ");

    int n;
    for (n=0; n< 64; n++) {
        if (substr == NULL) break;
        strncpy(subpattern[n], substr, sz+1);
        substr = strtok(NULL, " ");
    }
    
    for (int i=0; i<n; i++) {
        if (!has_substr(name, subpattern[i]))
            return 0;
    }
    return 1;
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

static int
_update_cb(const char * content, char ** result) {
    return filter(pool_ref, content, result, 6);
}

static void
_commit_cb(const char * content) {
    printf(content);
    bar_main_quit();
}

int main(int argc, char * argv[]) {
    char * path;
    
    gtk_init(&argc, &argv);

    path = argc==1 ? "." : argv[1];

    pool_ref = malloc(sizeof(struct fname_pool));
    pool_ref->fref = malloc(sizeof(struct fname) * 1024);
    pool_ref->cap = 1024;
    pool_ref->cnt = 0;
    findfile(path);

    bar_set_update_cb(_update_cb);
    bar_set_commit_cb(_commit_cb);
    bar_main();
}

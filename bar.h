#ifndef _BAR_H
#define _BAR_H 1

typedef int (*UPDATE_CB)(const char * content, char ** item) ;
typedef void (*COMMIT_CB)(const char * content) ;

void bar_main_quit();
void bar_main();
void bar_set_commit_cb(COMMIT_CB cb);
void bar_set_update_cb(UPDATE_CB cb);

#endif


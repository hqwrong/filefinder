#include <stdio.h>
/* #include "match.h" */
    
static int
_match(const char* str, const char * pattern) {
    if (*pattern == '\0')
        return 1;
    
    if (*str == '\0')
        return 0;

    switch (*pattern) {
    case '*' :
        return _match(str+1, pattern+1) || _match(str+1, pattern);
    case '?':
        return _match(str+1, pattern+1);
    default :
        return *str == *pattern && _match(str+1, pattern+1);
    }
}

char *
wildcard_match(const char* content, const char * pattern) {
    for (const char * substr = content; *substr != '\0'; substr++) {
        if (_match(substr, pattern))
            return substr;
    }
    return NULL;
}


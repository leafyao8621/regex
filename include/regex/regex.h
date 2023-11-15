#ifndef REGEX_REGEX_H_
#define REGEX_REGEX_H_

#include <regex/errcode.h>

#include <containers/darray.h>

typedef enum RegexTokenType {
    REGEX_TOKEN_TYPE_SINGLE,
    REGEX_TOKEN_TYPE_GROUP
} RegexTokenType;

typedef struct RegexToken {
    RegexTokenType type;
    union {
        struct {
            char acceptable_characters[32];
            size_t run_length;
        } single;
        size_t group;
    } data;
} RegexToken;

DEF_DARRAY(RegexToken)
DEF_DARRAY(DArrayRegexToken)
typedef DArrayDArrayRegexToken RegexGroup;

DEF_DARRAY(RegexGroup)

typedef struct Regex {
    DArrayRegexGroup groups;
} Regex;

typedef struct RegexMatch {
    size_t start;
    size_t end;
} RegexMatch;

RegexErrcode regex_initialize(Regex *regex);
RegexErrcode regex_finalize(Regex *regex);
RegexErrcode regex_compile(Regex *regex, char *pattern);

#endif

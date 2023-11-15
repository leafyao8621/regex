#ifndef REGEX_REGEX_H_
#define REGEX_REGEX_H_

#include <stdio.h>

#include <regex/errcode.h>

#include <containers/darray.h>

#define REGEX_TOKEN_RUN_LENGTH_LAZY 0x8000000000000000
#define REGEX_TOKEN_RUN_LENGTH_INFTY 0x7fffffffffffffff

typedef enum RegexTokenType {
    REGEX_TOKEN_TYPE_SINGLE,
    REGEX_TOKEN_TYPE_GROUP
} RegexTokenType;

typedef struct RegexToken {
    RegexTokenType type;
    union {
        struct {
            char acceptable_characters[32];
            struct {
                size_t lb;
                size_t ub;
            } run_length;
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
RegexErrcode regex_log(Regex *regex, FILE *fout);

#endif

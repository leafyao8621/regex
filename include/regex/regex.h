#ifndef REGEX_REGEX_H_
#define REGEX_REGEX_H_

#include <regex/errcode.h>

#include <containers/darray.h>
#include <containers/dstring.h>

typedef enum RegexTokenType {
    REGEX_TOKEN_TYPE_SINGLE,
    REGEX_TOKEN_TYPE_MULTIPLE,
    REGEX_TOKEN_TYPE_WILDCARD,
    REGEX_TOKEN_TYPE_GROUP
} RegexTokenType;

typedef struct RegexToken {
    RegexTokenType type;
    union {
        char single;
        String multiple;
    } acceptable_characters;
    size_t run_length;
} RegexToken;

DEF_DARRAY(RegexToken)
typedef DArrayRegexToken RegexGroup;

DEF_DARRAY(RegexGroup)

typedef struct Regex {
    DArrayRegexGroup groups;
} Regex;

RegexErrcode regex_initialize(Regex *regex);
RegexErrcode regex_finalize(Regex *regex);

#endif

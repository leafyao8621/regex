#include <regex/regex.h>

#define REGEX_TOKEN_RUN_LENGTH_ONE_OR_MORE 0x8000000000000000
#define REGEX_TOKEN_RUN_LENGTH_ZERO_OR_MORE 0x4000000000000000
#define REGEX_TOKEN_RUN_LENGTH_GREEDY 0x2000000000000000
#define REGEX_TOKEN_RUN_LENGTH_LENGTH 0x1000000000000000

RegexErrcode regex_compile(Regex *regex, char *pattern) {
    if (!regex || !pattern) {
        return REGEX_ERR_NULL_PTR;
    }
    char *iter = pattern;
    for (size_t i = 0; *iter; ++i, ++iter) {

    }
    return REGEX_ERR_OK;
}

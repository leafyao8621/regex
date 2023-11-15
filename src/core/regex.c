#include <regex/regex.h>

RegexErrcode regex_initialize(Regex *regex) {
    if (!regex) {
        return REGEX_ERR_NULL_PTR;
    }
    int ret = DArrayRegexGroup_initialize(&regex->groups, 1);
    if (ret) {
        return REGEX_ERR_OUT_OF_MEMORY;
    }
    return REGEX_ERR_OK;
}

RegexErrcode regex_finalize(Regex *regex) {
    DArrayRegexGroup_finalize(&regex->groups);
    return REGEX_ERR_OK;
}

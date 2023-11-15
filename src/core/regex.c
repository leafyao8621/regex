#include <regex/regex.h>

RegexErrcode regex_initialize(Regex *regex) {
    if (!regex) {
        return REGEX_ERR_NULL_PTR;
    }
    int ret = DArrayRegexGroup_initialize(&regex->groups, 1);
    if (ret) {
        return REGEX_ERR_OUT_OF_MEMORY;
    }
    RegexGroup group;
    ret = DArrayDArrayRegexToken_initialize(&group, 1);
    if (ret) {
        return REGEX_ERR_OUT_OF_MEMORY;
    }

    DArrayRegexToken alt;
    ret = DArrayRegexToken_initialize(&alt, 1);
    if (ret) {
        return REGEX_ERR_OUT_OF_MEMORY;
    }
    DArrayDArrayRegexToken_push_back(&group, &alt);
    DArrayRegexGroup_push_back(&regex->groups, &group);
    return REGEX_ERR_OK;
}

RegexErrcode regex_finalize(Regex *regex) {
    RegexGroup *iter_group = regex->groups.data;
    for (size_t i = 0; i < regex->groups.size; ++i, ++iter_group) {
        DArrayRegexToken *iter_alt = iter_group->data;
        for (size_t j = 0; j < iter_group->size; ++j, ++iter_alt) {
            DArrayRegexToken_finalize(iter_alt);
        }
        DArrayDArrayRegexToken_finalize(iter_group);
    }
    DArrayRegexGroup_finalize(&regex->groups);
    return REGEX_ERR_OK;
}

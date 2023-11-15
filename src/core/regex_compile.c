#include <string.h>

#include <regex/regex.h>

RegexErrcode regex_compile(Regex *regex, char *pattern) {
    if (!regex || !pattern) {
        return REGEX_ERR_NULL_PTR;
    }
    size_t group_idx = 0;
    char *iter = pattern;
    for (size_t i = 0; *iter; ++i, ++iter) {
        RegexToken cur;
        int ret = 0;
        switch (*iter) {
        case '[':
            break;
        case '(':
            break;
        case '{':
            break;
        case '+':
            break;
        case '*':
            break;
        case '\\':
            break;
        case '.':
            cur.type = REGEX_TOKEN_TYPE_SINGLE;
            memset(cur.data.single.acceptable_characters, 0xff, 32);
            cur.data.single.run_length.lb = 1;
            cur.data.single.run_length.ub = 1;
            ret =
                DArrayRegexToken_push_back(
                    &regex
                        ->groups
                        .data[group_idx]
                        .data[
                            regex
                                ->groups
                                .data[group_idx]
                                .size - 1
                        ], &cur);
            if (ret) {
                return REGEX_ERR_OUT_OF_MEMORY;
            }
            break;
        default:
            cur.type = REGEX_TOKEN_TYPE_SINGLE;
            memset(cur.data.single.acceptable_characters, 0, 32);
            cur.data.single.acceptable_characters[*iter >> 3] |=
                1 << (*iter & 7);
            cur.data.single.run_length.lb = 1;
            cur.data.single.run_length.ub = 1;
            ret =
                DArrayRegexToken_push_back(
                    &regex
                        ->groups
                        .data[group_idx]
                        .data[
                            regex
                                ->groups
                                .data[group_idx]
                                .size - 1
                        ], &cur);
            if (ret) {
                return REGEX_ERR_OUT_OF_MEMORY;
            }
            break;
        }
    }
    return REGEX_ERR_OK;
}

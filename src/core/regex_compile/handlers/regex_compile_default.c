#include "../regex_compile.h"

RegexErrcode regex_compile_handle_default(
    Regex *regex,
    short *state,
    size_t *group_idx,
    RegexToken *cur,
    char **iter,
    char *last_char) {
    int ret = 0;
    if ((*state & STATE_MULTIPLE) && (*state & STATE_ESCAPE)) {
        return REGEX_ERR_INVALID_ESCAPE_SEQUENCE;
    }
    if (!(*state & STATE_MULTIPLE)) {
        cur->type = REGEX_TOKEN_TYPE_SINGLE;
        memset(cur->data.single.acceptable_characters, 0, 32);
        cur->data.single.acceptable_characters[**iter >> 3] |=
            1 << (**iter & 7);
        cur->data.single.run_length.lb = 1;
        cur->data.single.run_length.ub = 1;
        ret =
            DArrayRegexToken_push_back(
                &regex
                    ->groups
                    .data[*group_idx]
                    .data[
                        regex
                            ->groups
                            .data[*group_idx]
                            .size - 1
                    ], cur);
        if (ret) {
            return REGEX_ERR_OUT_OF_MEMORY;
        }
        *state |= STATE_QUANTIFIABLE;
        *state &= ~STATE_QUANTIFIER;
    } else {
        regex
            ->groups
            .data[*group_idx]
            .data[
                regex
                    ->groups
                    .data[*group_idx]
                    .size - 1
            ].data[
                regex
                    ->groups
                    .data[*group_idx]
                    .data[
                        regex
                            ->groups
                            .data[*group_idx]
                            .size - 1
                    ].size - 1
            ].data.single.acceptable_characters[**iter >> 3] |=
                1 << (**iter & 7);
        *last_char = **iter;
        *state &= ~STATE_MULTIPLE_START;
    }
    return REGEX_ERR_OK;
}

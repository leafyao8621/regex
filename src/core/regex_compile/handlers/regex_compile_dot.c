#include "../regex_compile.h"

RegexErrcode regex_compile_handle_dot(
    Regex *regex,
    short *state,
    size_t *group_idx,
    RegexToken *cur,
    char *last_char) {
    int ret = 0;
    if (!(*state & STATE_MULTIPLE)) {
        cur->type = REGEX_TOKEN_TYPE_SINGLE;
        if (*state & STATE_ESCAPE) {
            memset(cur->data.single.acceptable_characters, 0, 32);
            cur->data.single.acceptable_characters['.' >> 3] |=
                1 << ('.' & 7);
            *state &= ~STATE_ESCAPE;
        } else {
            memset(cur->data.single.acceptable_characters, 0xff, 32);
        }
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
                    ], cur
            );
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
            ].data.single.acceptable_characters['.' >> 3] |=
                1 << ('.' & 7);
        *last_char = '.';
        *state &= ~STATE_MULTIPLE_START;
    }
    return REGEX_ERR_OK;
}

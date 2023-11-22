#include "../regex_compile.h"

RegexErrcode regex_compile_handle_question(
    Regex *regex, char *state, size_t *group_idx, RegexToken *cur) {
    int ret = 0;
    if (*state & STATE_ESCAPE) {
        cur->type = REGEX_TOKEN_TYPE_SINGLE;
        memset(cur->data.single.acceptable_characters, 0, 32);
        cur->data.single.acceptable_characters['?' >> 3] |=
            1 << ('?' & 7);
        *state &= ~STATE_ESCAPE;
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
    } else if (*state & STATE_QUANTIFIER) {
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
            ].data.single.run_length.ub |= REGEX_TOKEN_RUN_LENGTH_LAZY;
    } else if (*state & STATE_MULTIPLE) {
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
            ].data.single.acceptable_characters['?' >> 3] |=
                1 << ('?' & 7);
    } else {
        if (*state & STATE_QUANTIFIER) {
            return REGEX_ERR_DUPLICATE_QUANTIFIER;
        }
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
            ].data.single.run_length.lb = 0;
        if (ret) {
            return REGEX_ERR_OUT_OF_MEMORY;
        }
        *state ^= STATE_QUANTIFIABLE | STATE_QUANTIFIER;
    }
    return REGEX_ERR_OK;
}

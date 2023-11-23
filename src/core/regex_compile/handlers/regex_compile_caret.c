#include "../regex_compile.h"

RegexErrcode regex_compile_handle_caret(
    Regex *regex,
    short *state,
    size_t *group_idx,
    RegexToken *cur,
    char *last_char) {
    int ret = 0;
    if (*state & STATE_ESCAPE) {
        if (!(*state & STATE_MULTIPLE_START)) {
            return REGEX_ERR_INVALID_ESCAPE_SEQUENCE;
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
            ].data.single.acceptable_characters['^' >> 3] |=
                1 << ('^' & 7);
        *state ^= STATE_ESCAPE;
    } else if (*state & STATE_MULTIPLE) {
        if (*state & STATE_MULTIPLE_START) {
            memset(
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
                    ].data.single.acceptable_characters,
                0xff,
                32
            );
            *state |= STATE_MULTIPLE_COMPLIMENT;
        } else {
            if (!(*state & STATE_MULTIPLE_COMPLIMENT)) {
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
                    ].data.single.acceptable_characters['^' >> 3] |=
                        1 << ('^' & 7);
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
                    ].data.single.acceptable_characters['^' >> 3] &=
                        ~(1 << ('^' & 7));
            }
            *state &= ~STATE_MULTIPLE_START;
        }
        *last_char = '^';
    } else {
        cur->type = REGEX_TOKEN_TYPE_SINGLE;
        memset(cur->data.single.acceptable_characters, 0, 32);
        cur->data.single.acceptable_characters['^' >> 3] |=
            1 << ('^' & 7);
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
    }
    return REGEX_ERR_OK;
}

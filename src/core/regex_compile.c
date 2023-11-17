#include <string.h>

#include <regex/regex.h>

#define STATE_QUANTIFIABLE 0x1
#define STATE_QUANTIFIER 0x2
#define STATE_ESCAPE 0x4
#define STATE_MULTIPLE 0x8

RegexErrcode regex_compile(Regex *regex, char *pattern) {
    if (!regex || !pattern) {
        return REGEX_ERR_NULL_PTR;
    }
    size_t group_idx = 0;
    char *iter = pattern;
    char state = 0;
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
            if (state & STATE_ESCAPE) {
                cur.type = REGEX_TOKEN_TYPE_SINGLE;
                memset(cur.data.single.acceptable_characters, 0, 32);
                cur.data.single.acceptable_characters['+' >> 3] |=
                    1 << ('+' & 7);
                state &= ~STATE_ESCAPE;
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
                            ], &cur
                    );
                if (ret) {
                    return REGEX_ERR_OUT_OF_MEMORY;
                }
                state |= STATE_QUANTIFIABLE;
                state &= ~STATE_QUANTIFIER;
            } else {
                if (state & STATE_QUANTIFIER) {
                    return REGEX_ERR_DUPLICATE_QUANTIFIER;
                }
                regex
                    ->groups
                    .data[group_idx]
                    .data[
                        regex
                            ->groups
                            .data[group_idx]
                            .size - 1
                    ].data[
                        regex
                            ->groups
                            .data[group_idx]
                            .data[
                                regex
                                    ->groups
                                    .data[group_idx]
                                    .size - 1
                            ].size - 1
                    ].data.single.run_length.ub = REGEX_TOKEN_RUN_LENGTH_INFTY;
                if (ret) {
                    return REGEX_ERR_OUT_OF_MEMORY;
                }
                state ^= STATE_QUANTIFIABLE | STATE_QUANTIFIER;
            }
            break;
        case '*':
            if (state & STATE_ESCAPE) {
                cur.type = REGEX_TOKEN_TYPE_SINGLE;
                memset(cur.data.single.acceptable_characters, 0, 32);
                cur.data.single.acceptable_characters['*' >> 3] |=
                    1 << ('*' & 7);
                state &= ~STATE_ESCAPE;
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
                            ], &cur
                    );
                if (ret) {
                    return REGEX_ERR_OUT_OF_MEMORY;
                }
                state |= STATE_QUANTIFIABLE;
                state &= ~STATE_QUANTIFIER;
            } else {
                if (state & STATE_QUANTIFIER) {
                    return REGEX_ERR_DUPLICATE_QUANTIFIER;
                }
                regex
                    ->groups
                    .data[group_idx]
                    .data[
                        regex
                            ->groups
                            .data[group_idx]
                            .size - 1
                    ].data[
                        regex
                            ->groups
                            .data[group_idx]
                            .data[
                                regex
                                    ->groups
                                    .data[group_idx]
                                    .size - 1
                            ].size - 1
                    ].data.single.run_length.lb = 0;
                regex
                    ->groups
                    .data[group_idx]
                    .data[
                        regex
                            ->groups
                            .data[group_idx]
                            .size - 1
                    ].data[
                        regex
                            ->groups
                            .data[group_idx]
                            .data[
                                regex
                                    ->groups
                                    .data[group_idx]
                                    .size - 1
                            ].size - 1
                    ].data.single.run_length.ub = REGEX_TOKEN_RUN_LENGTH_INFTY;
                if (ret) {
                    return REGEX_ERR_OUT_OF_MEMORY;
                }
                state ^= STATE_QUANTIFIABLE | STATE_QUANTIFIER;
            }
            break;
        case '\\':
            if (!(state & STATE_ESCAPE)) {
                state |= STATE_ESCAPE;
                state &= ~STATE_QUANTIFIABLE;
            } else {
                memset(cur.data.single.acceptable_characters, 0, 32);
                cur.data.single.acceptable_characters['\\' >> 3] |=
                    1 << ('\\' & 7);
                state &= ~STATE_ESCAPE;
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
                            ], &cur
                    );
                if (ret) {
                    return REGEX_ERR_OUT_OF_MEMORY;
                }
                state |= STATE_QUANTIFIABLE;
                state &= ~STATE_QUANTIFIER;
            }
            break;
        case '.':
            cur.type = REGEX_TOKEN_TYPE_SINGLE;
            if (state & STATE_ESCAPE) {
                memset(cur.data.single.acceptable_characters, 0, 32);
                cur.data.single.acceptable_characters['.' >> 3] |=
                    1 << ('.' & 7);
                state &= ~STATE_ESCAPE;
            } else {
                memset(cur.data.single.acceptable_characters, 0xff, 32);
            }
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
                        ], &cur
                );
            if (ret) {
                return REGEX_ERR_OUT_OF_MEMORY;
            }
            state |= STATE_QUANTIFIABLE;
            state &= ~STATE_QUANTIFIER;
            break;
        case '?':
            if (state & STATE_ESCAPE) {
                cur.type = REGEX_TOKEN_TYPE_SINGLE;
                memset(cur.data.single.acceptable_characters, 0, 32);
                cur.data.single.acceptable_characters['?' >> 3] |=
                    1 << ('?' & 7);
                state &= ~STATE_ESCAPE;
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
                            ], &cur
                    );
                if (ret) {
                    return REGEX_ERR_OUT_OF_MEMORY;
                }
                state |= STATE_QUANTIFIABLE;
                state &= ~STATE_QUANTIFIER;
            } else if (state & STATE_QUANTIFIER) {
                regex
                    ->groups
                    .data[group_idx]
                    .data[
                        regex
                            ->groups
                            .data[group_idx]
                            .size - 1
                    ].data[
                        regex
                            ->groups
                            .data[group_idx]
                            .data[
                                regex
                                    ->groups
                                    .data[group_idx]
                                    .size - 1
                            ].size - 1
                    ].data.single.run_length.ub |= REGEX_TOKEN_RUN_LENGTH_LAZY;
            } else {
                if (state & STATE_QUANTIFIER) {
                    return REGEX_ERR_DUPLICATE_QUANTIFIER;
                }
                regex
                    ->groups
                    .data[group_idx]
                    .data[
                        regex
                            ->groups
                            .data[group_idx]
                            .size - 1
                    ].data[
                        regex
                            ->groups
                            .data[group_idx]
                            .data[
                                regex
                                    ->groups
                                    .data[group_idx]
                                    .size - 1
                            ].size - 1
                    ].data.single.run_length.lb = 0;
                if (ret) {
                    return REGEX_ERR_OUT_OF_MEMORY;
                }
                state ^= STATE_QUANTIFIABLE | STATE_QUANTIFIER;
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
            state |= STATE_QUANTIFIABLE;
            state &= ~STATE_QUANTIFIER;
            break;
        }
    }
    if (state & ~(STATE_QUANTIFIABLE | STATE_QUANTIFIER)) {
        if (state & STATE_ESCAPE) {
            return REGEX_ERR_TRAILING_ESCAPE;
        }
        if (state & STATE_MULTIPLE) {
            return REGEX_ERR_UNBALANCED;
        }
    }
    return REGEX_ERR_OK;
}

#include <string.h>

#include <regex/regex.h>

#include "regex_compile.h"

RegexErrcode regex_compile(Regex *regex, char *pattern) {
    if (!regex || !pattern) {
        return REGEX_ERR_NULL_PTR;
    }
    size_t group_idx = 0;
    char *iter = pattern;
    char state = 0;
    RegexErrcode errcode = REGEX_ERR_OK;
    for (size_t i = 0; *iter; ++i, ++iter) {
        printf("%c 0x%02hhx\n", *iter, state);
        RegexToken cur;
        int ret = 0;
        switch (*iter) {
        case '[':
            errcode =
                regex_compile_handle_open_bracket(
                    regex,
                    &state,
                    &group_idx,
                    &cur
                );
            if (errcode) {
                return errcode;
            }
            break;
        case ']':
            errcode =
                regex_compile_handle_close_bracket(
                    regex,
                    &state,
                    &group_idx,
                    &cur
                );
            if (errcode) {
                return errcode;
            }
            break;
        case '(':
            break;
        case ')':
            break;
        case '{':
            errcode =
                regex_compile_handle_open_brace(
                    regex,
                    &state,
                    &group_idx,
                    &cur
                );
            if (errcode) {
                return errcode;
            }
            break;
        case '}':
            if (state & STATE_ESCAPE) {
                if (state & STATE_MULTIPLE) {
                    return REGEX_ERR_INVALID_ESCAPE_SEQUENCE;
                }
                cur.type = REGEX_TOKEN_TYPE_SINGLE;
                memset(cur.data.single.acceptable_characters, 0, 32);
                cur.data.single.acceptable_characters['}' >> 3] |=
                    1 << ('}' & 7);
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
            } else if (state & STATE_MULTIPLE) {
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
                    ].data.single.acceptable_characters['}' >> 3] |=
                        1 << ('}' & 7);
            } else {
                state = STATE_QUANTIFIER;
            }
            break;
        case '+':
            if (state & STATE_ESCAPE) {
                if (state & STATE_MULTIPLE) {
                    return REGEX_ERR_INVALID_ESCAPE_SEQUENCE;
                }
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
            } else if (state & STATE_MULTIPLE) {
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
                    ].data.single.acceptable_characters['+' >> 3] |=
                        1 << ('+' & 7);
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
                if (state & STATE_MULTIPLE) {
                    return REGEX_ERR_INVALID_ESCAPE_SEQUENCE;
                }
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
            } else if (state & STATE_MULTIPLE) {
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
                    ].data.single.acceptable_characters['*' >> 3] |=
                        1 << ('*' & 7);
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
                if (!(state & STATE_MULTIPLE)) {
                    state &= ~STATE_QUANTIFIABLE;
                }
            } else if (state & STATE_MULTIPLE) {
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
                    ].data.single.acceptable_characters['\\' >> 3] |=
                        1 << ('\\' & 7);
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
            if (!(state & STATE_MULTIPLE)) {
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
            } else {
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
                    ].data.single.acceptable_characters['.' >> 3] |=
                        1 << ('.' & 7);
            }
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
            } else if (state & STATE_MULTIPLE) {
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
                    ].data.single.acceptable_characters['?' >> 3] |=
                        1 << ('?' & 7);
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
            if ((state & STATE_MULTIPLE) && (state & STATE_ESCAPE)) {
                return REGEX_ERR_INVALID_ESCAPE_SEQUENCE;
            }
            if (!(state & STATE_MULTIPLE)) {
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
            } else {
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
                    ].data.single.acceptable_characters[*iter >> 3] |=
                        1 << (*iter & 7);
            }
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

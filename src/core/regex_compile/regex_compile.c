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
            errcode =
                regex_compile_handle_close_brace(
                    regex,
                    &state,
                    &group_idx,
                    &cur
                );
            if (errcode) {
                return errcode;
            }
            break;
        case '+':
            errcode =
                regex_compile_handle_plus(
                    regex,
                    &state,
                    &group_idx,
                    &cur
                );
            if (errcode) {
                return errcode;
            }
            break;
        case '*':
            errcode =
                regex_compile_handle_asterisk(
                    regex,
                    &state,
                    &group_idx,
                    &cur
                );
            if (errcode) {
                return errcode;
            }
            break;
        case '\\':
            errcode =
                regex_compile_handle_backslash(
                    regex,
                    &state,
                    &group_idx,
                    &cur
                );
            if (errcode) {
                return errcode;
            }
            break;
        case '.':
            errcode =
                regex_compile_handle_dot(
                    regex,
                    &state,
                    &group_idx,
                    &cur
                );
            if (errcode) {
                return errcode;
            }
            break;
        case '?':
            errcode =
                regex_compile_handle_question(
                    regex,
                    &state,
                    &group_idx,
                    &cur
                );
            if (errcode) {
                return errcode;
            }
            break;
        default:
            errcode =
                regex_compile_handle_default(
                    regex,
                    &state,
                    &group_idx,
                    &cur,
                    &iter
                );
            if (errcode) {
                return errcode;
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

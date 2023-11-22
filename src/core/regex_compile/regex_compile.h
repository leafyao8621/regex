#ifndef REGEX_CORE_REGEX_COMPILE_STATES_H_
#define REGEX_CORE_REGEX_COMIPLE_STATES_H_

#include <regex/regex.h>
#include <regex/errcode.h>

#define STATE_QUANTIFIABLE 0x1
#define STATE_QUANTIFIER 0x2
#define STATE_ESCAPE 0x4
#define STATE_MULTIPLE 0x8
#define STATE_REPETITION 0x10

RegexErrcode regex_compile_handle_open_bracket(
    Regex *regex, char *state, size_t *group_idx, RegexToken *cur);

RegexErrcode regex_compile_handle_close_bracket(
    Regex *regex, char *state, size_t *group_idx, RegexToken *cur);

RegexErrcode regex_compile_handle_open_brace(
    Regex *regex, char *state, size_t *group_idx, RegexToken *cur);

RegexErrcode regex_compile_handle_close_brace(
    Regex *regex, char *state, size_t *group_idx, RegexToken *cur);

RegexErrcode regex_compile_handle_plus(
    Regex *regex, char *state, size_t *group_idx, RegexToken *cur);

RegexErrcode regex_compile_handle_asterisk(
    Regex *regex, char *state, size_t *group_idx, RegexToken *cur);

RegexErrcode regex_compile_handle_backslash(
    Regex *regex, char *state, size_t *group_idx, RegexToken *cur);

RegexErrcode regex_compile_handle_dot(
    Regex *regex, char *state, size_t *group_idx, RegexToken *cur);

RegexErrcode regex_compile_handle_question(
    Regex *regex, char *state, size_t *group_idx, RegexToken *cur);

RegexErrcode regex_compile_handle_default(
    Regex *regex,
    char *state,
    size_t *group_idx,
    RegexToken *cur,
    char **iter);

#endif

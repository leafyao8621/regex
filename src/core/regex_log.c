#include <ctype.h>

#include <regex/regex.h>

inline static void log_token(RegexToken *token, FILE *fout) {
    fputs("token:\n", fout);
    switch (token->type) {
    case REGEX_TOKEN_TYPE_SINGLE:
        fputs("single\n", fout);
        for (unsigned i = 0; i <= 0xff; ++i) {
            if (
                token->data.single.acceptable_characters[i >> 3] &
                (1 << (i & 7))) {
                fprintf(
                    fout,
                    "acceptable %c 0x%02hhx\n",
                    isprint(i) ? i : '*',
                    i
                );
            }
        }
        fputs("run length:\n", fout);
        fprintf(fout, "lb: %lu\n", token->data.single.run_length.lb);
        fputs("ub: ", fout);
        if (
            (
                token->data.single.run_length.ub &
                REGEX_TOKEN_RUN_LENGTH_INFTY
            ) ==
            REGEX_TOKEN_RUN_LENGTH_INFTY) {
            fputs("INFTY", fout);
        } else {
            fprintf(
                fout,
                "%lu",
                token->data.single.run_length.ub &
                REGEX_TOKEN_RUN_LENGTH_INFTY
            );
        }
        if (
            token->data.single.run_length.ub &
            REGEX_TOKEN_RUN_LENGTH_LAZY) {
            fputs(" LAZY\n", fout);
        } else {
            fputc('\n', fout);
        }
        break;
    case REGEX_TOKEN_TYPE_GROUP:
        break;
    }
}

RegexErrcode regex_log(Regex *regex, FILE *fout) {
    if (!regex || !fout) {
        return REGEX_ERR_NULL_PTR;
    }
    RegexGroup *iter_group = regex->groups.data;
    for (size_t i = 0; i < regex->groups.size; ++i, ++iter_group) {
        fprintf(fout, "group %lu:\n", i);
        DArrayRegexToken *iter_alt = iter_group->data;
        for (size_t j = 0; j < iter_group->size; ++j, ++iter_alt) {
            fprintf(fout, "alt %lu:\n", j);
            RegexToken *iter_token = iter_alt->data;
            for (size_t k = 0; k < iter_alt->size; ++k, ++iter_token) {
                log_token(iter_token, fout);
            }
        }
    }
    return REGEX_ERR_OK;
}

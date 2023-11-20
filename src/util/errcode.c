#include <regex/errcode.h>

const char *regex_err_code_lookup[7] =
    {
        "REGEX_ERR_OK",
        "REGEX_ERR_NULL_PTR",
        "REGEX_ERR_OUT_OF_MEMORY",
        "REGEX_ERR_TRAILING_ESCAPE",
        "REGEX_ERR_UNBALANCED",
        "REGEX_ERR_DUPLICATE_QUANTIFIER",
        "REGEX_ERR_INVALID_ESCAPE_SEQUENCE"
    };

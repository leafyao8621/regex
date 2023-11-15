#ifndef REGEX_ERRCODE_H_
#define REGEX_ERRCODE_H_

typedef enum RegexErrcode {
    REGEX_ERR_OK,
    REGEX_ERR_NULL_PTR,
    REGEX_ERR_OUT_OF_MEMORY
} RegexErrcode;

extern const char *regex_err_code_lookup[3];

#endif

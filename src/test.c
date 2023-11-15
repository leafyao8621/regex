#include <stdio.h>
#include <regex.h>

#include <regex/regex.h>

#define TEST(pattern, str, group)\
puts("pattern: "pattern"\nstr: "str);\
puts("stdlib:");\
regcomp(&regex_std, pattern, REG_EXTENDED);\
ret = regexec(&regex_std, str, group, match_std, 0);\
switch (ret) {\
case 0:\
    iter_std = match_std;\
    for (size_t i = 0; i < group; ++i, ++iter_std) {\
        printf("start: %lu\nend: %lu\n", iter_std->rm_so, iter_std->rm_eo);\
    }\
    break;\
case REG_NOMATCH:\
    puts("No match");\
    break;\
default:\
    puts("Error");\
    break;\
}\
regfree(&regex_std);\
puts("test:");\
errcode = regex_initialize(&regex);\
printf(\
    "errcode: %d\ndescription: %s\n",\
    errcode,\
    regex_err_code_lookup[errcode]\
);\
errcode = regex_compile(&regex, pattern);\
printf(\
    "errcode: %d\ndescription: %s\n",\
    errcode,\
    regex_err_code_lookup[errcode]\
);\
regex_finalize(&regex);

int main(void) {
    Regex regex;
    RegexErrcode errcode;
    // RegexMatch match[10];
    int ret;
    regex_t regex_std;
    regmatch_t match_std[10], *iter_std;
    TEST("abcxxx", "abcxxxaa", 1)
    // TEST("([0-9]{4})-(0[0-9]|1[0-2])-([0-2][0-9]|3[01])", "2023-11-15", 4);
    // TEST("([0-9]{4})-(0[0-9]|1[0-2])-([0-2][0-9]|3[01])", "2023-15-15", 4);
    // TEST("([0-9]{4})-(0[0-9]|1[0-2])-([0-2][0-9]|3[01])", "2023-11-33", 4);
    return 0;
}

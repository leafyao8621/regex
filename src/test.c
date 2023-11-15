#include <stdio.h>
#include <regex.h>

#include <regex/regex.h>

int main(void) {
    Regex regex;
    RegexErrcode errcode = regex_initialize(&regex);
    regex_finalize(&regex);
    return 0;
}

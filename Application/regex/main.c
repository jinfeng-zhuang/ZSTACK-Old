// https://github.com/luvit/pcre2/blob/master/src/pcre2demo.c

#define PCRE2_CODE_UNIT_WIDTH 8

#include <zstack/zstack.h>
#include <pcre2/pcre2.h>

#include "main.h"

struct application app;
PCRE2_UCHAR errorbuffer[256];

int main(int argc, char *argv[])
{
    pcre2_code* reCM;
    const char* error;
    int erroffset;
    int errornumber;
    char pattern_CM[] = "^1(3[4-9]|5[012789]|8[78])\\d{8}$";

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    reCM = pcre2_compile(pattern_CM, PCRE2_ZERO_TERMINATED, 0, &errornumber, &erroffset, NULL);
    if (NULL == reCM) {
        pcre2_get_error_message(errornumber, errorbuffer, sizeof(errorbuffer));
        return -1;
    }

    //pcre2_match(reCM, subject, subject_len, 0, 0, match_data, NULL);

    log_init(app.param.log_config);

    return 0;
}

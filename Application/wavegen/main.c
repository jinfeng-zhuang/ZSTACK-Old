#include <math.h>

#include <zstack.h>

#include "main.h"

#define PI              (3.1415926)
#define SAMPLE_NUMBER   4096
#define SAMPLE_FREQ     (SAMPLE_NUMBER * 2)

struct application app;

int main(int argc, char *argv[])
{
    int i;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    app.sample = (float *)malloc(app.param.number * sizeof(float));

    wavegen(app.sample, app.param.number, app.param.freq, app.param.sample);

    if (app.param.format == FORMAT_CARRAY) {
        log_info("float data[%d] = {\n", app.param.number);
        for (i = 0; i < app.param.number - 1; i++) {
            log_info("%f,", app.sample[i]);
        }
        log_info("%f};\n", app.sample[i]);
    }
    else {
        for (i = 0; i < app.param.number; i++) {
            log_info("%f\n", app.sample[i]);
        }
    }

    return 0;
}

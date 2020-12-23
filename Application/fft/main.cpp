#include <math.h>
#include <zstack.h>

#include "main.h"

struct application app;

int main(int argc, char *argv[])
{
    int i;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    app.param.number = 2048;
    app.param.freq = 48;
    app.param.sample_freq = 1024; // Hz

    app.sample = (double *)malloc(app.param.number * sizeof(double));
    app.buffer = (double *)malloc(app.param.number * sizeof(double) * 2);

    wavegen(app.sample, app.param.number, app.param.freq, app.param.sample_freq);

    for (i = 0; i < app.param.number; i++) {
        app.buffer[i * 2 + 0] = app.sample[i];
        app.buffer[i * 2 + 1] = 0;
    }

    fft(app.buffer, app.param.number, 0);

    for (i = 0; i < app.param.number; i++) {
        log_info("%f\n", app.buffer[i]); 
    }

    return 0;
}

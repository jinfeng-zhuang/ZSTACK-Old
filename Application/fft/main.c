#include <math.h>
#include <zstack.h>

#include "main.h"

struct application app;

int main(int argc, char *argv[])
{
    int i;
    int ret;
    struct index_buffer ib;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    ret = readlines(app.param.filename, &ib);
    if (0 == ret)
        return -1;

    app.sample = (float *)malloc(ib.total * sizeof(float));
    if (NULL == app.sample)
        return -1;

    app.r_data = (float *)malloc(ib.total * sizeof(float));
    if (NULL == app.r_data)
        return -1;

    app.i_data = (float *)malloc(ib.total * sizeof(float));
    if (NULL == app.i_data)
        return -1;

    app.A_data = (float *)malloc(ib.total * sizeof(float));
    if (NULL == app.A_data)
        return -1;

    app.complex = (float *)malloc(ib.total * sizeof(float) * 2);
    if (NULL == app.complex)
        return -1;

#if 0
    for (i = 0; i < ib.total; i++) {
        app.complex[i * 2 + 0] = atof((char *)&ib.data[ib.index[i]]);
        app.complex[i * 2 + 1] = 0;
    }

    fft(app.complex, ib.total, 0);
#else
    for (i = 0; i < ib.total; i++) {
        app.r_data[i] = atof((char *)&ib.data[ib.index[i]]);
        app.i_data[i] = 0;
        app.A_data[i] = 0;
    }

    fft(app.r_data, app.i_data, ib.total);

    power_spectrum(app.r_data, app.i_data, app.A_data, ib.total);
#endif

    for (i = 0; i < ib.total; i++) {
#if 0
        log_info("%10.5f %10.5f\n", app.complex[i*2 + 0], app.complex[i*2 + 1]);
#else
        if (app.param.flag_power) {
            log_info("%f\n", app.A_data[i]);
        }
        else {
            log_info("%10.5f %10.5f\n", app.r_data[i], app.i_data[i]);
        }
#endif
    }

    return 0;
}

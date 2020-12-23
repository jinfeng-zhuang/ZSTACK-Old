#include <math.h>
#include <zstack.h>

#include "main.h"

extern double simulate_data[];

struct application app;

static void calc_waveform_parameter(void)
{
    int i;
    double max = 0;

    for (i = 0; i < 9557; i++) {
        if (app.sample[i] > max)
            max = app.sample[i];
    }
    app.waveform_parameter.max = max;

    double min = 0xFFFFFFFF;
    for (i = 0; i < 9557; i++) {
        if (app.sample[i] < min)
            min = app.sample[i];
    }
    app.waveform_parameter.min = min;

    double total = 0;
    for (i = 0; i < 9557; i++) {
        total += app.sample[i];
    }
    app.waveform_parameter.mean = total / 9557;

    total = 0;
    for (i = 0; i < 9557; i++) {
        total += _abs64(app.sample[i]);
    }
    app.waveform_parameter.aver = total / 9557;

    total = 0;
    for (i = 0; i < 9557; i++) {
        total += pow(app.sample[i], 2);
    }
    app.waveform_parameter.rms = sqrt(total / 9557);

    total = 0;
    for (i = 0; i < 9557; i++) {
        total += sqrt((double)_abs64(app.sample[i]));
    }
    total = total / 9557;
    total = pow(total, 2);

    app.waveform_parameter.smra = total;

    app.waveform_parameter.formf = app.waveform_parameter.rms / app.waveform_parameter.aver;
    app.waveform_parameter.crestfpos = app.waveform_parameter.max / app.waveform_parameter.rms;
    app.waveform_parameter.crestfneg = app.waveform_parameter.min / app.waveform_parameter.rms;
    app.waveform_parameter.impulsefpos = app.waveform_parameter.max / app.waveform_parameter.aver;
    app.waveform_parameter.impulsefneg = app.waveform_parameter.min / app.waveform_parameter.aver;
    app.waveform_parameter.marginfpos = app.waveform_parameter.max / app.waveform_parameter.smra;
    app.waveform_parameter.marginfneg = app.waveform_parameter.min / app.waveform_parameter.smra;

    total = 0;
    for (i = 0; i < 9557; i++) {
        total += pow(app.sample[i], 4);
    }
    total = total / 9557;
    app.waveform_parameter.kurtosis = total / pow(app.waveform_parameter.rms, 4);

    total = 0;
    for (i = 0; i < 9557; i++) {
        total += pow(app.sample[i], 3);
    }
    total = total / 9557;
    app.waveform_parameter.skewness = total / pow(app.waveform_parameter.rms, 3);

    log_info(
        "waveform_parameters:\n"
        "Max = %12.5f\n"
        "Min=%12.5f\n"
        "Mean=%f\n"
        "ABS Mean=%f\n"
        "RMS=%f\n"
        "SMRA=%f\n"
        "formf=%f\n"
        "crestfpos=%f\n"
        "crestfneg=%f\n"
        "impulsefpos=%f\n"
        "impulsefneg=%f\n"
        "marginfpos=%f\n"
        "marginfneg=%f\n"
        "kurtosis=%f\n"
        "skewness=%f\n",
        app.waveform_parameter.max,
        app.waveform_parameter.min,
        app.waveform_parameter.mean,
        app.waveform_parameter.aver,
        app.waveform_parameter.rms,
        app.waveform_parameter.smra,
        app.waveform_parameter.formf,
        app.waveform_parameter.crestfpos,
        app.waveform_parameter.crestfneg,
        app.waveform_parameter.impulsefpos,
        app.waveform_parameter.impulsefneg,
        app.waveform_parameter.marginfpos,
        app.waveform_parameter.marginfneg,
        app.waveform_parameter.kurtosis,
        app.waveform_parameter.skewness
        );
}

int main(int argc, char *argv[])
{
	int i;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

#if 0
	wavegen(app.sample, 1024, app.param.freq, app.param.sample_freq);

	for (i = 0; i < 1024; i++) {
		log_info("%f \n", (float)app.sample[i]);
	}
#elif 0
    // test hanning window
    for (i = 0; i < 1024; i++) {
        //app.sample[i] = 1;
        wavegen(app.sample, 1024, 48, 1000);
    }

    hanning_window(app.sample, 1024);

	for (i = 0; i < 1024; i++) {
		log_info("%f \n", (float)app.sample[i]);
	}
#endif

    for (i = 0; i < 9557; i++) {
        app.sample[i] = simulate_data[i];
    }

    linear_regression(app.sample, 9557);

    calc_waveform_parameter();

    return 0;
}

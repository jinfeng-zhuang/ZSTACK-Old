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

	wavegen((float*)app.sample, 1024, app.param.freq, app.param.sample_freq);

	for (i = 0; i < 1024; i++) {
		//printf("%f \n", (float)app.sample[i]);
	}

    return 0;
}

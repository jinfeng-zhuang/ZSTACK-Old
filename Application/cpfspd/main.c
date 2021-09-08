#include <zstack/zstack.h>
#include <cpfspd/cpfspd.h>

#include "main.h"

struct application app;

int main(int argc, char *argv[])
{
    pT_status status;
    pT_header header;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    status = p_read_header(app.param.input_filename, &header);
    if (P_OK == status) {
        p_print_header(&header, stdout);
    }
    else {
        info("file not found: %s\n", app.param.input_filename);
    }

    return 0;
}

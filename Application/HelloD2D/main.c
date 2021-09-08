// https://docs.microsoft.com/en-us/windows/win32/direct2d/direct2d-quickstart
// https://docs.microsoft.com/en-us/windows/win32/direct2d/getting-started-with-direct2d?redirectedfrom=MSDN

#include <zstack/zstack.h>
#include <d2d1.h>
#include <Windows.h>

#include "main.h"

struct application app;

int main(int argc, char *argv[])
{
    ID2D1Factory* pD2DFactory = NULL;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    // start point of D2D, to create D2D resources
    HRESULT hr = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED, 0, 0,
        &pD2DFactory
    );

    return 0;
}

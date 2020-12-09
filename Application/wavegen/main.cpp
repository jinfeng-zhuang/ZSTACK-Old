//#include <math.h>

#include <zstack/log.h>

#define PI              (3.1415926)
#define SAMPLE_NUMBER   4096
#define SAMPLE_FREQ     (SAMPLE_NUMBER * 2)

int main(int argc, char *argv[])
{
    int x;
    float y;
    
    int amp = 5;
    int offset = 0;
    int freq = 50;

    log(LOG_USER, "Hello World\n");

    for (x = 0; x < SAMPLE_NUMBER; x++) {
        //y = amp * sin(2.0 * PI * x * freq / SAMPLE_FREQ) + offset;
        log(LOG_USER, "%f\n", y);
    }

    return 0;
}
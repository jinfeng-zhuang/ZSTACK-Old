#include <Windows.h>
#include <stdio.h>

struct frame {
    unsigned char data[500*500*4];
};

static struct frame frames[64];
static int index;

static unsigned char frameDemo[500*500*4];
static unsigned char frameCapture[500*500*4];

// alpha ref: https://www.cnblogs.com/oloroso/p/10724803.html
unsigned char *FrameGenerator_Get(void)
{
    int i;
    int j;
    unsigned char alpha;
    
    index = index % 64;
    
    unsigned char *data = frames[index].data;

    for (i = 0; i < 500; i++) {
        for (j = 0; j < 500; j++) {
            alpha = index * 3;
            frameCapture[i*500*4 + j*4+0] = data[i*500*4 + j*4+0] * alpha >> 8;
            frameCapture[i*500*4 + j*4+1] = data[i*500*4 + j*4+1] * alpha >> 8;
            frameCapture[i*500*4 + j*4+2] = data[i*500*4 + j*4+2] * alpha >> 8; 
            frameCapture[i*500*4 + j*4+3] = alpha; // alpha, 0 - transparent, see nothing
        }
    }

    index++;

    return frameCapture;
    //return frameDemo;
}

void FrameGeneratorInit(void)
{
    int framenum;
    int i, j;

    for (i = 0; i < 500*500*4; i+=4) {
        frameDemo[i] = 0xFF;
    }
    
    for (framenum = 0; framenum < 64; framenum++) {
        for (i = 0; i < 500; i++) {
            for (j = 0; j < 500; j++) {
                frames[framenum].data[i*500*4 + j*4+3] = framenum * 3; // alpha, 0 - transparent, see nothing
                frames[framenum].data[i*500*4 + j*4+2] = 0xFF;
            }
        }
    }
}

int FrameGenerator(void *arg)
{
    FrameGeneratorInit();
    
    while (1) {
        //printf("%s\n", __func__);
        Sleep(1000);
    }
}

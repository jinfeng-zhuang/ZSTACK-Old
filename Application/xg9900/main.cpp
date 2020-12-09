#include <zstack/log.h>

struct {
    float sample[9557];
    int run_threshold;
    int channel_enable[3];
    int channel_max[3];
    int channel_min[3];
    int channel_running[3];
    int machine_running;
} db;

int main(int argc, char *argv[])
{
    int i;
    unsigned short min = 0xFFFF;
    unsigned short max = 0;
    int channel;

    for (channel = 0; channel < 3; channel++) {
        //if (db.channel_enable[channel] == 0)
         //   continue;

        // 1. do sample

        // 2. do compare
        for (i=0; i<3072; i++) {
            min = (db.sample[i] < min) ? db.sample[i] : min;
            max = (db.sample[i] > max) ? db.sample[i] : max;
        }

        if ((max - min) < db.run_threshold) {
            db.channel_running[channel] = 0;
        }

        db.channel_max[channel] = max;
        db.channel_min[channel] = min;
    }

    if ((db.channel_running[0] == 0) && (db.channel_running[1] == 0) && (db.channel_running[2] == 0)) {
        db.machine_running = 0;
    }

    ////////////////////////////////////////////////////////

    float avg_xy = 0.0, avg_x = 0.0, avg_y = 0.0, avg_xx = 0.0;
    int length = 9557;
    float slope;
    float intercept;

    for (channel = 0; channel < 3; channel++) {
        //if (db.channel_running[channel] == 0)
        //    continue;

        // do sample: 9557

        // convert to 4-byte float
        
        // do linear regression
        for (i=0; i <length; i++) {
            avg_xy += i * db.sample[i] / length;
            avg_x += i / length;
            avg_y += db.sample[i] / length;
            avg_xx += i * i / length;
        }

        slope = (avg_xy - avg_x * avg_y) / (avg_xx - avg_x * avg_x);
        intercept = avg_y - slope * avg_x;

        for (i=0; i <length; i++) {
            db.sample[i] = db.sample[i] - (slope * i + intercept);
        }
     }

    //////////////////////////////////////////////////////////////////////////////////

    return 0;
}
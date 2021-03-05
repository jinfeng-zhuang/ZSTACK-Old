#include <zstack.h>
#include <3rdparty/cJSON.h>

#include "main.h"

struct application app;

#define CHANNEL_MAX 3

struct config {
    float version;
    
    float ref_speed_rpm;
    
    float ref_speed_rpm_array[3];
    float chan_enable[CHANNEL_MAX];
    float interval[3];
    
    float running_threshold_digit;
    float overload_threshold_digit;
    
    float sensor_sensitivity;
    float sensor_low_cutoff;
    float sensor_high_cutoff;
    
    float sampling_freq;
    float system_gain;
    float adc_resolution_bit;
    float adc_ref_voltage;
    
    float fft_frame_size;
    float fft_average;
    float fft_wvfm_overlap;
    float freq_span;
    float petal_num;
    float hamonic_para_span;
    float pdf_plt_range[2];
    float pdf_bin_width;
};

int config_parse(struct config *config, char *buffer)
{
    cJSON *monitor_json;
    cJSON *item = NULL;
    cJSON *array = NULL;
    int i;
    
    monitor_json = cJSON_Parse(buffer);
    if (monitor_json == NULL) {
        return -1;
    }

    item = cJSON_GetObjectItemCaseSensitive(monitor_json, "version");
    config->version = item->valueint;
    
    array = cJSON_GetObjectItemCaseSensitive(monitor_json, "reference_rpm");
    for (i = 0; i < 3; i++) {
        item = cJSON_GetArrayItem(array, i);
        config->ref_speed_rpm_array[i] = item->valuedouble;
    }

    array = cJSON_GetObjectItemCaseSensitive(monitor_json, "channel_enable");
    for (i = 0; i < 3; i++) {
        item = cJSON_GetArrayItem(array, i);
        config->chan_enable[i] = item->valuedouble;
    }
    
    array = cJSON_GetObjectItemCaseSensitive(monitor_json, "interval");
    for (i = 0; i < 3; i++) {
        item = cJSON_GetArrayItem(array, i);
        config->interval[i] = item->valueint;
    }

    item = cJSON_GetObjectItemCaseSensitive(monitor_json, "running_threshold_digit");
    config->running_threshold_digit = item->valuedouble;

    item = cJSON_GetObjectItemCaseSensitive(monitor_json, "overload_threshold_digit");
    config->overload_threshold_digit = item->valuedouble;

    item = cJSON_GetObjectItemCaseSensitive(monitor_json, "sensor_sensitivity");
    config->sensor_sensitivity = item->valuedouble;

    item = cJSON_GetObjectItemCaseSensitive(monitor_json, "sensor_low_cutoff");
    config->sensor_low_cutoff = item->valuedouble;
    
    item = cJSON_GetObjectItemCaseSensitive(monitor_json, "sensor_high_cutoff");
    config->sensor_high_cutoff = item->valuedouble;

    item = cJSON_GetObjectItemCaseSensitive(monitor_json, "sampling_freq");
    config->sampling_freq = item->valuedouble;

    item = cJSON_GetObjectItemCaseSensitive(monitor_json, "system_gain");
    config->system_gain = item->valuedouble;

    item = cJSON_GetObjectItemCaseSensitive(monitor_json, "adc_resolution_bit");
    config->adc_resolution_bit = item->valuedouble;

    item = cJSON_GetObjectItemCaseSensitive(monitor_json, "adc_ref_voltage");
    config->adc_ref_voltage = item->valuedouble;

    item = cJSON_GetObjectItemCaseSensitive(monitor_json, "fft_frame_size");
    config->fft_frame_size = item->valuedouble;

    item = cJSON_GetObjectItemCaseSensitive(monitor_json, "fft_average");
    config->fft_average = item->valuedouble;

    item = cJSON_GetObjectItemCaseSensitive(monitor_json, "fft_wvfm_overlap");
    config->fft_wvfm_overlap = item->valuedouble;
    
    item = cJSON_GetObjectItemCaseSensitive(monitor_json, "freq_span");
    config->freq_span = item->valuedouble;

    item = cJSON_GetObjectItemCaseSensitive(monitor_json, "petal_num");
    config->petal_num = item->valuedouble;

    item = cJSON_GetObjectItemCaseSensitive(monitor_json, "hamonic_para_span");
    config->hamonic_para_span = item->valuedouble;

    array = cJSON_GetObjectItemCaseSensitive(monitor_json, "pdf_plt_range");
    for (i = 0; i < 2; i++) {
        item = cJSON_GetArrayItem(array, i);
        config->pdf_plt_range[i] = item->valuedouble;
    }

    item = cJSON_GetObjectItemCaseSensitive(monitor_json, "pdf_bin_width");
    config->pdf_bin_width = item->valuedouble;
    return 0;
}

int main(int argc, char *argv[])
{
    int ret;
    struct config config;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    app.file_content = file_load(app.param.filename, &app.file_size);

    memset(&config, 0, sizeof(struct config));

    config_parse(&config, (char *)app.file_content);

    info("config dump\n");
    info("version: %f\n", config.version);
    info("speed_rpm: %d\n", config.ref_speed_rpm);
    info("speed_rpm_array: %d %d %d\n", config.ref_speed_rpm_array[0],config.ref_speed_rpm_array[1],config.ref_speed_rpm_array[2]);
    info("interval: %f %f %f\n", config.interval[0],config.interval[1],config.interval[2]);

    return 0;
}

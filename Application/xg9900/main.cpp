#include <math.h>
#include <assert.h>
#include <zstack.h>
#include <limits.h>

#include "main.h"

extern float simulate_data[];

struct application app;

static void calc_waveform_parameter(void)
{
    int i;
    float max = 0;

    for (i = 0; i < SAMPLE_SIZE; i++) {
        if (app.sample[i] > max)
            max = app.sample[i];
    }
    app.waveform_parameter.max = max;

    float min = 0xFFFFFFFF;
    for (i = 0; i < SAMPLE_SIZE; i++) {
        if (app.sample[i] < min)
            min = app.sample[i];
    }
    app.waveform_parameter.min = min;

    float total = 0;
    for (i = 0; i < SAMPLE_SIZE; i++) {
        total += app.sample[i];
    }
    app.waveform_parameter.mean = total / SAMPLE_SIZE;

    total = 0;
    for (i = 0; i < SAMPLE_SIZE; i++) {
        total += abs(app.sample[i]);
    }
    app.waveform_parameter.aver = total / SAMPLE_SIZE;

    total = 0;
    for (i = 0; i < SAMPLE_SIZE; i++) {
        total += pow(app.sample[i], 2);
    }
    app.waveform_parameter.rms = sqrt(total / SAMPLE_SIZE);

    total = 0;
    for (i = 0; i < SAMPLE_SIZE; i++) {
        total += sqrt((float)abs(app.sample[i]));
    }
    total = total / SAMPLE_SIZE;
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
    for (i = 0; i < SAMPLE_SIZE; i++) {
        total += pow(app.sample[i], 4);
    }
    total = total / SAMPLE_SIZE;
    app.waveform_parameter.kurtosis = total / pow(app.waveform_parameter.rms, 4);

    total = 0;
    for (i = 0; i < SAMPLE_SIZE; i++) {
        total += pow(app.sample[i], 3);
    }
    total = total / SAMPLE_SIZE;
    app.waveform_parameter.skewness = total / pow(app.waveform_parameter.rms, 3);

    log_info("-------------------------------------------------------------------------------------------------------\n");
    log_info("%15s %s\n", "Waveform", "Value");
    log_info("-------------------------------------------------------------------------------------------------------\n");
    log_info(
        "%15s %f\n"
        "%15s %f\n"
        "%15s %f\n"
        "%15s %f\n"
        "%15s %f\n"
        "%15s %f\n"
        "%15s %f\n"
        "%15s %f\n"
        "%15s %f\n"
        "%15s %f\n"
        "%15s %f\n"
        "%15s %f\n"
        "%15s %f\n"
        "%15s %f\n"
        "%15s %f\n",

        "Max", app.waveform_parameter.max,
        "Min", app.waveform_parameter.min,
        "Mean", app.waveform_parameter.mean,
        "Average", app.waveform_parameter.aver,
        "RMS", app.waveform_parameter.rms,
        "SMRA", app.waveform_parameter.smra,
        "FormF", app.waveform_parameter.formf,
        "CrestFPos", app.waveform_parameter.crestfpos,
        "CrestFNeg", app.waveform_parameter.crestfneg,
        "ImpulseFPos", app.waveform_parameter.impulsefpos,
        "ImpulseFNeg", app.waveform_parameter.impulsefneg,
        "MarginFPos", app.waveform_parameter.marginfpos,
        "MarginFNeg", app.waveform_parameter.marginfneg,
        "Kurtosis", app.waveform_parameter.kurtosis,
        "Skewness", app.waveform_parameter.skewness
        );
    log_info("-------------------------------------------------------------------------------------------------------\n");
}

static int convert_to_complex(float *data, unsigned int n)
{
    unsigned int i;

    for (i = n; i > 0; i--) {
        data[i * 2 - 1] = data[i - 1];
        data[i * 2 - 2] = 0;
    }

    return 0;
}

static float digit2micron(float x)
{
    float ref_voltage = 3300.0;
    float adc_resolution_bit = 15;
    float sensor_sensitivity = 7.874;
    float hardware_gain = 12.0;

    x *= ref_voltage/pow(2, adc_resolution_bit);
    x /= hardware_gain;
    x /= sensor_sensitivity;
        
    return x;
}

static int find_running_speed_true(float *power, unsigned int length, unsigned int ref_line, float *true_speed)
{
    unsigned int i;
    unsigned int start, end;
    float max = 0;
    unsigned int max_index = 0;
    float freq_span = 0.025;
    unsigned int petal_num = 2;
    float sum;
    float petal_power_sum, left_petal_ratio, right_petal_ratio, petal_ratio_sum, speed_true;
	  //float Amp_1xRPM_true;

    // 寻找范围，参考转速左右各2.5%，找到最大峰值
    start = ref_line * (1 - freq_span);
    end   = ref_line * (1 + freq_span) + 1;

    for (i = start; i < end + 1; i++) {
        if (power[i] > max) {
            max = power[i];
            max_index = i;
        }
    }

    assert(0 != max_index);

    // 峰值左右各取2点，共5点，进行计算
    sum = 0;
    for (i = max_index - petal_num; i < max_index + petal_num + 1; i++) {
        sum += power[i];
    }
    petal_power_sum = sum;

    sum = 0;
    for (i = max_index - petal_num; i < max_index; i++) {
        sum += power[i];
    }
    left_petal_ratio = sum / petal_power_sum;

    sum = 0;
    for (i = max_index + 1; i < max_index + petal_num + 1; i++) {
        sum += power[i];
    }
    right_petal_ratio = sum / petal_power_sum;

    petal_ratio_sum = left_petal_ratio + right_petal_ratio;

    if ((petal_ratio_sum < 0.33) || (petal_ratio_sum > 0.52)) {
        *true_speed = 0.0;

        log_warn("True speed not found\n");
    }
    else {
        speed_true = 0.0;
        for (i = max_index - petal_num; i < max_index + petal_num + 1; i++) {
            speed_true += i * power[i];
        }
        speed_true /= petal_power_sum;
        //Amp_1xRPM_true = sqrt(petal_power_sum * 8.0 / 3.0) / 2.0;

        *true_speed = speed_true;

        //log_info("True speed: %f %f\n", speed_true, Amp_1xRPM_true); // TODO 'Amp_1xRPM_true' not used
				log_info("True speed: %f\n", speed_true);
    }

    return 0;
}

float round(float number)
{
    return number < 0.0 ? ceil(number - 0.5) : floor(number + 0.5);
}

enum np_type {
    NP_DOUBLE,
};

unsigned long long np_amax(enum np_type, void *a, unsigned int s, unsigned int e)
{
    unsigned int i;
    float *a_double = (float *)a;
    float a_double_max = -1; // TODO

    for (i = s; i < e; i++) {
        if (a_double[i] > a_double_max)
            a_double_max = a_double[i];
    }

    return (unsigned long long)a_double_max;
}

unsigned long long np_amin(enum np_type, void *a, unsigned int s, unsigned int e)
{
    unsigned int i;
    float *a_double = (float *)a;
    float a_double_min = 0xFFFFFFFF; // TODO

    for (i = s; i < e; i++) {
        if (a_double[i] < a_double_min)
            a_double_min = a_double[i];
    }

    return (unsigned long long)a_double_min;
}

/*
 * 寻找真峰峰值
 *
 * 在1.5个周期内，最大值最小值的差
 * 移动1个周期，找下一个最大值最小值的差
 * 比较，保留大的
 */
static int find_true_pkpk(float *wvfm, unsigned int length, float spd, float *pkpk)
{
    unsigned int wvfm_len;
    unsigned int sampling_points_in_cycle;
    unsigned int i, j;
    float true_pkpk_digit;

    wvfm_len = length;
    sampling_points_in_cycle = round(FRAME_SIZE / spd);

    i = 0;
    j = i + sampling_points_in_cycle * 1.5;
    true_pkpk_digit = 0.0;

    while (j <= (wvfm_len-1)) {
        true_pkpk_digit = max(true_pkpk_digit, np_amax(NP_DOUBLE, wvfm, i, j) - np_amin(NP_DOUBLE, wvfm, i, j));
        i += sampling_points_in_cycle;
        j += sampling_points_in_cycle;
    }

    *pkpk = true_pkpk_digit;

    log_info("True Pk-Pk Value is %f Microns\n", digit2micron(true_pkpk_digit));

    return 0;
}

static int calculate_probability_density(float *x)
{
    return 0;
}

static float calculate_band_energy(float *power, unsigned int *ctrl)
{
    unsigned int i;
    float energy;
    float sum = 0;
    unsigned int s = ctrl[0];
    unsigned int e = ctrl[1];

    energy = 0.5 * power[s];

    for (i = s + 1; i < e; i++) {
        sum += power[i];
    }

    energy += sum;
    energy += 0.5 * power[e];

    return energy;
}

static char *para_nonsyn[] = {
    "Subsynchronous",
    "Nonsyn 1-2xRPM",
    "Nonsyn 2-3xRPM",
    "Nonsyn 3-4xRPM",
    "Nonsyn 4-5xRPM",
    "Nonsyn 5-6xRPM",
    "Nonsyn 6-7xRPM",
    "Nonsyn 7-8xRPM",
    "Nonsyn 8-9xRPM",
    "Nonsyn 9-10xRPM"
};

static char *para_harmonic[] = {
    "Harmonic 1xRPM",
    "Harmonic 2xRPM",
    "Harmonic 3xRPM",
    "Harmonic 4xRPM",
    "Harmonic 5xRPM",
    "Harmonic 6xRPM",
    "Harmonic 7xRPM",
    "Harmonic 8xRPM",
    "Harmonic 9xRPM",
    "Harmonic 10xRPM"
};

static int calculate_spectral_parameters(float *pw_spc, unsigned int length, float spd, float *para_overall_pkpk, float *pa_nonsyn_value, float *pa_harmonic_value)
{
    float pa_span = 0.15;

    unsigned int spc_len;
    unsigned int pa_harmonic_span[10][2];
    unsigned int pa_nonsyn_span[10][2];
    unsigned int harmonic_number;
    float overall_pkpk_value;
    unsigned int i;
    unsigned int overall_pkpk_span[2];

    memset(pa_harmonic_span, 0, sizeof(pa_harmonic_span));
    memset(pa_nonsyn_span, 0, sizeof(pa_nonsyn_span));

    spc_len = length;

    harmonic_number = length / spd;
    if (harmonic_number > 10)
        harmonic_number = 10;

    overall_pkpk_span[0] = 0;
    overall_pkpk_span[1] = spc_len - 1;

    overall_pkpk_value = calculate_band_energy(pw_spc, overall_pkpk_span);
    overall_pkpk_value = sqrt(overall_pkpk_value * 8.0 / 3.0) / 2.0;

    log_info("Overall Pk-Pk Value is %f Microns\n", digit2micron(overall_pkpk_value));

    *para_overall_pkpk = overall_pkpk_value;

    // 计算频段的上下限
    pa_nonsyn_span[0][0] = (float)FRAME_SIZE * FREQ_LIMIT_LOW / SAMPLE_FREQ + 0.5; // TODO if float take effect

    for (i = 0; i < harmonic_number; i++) {
        pa_nonsyn_span[i][1] = round((i + 1 - pa_span) * spd);
        pa_harmonic_span[i][0] = pa_nonsyn_span[i][1];
        pa_harmonic_span[i][1] = round((i + 1 + pa_span) * spd);

        if (i < 9) {
            pa_nonsyn_span[i+1][0] = pa_harmonic_span[i][1];
        }
    }

    // 修正最后一个元素
    if (pa_harmonic_span[harmonic_number-1][1] > (spc_len-1)) {
        pa_harmonic_span[harmonic_number-1][1]=(spc_len-1);
        if (harmonic_number < 10) {
            pa_nonsyn_span[harmonic_number][0] = 0;
        }
    }
    else if (harmonic_number < 10) {
        pa_nonsyn_span[harmonic_number][1] = round((harmonic_number+1-pa_span)*spd);
        if (pa_nonsyn_span[harmonic_number][1] > (spc_len-1)) {
            pa_nonsyn_span[harmonic_number][1]=(spc_len-1);
        }
    }

    for (i = 0; i < 10; i++) {
        if (pa_nonsyn_span[i][0] && pa_nonsyn_span[i][1]) {
            pa_nonsyn_value[i] = calculate_band_energy(pw_spc, pa_nonsyn_span[i]);
            pa_nonsyn_value[i] = sqrt(pa_nonsyn_value[i] * 8.0 / 3.0) / 2.0;
        }

        if ((pa_harmonic_span[i,0]) && (pa_harmonic_span[i,1])) {
            pa_harmonic_value[i] = calculate_band_energy(pw_spc, pa_harmonic_span[i]);
            pa_harmonic_value[i] = sqrt(pa_harmonic_value[i] * 8.0 / 3.0) / 2.0;
        }
    }

#if 1
    log_info("-------------------------------------------------------------------------------------------------------\n");
    log_info("%20s\t%4s %4s %10s %6s\n", "Parameter Name", "Bin", "Edge", "Microns", "Digital");
    log_info("-------------------------------------------------------------------------------------------------------\n");
    for (i = 0; i < 10; i++) {
        log_info("%20s:\t%4d %4d %10.5f %6.0f\n", para_nonsyn[i], pa_nonsyn_span[i][0], pa_nonsyn_span[i][1], digit2micron(pa_nonsyn_value[i]), pa_nonsyn_value[i]);
    }
    for (i = 0; i < 10; i++) {
        log_info("%20s:\t%4d %4d %10.5f %6.0f\n", para_harmonic[i], pa_harmonic_span[i][0], pa_harmonic_span[i][1], digit2micron(pa_harmonic_value[i]), pa_harmonic_value[i]);
    }
    log_info("-------------------------------------------------------------------------------------------------------\n");
#endif

    return 0;
}

int main(int argc, char *argv[])
{
	int i, j;
    unsigned int sample_start_point;
    unsigned char *file_content;
    unsigned int file_size;
    int split_offset;
    char digit_str[64];

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    log_info("Application Size %d KB\n", sizeof(app) >> 10);

    file_content = file_load(app.param.filename, &file_size);
    if (NULL == file_content) {
        log_info("Can't open '%s'\n", app.param.filename);
        return -1;
    }

    split_offset = 0;
    for (i = 0; i < SAMPLE_SIZE; i++) {
        memset(digit_str, 0, sizeof(digit_str));
        split(digit_str, sizeof(digit_str), (char*)file_content, file_size, ',', &split_offset);
        app.sample[i] = atoi(digit_str);
    }

    //===================== 采样结束 ==============================

    // 去除趋势项
    linear_regression(app.sample, SAMPLE_SIZE);

    // 计算波形参数
    calc_waveform_parameter();

    // 进行 5 次 FFT 做平均
    for (i = 0; i < 5; i++) {
        sample_start_point = i * FRAME_SIZE * (1.0 - 2.0/3.0);

        memcpy(&app.sample[SAMPLE_CALC_ADDR], &app.sample[sample_start_point], FRAME_SIZE * sizeof(float));

        linear_regression(&app.sample[SAMPLE_CALC_ADDR], FRAME_SIZE);

        // TODO: shouldn't change the value in sample, otherwise the overlapped area will be change
        hanning_window(&app.sample[SAMPLE_CALC_ADDR], FRAME_SIZE);

        memset(&app.sample[SAMPLE_CALC_ADDR + FRAME_SIZE], 0, FRAME_SIZE * sizeof(float));
        fft(&app.sample[SAMPLE_CALC_ADDR], &app.sample[SAMPLE_CALC_ADDR + FRAME_SIZE], FRAME_SIZE);
        power_spectrum(&app.sample[SAMPLE_CALC_ADDR], &app.sample[SAMPLE_CALC_ADDR + FRAME_SIZE], &app.sample[SAMPLE_CALC_ADDR], FRAME_SIZE);

        // FFT Normalization
        for (j = 1; j < FRAME_SIZE; j++) {
            app.sample[SAMPLE_CALC_ADDR + j] = app.sample[SAMPLE_CALC_ADDR + j] * 2.0 / FRAME_SIZE;
        }
        app.sample[SAMPLE_CALC_ADDR] = 0; // Remove DC current

        // 计算 5 次的平均值
        for (j = 0; j < FRAME_SIZE; j++) {
            app.sample[ADDR_FFT + j] *= i;
            app.sample[ADDR_FFT + j] += app.sample[SAMPLE_CALC_ADDR + j];
            app.sample[ADDR_FFT + j] /= (i + 1);
        }
    }

    // 将传感器频率下限以下的值置零
    app.lines = (FRAME_SIZE / 2.56) + 1;
    app.low_cut = ((FRAME_SIZE * FREQ_LIMIT_LOW) / SAMPLE_FREQ) + 1;

    for (j = 0; j < app.low_cut; j++) {
        app.sample[ADDR_FFT + j] = 0.0;
    }

    for (j = 0; j < FRAME_SIZE; j++) {
        app.sample[ADDR_FFT + j] *= 4.0;
    }

    // 计算位移通频值（峰峰值），频谱的平方和，经过修正
    float sum = 0;
    for (i = 0; i < app.lines; i++) {
        sum += pow(app.sample[ADDR_FFT + i], 2);
    }
    app.dsp_Overall = sum;
    app.dsp_Overall = sqrt(app.dsp_Overall * 8.0 / 3.0) / 2;
    app.dsp_Overall_um = digit2micron(app.dsp_Overall);

    // 计算速度通频值（有效均方根值），频谱的平方和，经过修正
    sum = 0;
    for (i = 0; i < app.lines; i++) {
        app.sample[ADDR_VEL + i] = app.sample[ADDR_FFT + i] * i * 2.0 * PI * SAMPLE_FREQ / FRAME_SIZE; // 将位移谱转为速度谱，谱上每一点乘以2*PI*该点频率
        sum += pow(app.sample[ADDR_VEL + i], 2);
    }
    app.vel_Overall = sum;
    app.vel_Overall = sqrt(app.vel_Overall * 8.0 / 3.0) / 2 / 2 / sqrt(2.0); // 修正，并转为有效均方根值
    app.vel_Overall = app.vel_Overall / 1000; // 单位转换
    app.vel_Overall_mmps = digit2micron(app.vel_Overall);

    // 计算加速度通频值（有效均方根值），频谱的平方和，经过修正
    sum = 0;
    for (i = 0; i < app.lines; i++) {
        app.sample[ADDR_ACC + i] = app.sample[ADDR_VEL + i] * i * 2.0 * PI * SAMPLE_FREQ / FRAME_SIZE; // 将位移谱转为速度谱，谱上每一点乘以2*PI*该点频率
        sum += pow(app.sample[ADDR_ACC + i], 2);
    }
    app.acc_Overall = sum;
    app.acc_Overall = sqrt(app.acc_Overall * 8.0 / 3.0) / 2 / 2 / sqrt(2.0); // 修正，并转为有效均方根值
    app.acc_Overall = app.acc_Overall / 1000 / 1000 / 9.81;  // 单位转换
    app.acc_Overall_gs = digit2micron(app.acc_Overall);

    log_info("DSP PkPk Total\t%f = %f Micons\n", app.dsp_Overall, app.dsp_Overall_um);
    log_info("VEL Peak Total\t%f = %f mm/Sec\n", app.vel_Overall, app.vel_Overall_mmps);
    log_info("ACC Peak Total\t%f = %f G-s\n", app.acc_Overall, app.acc_Overall_gs);

    log_info("-------------------------------------------------------------------------------------------------------\n");
    log_info("Power Spectrum (%d lines)\n\n", app.lines);

    // 转为功率谱
    for (i = 0; i < app.lines; i++) {
        app.sample[ADDR_POWER + i] = pow(app.sample[ADDR_FFT + i], 2);
    }

    // 输出功率谱
    for (j = 0; j < app.lines; j++) {
        log_info("%f ", app.sample[ADDR_POWER + j]);
    }

    log_info("\n-------------------------------------------------------------------------------------------------------\n");

    // 确定真实转速
    app.spd_ref_hz = app.config.ref_speed_rpm / 60.0;
    app.spd_ref_line = app.spd_ref_hz * FRAME_SIZE / SAMPLE_FREQ;
    
    find_running_speed_true(&app.sample[ADDR_POWER], app.lines, app.spd_ref_line, &app.spd_true_line);

    app.spd_true_hz = app.spd_true_line * SAMPLE_FREQ / FRAME_SIZE;
    app.spd_true_rpm = app.spd_true_hz * 60;

    log_info("-------------------------------------------------------------------------------------------------------\n");

    // 如果能确定真实转速，计算频谱参数
    if (app.spd_true_line > 0.0) {
        calculate_spectral_parameters(&app.sample[ADDR_POWER], app.lines, app.spd_true_line, &app.overall_pkpk, app.para_nonsyn_value, app.para_harmonic_value);
    }

    // 真峰峰值
    find_true_pkpk(&app.sample[ADDR_RAW], SAMPLE_SIZE, app.spd_true_line, &app.true_pkpk_digit);

    // 在时域波形中，取完整周期的最大整数倍

    // 计算分布，需要再考虑一下这里，再讨论

    // 封装成 wave 文件格式

    return 0;
}

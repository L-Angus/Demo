#ifndef DSP_SPECTRUM_HPP
#define DSP_SPECTRUM_HPP

#include <cmath>
#include <complex>
#include <vector>

#include "window.h"
#include "utility.hpp"

enum class OutputMode
{
    DB,
    VOLT,
    PWR
};

// 策略基类
template <OutputMode Mode>
struct OutputModePolicy;

// 分贝输出策略
template <>
struct OutputModePolicy<OutputMode::DB>
{
    static DOUBLE convert(DOUBLE value)
    {
        return 20 * log10(value);
    }

    static DOUBLE compute_scale_factor(const ARRAY_D &R, DOUBLE std_level)
    {
        if (std_level == 0.0)
        {
            double maxAmplitude = *std::max_element(R.begin(), R.end());
            return 1.0 / maxAmplitude;
        }

        return 1 / std_level;
    }
};

// 电压输出策略
template <>
struct OutputModePolicy<OutputMode::VOLT>
{
    static DOUBLE convert(DOUBLE value)
    {
        return value;
    }

    static DOUBLE compute_scale_factor(const ARRAY_D &R, DOUBLE std_level)
    {
        if (std_level == 0.0)
        {
            double maxAmplitude = *std::max_element(R.begin(), R.end());
            return 1.0 / maxAmplitude;
        }
        return 1 / std_level;
    }
};

// 功率输出策略
template <>
struct OutputModePolicy<OutputMode::PWR>
{
    static DOUBLE convert(DOUBLE value)
    {
        return std::pow(std::abs(value), 2);
    }

    static double compute_scale_factor(const std::vector<double> &R, double std_level)
    {
        if (std_level == 0.0)
        {
            double maxPower = *std::max_element(R.begin(), R.end(), [](double a, double b)
                                                { return a * a < b * b; });
            return 1.0 / (maxPower * maxPower);
        }
        return 1 / std_level;
    }
};

// DSP_SPECTRUM函数模板
template <typename T>
void DSP_SPECTRUM(T *src, DOUBLE *result, size_t N, OutputMode output_mode, DOUBLE std_level, WindowType window, DOUBLE *filter)
{
    // apply window
    apply_window_to_signal(src, N, window);
    // perform fft
    ARRAY_COMPLEX out(N);
    DoDFT<T>(src, out.data(), N);
    // // perform normalizations
    DOUBLE dcComponent = std::abs(out[0] / static_cast<DOUBLE>(N));
    if (!filter)
    {
        filter = new DOUBLE[N];
        std::fill(filter, filter + N, 1.0);
    }

    ARRAY_D R(N / 2);
    for (size_t j = 1; j < N / 2; ++j)
    {
        COMPLEX rtemp = out[j] * 2.0 / static_cast<DOUBLE>(N);
        DOUBLE magnitude = std::abs(rtemp);
        R[j] = magnitude * filter[j];
    }
    R[0] = dcComponent * filter[0]; // for j = 0;

    // compute scale factor
    DOUBLE scaleFactor = 1.0;
    switch (output_mode)
    {
    case OutputMode::DB:
        scaleFactor = OutputModePolicy<OutputMode::DB>::compute_scale_factor(R, std_level);
        break;
    case OutputMode::VOLT:
        scaleFactor = OutputModePolicy<OutputMode::VOLT>::compute_scale_factor(R, std_level);
        break;
    case OutputMode::PWR:
        scaleFactor = OutputModePolicy<OutputMode::PWR>::compute_scale_factor(R, std_level);
        break;
    default:
        throw std::runtime_error("Unsupported Output Mode.");
    }

    // compute result
    for (size_t j = 0; j < R.size(); ++j)
    {
        double scaledValue = R[j] * scaleFactor;
        if (output_mode == OutputMode::DB)
        {
            result[j] = (j == 0) ? scaledValue : OutputModePolicy<OutputMode::DB>::convert(scaledValue);
        }
        else if (output_mode == OutputMode::PWR)
        {
            result[j] = OutputModePolicy<OutputMode::PWR>::convert(scaledValue);
        }
        else
        {
            result[j] = OutputModePolicy<OutputMode::VOLT>::convert(scaledValue);
        }
    }
}

#endif // DSP_SPECTRUM_HPP

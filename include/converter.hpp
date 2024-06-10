#ifndef DSP_CONVERTER_HPP
#define DSP_CONVERTER_HPP

#include "types.h"
#include "utility.hpp"

constexpr int N = 4;

template <LSBUsage usage>
struct LinearErrorPolicy
{
};

template <>
struct LinearErrorPolicy<LSBUsage::LSB>
{
    static void CalculateDLE(const ARRAY_D &V, ARRAY_D &dle, DOUBLE DeviceLSB)
    {
        const size_t size = V.size() - 2;
        dle.resize(size);
        for (size_t i = 0; i < size - 1; i++)
        {
            dle[i] = (V[i + 1] - V[i]) / DeviceLSB - 1;
        }
    }
    static void CalculateILE(const ARRAY_D &V, ARRAY_D &ile, DOUBLE DeviceLSB, DOUBLE Vref)
    {
        const size_t size = V.size();
        ile.resize(size);
        for (size_t i = 0; i < size - 1; i++)
        {
            ile[i] = (V[i] - (DeviceLSB * i + Vref)) / DeviceLSB;
        }
    }
};

template <>
struct LinearErrorPolicy<LSBUsage::NO_LSB>
{
    static void CalculateDLE(const ARRAY_D &V, ARRAY_D &dle, DOUBLE DeviceLSB)
    {
        const size_t size = V.size() - 2;
        dle.resize(size);
        for (size_t i = 0; i < size - 1; i++)
        {
            dle[i] = (V[i + 1] - V[i]) - DeviceLSB;
        }
    }
    static void CalculateILE(const ARRAY_D &V, ARRAY_D &ile, DOUBLE DeviceLSB, DOUBLE Vref)
    {
        const size_t size = V.size();
        ile.resize(size);
        for (size_t i = 0; i < size - 1; i++)
        {
            ile[i] = V[i] - (DeviceLSB * i + Vref);
        }
    }
};

template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value, T>::type
CalA(const ARRAY_x<T> &V)
{
    const size_t ns = V.size();
    T A = 0;
    for (size_t i = 0; i < ns; ++i)
    {
        A += V[i] * static_cast<T>(i);
    }
    return A;
}

template <typename T>
typename std::enable_if<!std::is_arithmetic<T>::value, T>::type
CalA(const ARRAY_x<T> &V) = delete;

template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value, T>::type
CalB(const ARRAY_x<T> &V)
{
    const size_t ns = V.size();
    T B = 0;
    for (size_t i = 0; i < ns; ++i)
    {
        B += V[i];
    }
    return B;
}
template <typename T>
typename std::enable_if<!std::is_arithmetic<T>::value, T>::type
CalB(const ARRAY_x<T> &V) = delete;

template <ILEMethod Method>
struct CalDeviceLsbAndVref
{
};

template <>
struct CalDeviceLsbAndVref<ILEMethod::END_PT_ILE>
{
    static double DeviceLSB(const ARRAY_D &V)
    {
        if (V.empty())
            throw std::runtime_error("Invalid ADC/DAC array.");
        const size_t ns = V.size();
        return (V[ns - 1] - V[0]) / (ns - 1);
    }

    static double Vref(const ARRAY_D &V)
    {
        if (V.empty())
            throw std::runtime_error("Invalid ADC/DAC array.");
        return V[0];
    }
};

template <>
struct CalDeviceLsbAndVref<ILEMethod::ZERO_PT_ILE>
{
    static double DeviceLSB(const ARRAY_D &V)
    {
        if (V.empty())
            throw std::runtime_error("Invalid ADC/DAC array.");
        const size_t ns = V.size();
        auto A = CalA(V);
        auto B = CalB(V);
        auto Numerator = (A + ns / 2 * ns * V[ns / 2] - ns / 2 * B - V[ns / 2] * (ns - 1) * ns / 2);
        auto Denominator = std::pow(ns / 2, 2) * ns - (std::pow(ns, 2) * (ns - 1) / 2) + ns * (ns - 1) * (2 * ns - 1) / 6;
        return Numerator / Denominator;
    }

    static double Vref(const ARRAY_D &V)
    {
        if (V.empty())
            throw std::runtime_error("Invalid ADC/DAC array.");
        const size_t ns = V.size();
        auto deviceLSB = DeviceLSB(V);
        return V[ns / 2] - deviceLSB * ns / 2;
    }
};

template <>
struct CalDeviceLsbAndVref<ILEMethod::MIN_RMS_ILE>
{
    static double DeviceLSB(const ARRAY_D &V)
    {
        if (V.empty())
            throw std::runtime_error("Invalid ADC/DAC array.");
        const size_t ns = V.size();
        auto A = CalA(V);
        auto B = CalB(V);
        auto Numerator = ns * A - (B * ns * (ns - 1) / 2);
        auto Denominator = std::pow(ns, 2) * ns * (2 * ns - 1) / 6 - std::pow((ns - 1) * ns / 2, 2);
        return Numerator / Denominator;
    }

    static double Vref(const ARRAY_D &V)
    {
        if (V.empty())
            throw std::runtime_error("Invalid ADC/DAC array.");
        const size_t ns = V.size();
        auto B = CalB(V);
        auto deviceLSB = DeviceLSB(V);
        return (B - (deviceLSB * ns * (ns - 1) / 2)) / ns;
    }
};

inline bool IsLSB(ILEType type)
{
    return type == ILEType::END_PT_LSB || type == ILEType::MIN_RMS_LSB || type == ILEType::ZERO_PT_LSB;
}

void DSP_LINEAR(ARRAY_D &src, LINERtype *result, ARRAY_D &dle, ARRAY_D &ile, ILEType ile_type)
{
    // compute bit width
    if (src.empty())
        throw std::runtime_error("Source array is empty.");
    // auto N = CalculateBitWidth(src.begin(), src.end());
    // std::cout << "bit width: " << N << std::endl;

    double device_lsb, vref;
    switch (ile_type)
    {
    case ILEType::END_PT:
    case ILEType::END_PT_LSB:
        device_lsb = CalDeviceLsbAndVref<ILEMethod::END_PT_ILE>::DeviceLSB(src);
        vref = CalDeviceLsbAndVref<ILEMethod::END_PT_ILE>::Vref(src);
        break;
    case ILEType::ZERO_PT:
    case ILEType::ZERO_PT_LSB:
        device_lsb = CalDeviceLsbAndVref<ILEMethod::ZERO_PT_ILE>::DeviceLSB(src);
        vref = CalDeviceLsbAndVref<ILEMethod::ZERO_PT_ILE>::Vref(src);
        break;
    case ILEType::MIN_RMS:
    case ILEType::MIN_RMS_LSB:
        device_lsb = CalDeviceLsbAndVref<ILEMethod::MIN_RMS_ILE>::DeviceLSB(src);
        vref = CalDeviceLsbAndVref<ILEMethod::MIN_RMS_ILE>::Vref(src);
        break;
    default:
        break;
    }
    std::cout << device_lsb << "<><>" << vref << std::endl;
    IsLSB(ile_type) ? LinearErrorPolicy<LSBUsage::LSB>::CalculateDLE(src, dle, device_lsb) : LinearErrorPolicy<LSBUsage::NO_LSB>::CalculateDLE(src, dle, device_lsb);
    IsLSB(ile_type) ? LinearErrorPolicy<LSBUsage::LSB>::CalculateILE(src, ile, device_lsb, 3.5) : LinearErrorPolicy<LSBUsage::NO_LSB>::CalculateILE(src, ile, device_lsb, 3.5);

    ARRAY_D abs_dle, abs_ile;
    DoAbs<double>(dle, abs_dle);
    DoAbs<double>(ile, abs_ile);

    DoMinMax<double>(dle, &result->dmin, &result->dmax, &result->dmin_index, &result->dmax_index);
    DoMinMax<double>(ile, &result->imin, &result->imax, &result->imin_index, &result->imax_index);
    double abs_dle_min, abs_ile_min;
    int abs_dle_min_index, abs_ile_min_index;
    DoMinMax<double>(abs_dle, &abs_dle_min, &result->damax, &abs_dle_min_index, &result->damax_index);
    DoMinMax<double>(abs_ile, &abs_ile_min, &result->iamax, &abs_ile_min_index, &result->iamax_index);
}

#endif // DSP_CONVERTER_HPP
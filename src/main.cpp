#include <complex>
#include <iostream>
#include <chrono>
#include "fftw3.h"
#include "FFT_impl.hpp"
#include "fft_engine.hpp"
#include "Spectrum.hpp"
#include "converter.hpp"

#include <random>

// 生成包含N个复数元素的样本数据，实部不变，虚部为随机值
std::vector<COMPLEX> generateRandomizedComplexSampleData(size_t N)
{
    std::vector<COMPLEX> data(N);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);

    for (size_t i = 0; i < N; ++i)
    {
        data[i].real(static_cast<double>(i));
        data[i].imag(dis(gen));
    }
    return data;
}

std::vector<double> generateSampleData(size_t N)
{
    std::vector<double> data(N);
    for (size_t i = 0; i < N; ++i)
    {
        data[i] = static_cast<double>(i);
    }
    return data;
}

template <typename T>
constexpr T square(T x)
{
    return x * x;
}

int main()
{
    std::cout << "Hello, World!" << std::endl;
    const size_t N = 18;
    auto data = generateSampleData(N);
    for (auto &d : data)
    {
        std::cout << d << " ";
    }
    std::cout << std::endl;
    // auto complex_data = generateRandomizedComplexSampleData(N);
    // 写一个10个复数的vector数组
    ARRAY_COMPLEX complex_data = {
        {1.0, 1.0},
        {2.0, 2.0},
        {3.0, 3.0},
        {4.0, 4.0},
        {5.0, 5.0},
        {6.0, 6.0},
        {7.0, 7.0},
        {8.0, 8.0},
        {9.0, 9.0},
        {10.0, 10.0}};

    ARRAY_COMPLEX result(N);
    fft_engine<double> fft_real(N);
    fft_engine<COMPLEX> fft_complex_engine(N);
    DOUBLE spectrum[N];
    DOUBLE filter[N];
    ARRAY_D dle(N), ile(N);
    LINERtype ile_result;

    auto start = std::chrono::high_resolution_clock::now();
    // fft_complex_engine.dft(complex_data, result);
    // DSP_SPECTRUM(data.data(), spectrum, N, OutputMode::PWR, 0.0, WindowType::Rectangular, NULL);

    DSP_LINEAR(data, &ile_result, dle, ile, ILEType::END_PT);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Time taken by function: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms" << std::endl;

    std::cout << "Has process " << result.size() << " elements" << std::endl;
    for (size_t i = 0; i < dle.size(); i++)
    {
        std::cout << "dle[i] = " << dle[i] << std::endl;
    }
    std::cout << "------------------------------" << std::endl;
    for (size_t i = 0; i < ile.size(); i++)
    {
        std::cout << "ile[i] = " << ile[i] << std::endl;
    }
    std::cout << "------------------------------" << std::endl;
    std::cout << ile_result.dmin << "<><>" << ile_result.dmin_index << "<><>" << ile_result.dmax << "<><>" << ile_result.dmax_index << "\n";
    std::cout << ile_result.imin << "**" << ile_result.imin_index << "**" << ile_result.imax << "**" << ile_result.imax_index << "\n";
    std::cout << ile_result.damax << "~" << ile_result.damax_index << std::endl;
    std::cout << ile_result.iamax << "~" << ile_result.iamax_index << std::endl;
    return 0;
}

#include <complex>
#include <iostream>
#include <chrono>
#include "fftw3.h"
#include "FFT_impl.hpp"

#include <random>

// 生成包含N个复数元素的样本数据，实部不变，虚部为随机值
std::vector<COMPLEX> generateRandomizedComplexSampleData(size_t N) {
    std::vector<COMPLEX> data(N);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);

    for (size_t i = 0; i < N; ++i) {
        data[i].real(static_cast<double>(i));
        data[i].imag(dis(gen));
    }
    return data;
}

std::vector<double> generateSampleData(size_t N){
    std::vector<double> data(N);
    for (size_t i = 0; i < N; ++i)
    {
        data[i] = static_cast<double>(i); 
    }
    return data;
}


int main() {
    std::cout << "Hello, World!" << std::endl;
    const size_t N = 10;
    auto data = generateSampleData(N);
    // auto complex_data = generateRandomizedComplexSampleData(N);
    // 写一个10个复数的vector数组
    ARRAY_COMPLEX complex_data = {
        {1.0,1.0},
        {2.0,2.0},
        {3.0,3.0},
        {4.0,4.0},
        {5.0,5.0},
        {6.0,6.0},
        {7.0,7.0},
        {8.0,8.0},
        {9.0,9.0},
        {10.0,10.0}
    };


    ARRAY_COMPLEX result(N);

    fft::FFT_impl<double> fft_engine(N);
    fft::FFT_impl<COMPLEX> fft_complex_engine(N);
    auto start = std::chrono::high_resolution_clock::now();
    fft_complex_engine.DoDFT(complex_data, result);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Time taken by function: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms" << std::endl;

    std::cout << "Has process "<< result.size() << " elements" << std::endl;
    for (size_t i = 0; i < N; i++)
    {
        std::cout << result[i].real() << "+" << result[i].imag()<< "i" << std::endl;
    }
    

    return 0;
}

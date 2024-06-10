#ifndef _FFT_ENGINE_HPP_
#define _FFT_ENGINE_HPP_

#include "FFT_impl.hpp"

template <typename T>
class fft_engine
{
public:
    using value_type = T;
    using complex_type = COMPLEX;
    using size_type = std::size_t;

    explicit fft_engine(size_type nfft) : m_engine(nfft) {}
    ~fft_engine() = default;

    void dft(const value_type *src, COMPLEX *result)
    {
        m_engine.DoDFT(src, result);
    }

    void dft(const ARRAY_x<value_type> &src, ARRAY_x<complex_type> &dst)
    {
        m_engine.DoDFT(src, dst);
    }

private:
    fft::FFT_impl<T> m_engine;
};
#endif // _FFT_ENGINE_HPP_
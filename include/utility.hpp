#ifndef DSP_UTILITY_HPP
#define DSP_UTILITY_HPP

#include <type_traits>
#include <algorithm>
#include "fft_engine.hpp"

namespace internal
{
    template <typename T>
    struct is_complext_type
    {
        static constexpr bool value = std::is_same_v<T, COMPLEX>;
    };

    template <typename T>
    constexpr bool isComplex()
    {
        return is_complext_type<T>::value;
    }

};

template <typename InputIt, typename OutputIt>
void MakeFFT(InputIt first, InputIt last, OutputIt o_first)
{
    using input_t = typename std::iterator_traits<InputIt>::value_type;
    const auto nfft = std::distance(first, last);
    fft_engine<input_t> plan(static_cast<typename fft_engine<input_t>::size_type>(nfft));
    plan.dft(&(*first), &(*o_first));
}

template <typename T>
void DoDFT(const ARRAY_x<T> &src, ARRAY_COMPLEX &result)
{
    ARRAY_COMPLEX out(src.size());
    MakeFFT(src.begin(), src.end(), out.begin());
    const size_t N = internal::isComplex<T>() ? src.size() : src.size() / 2;
    result.resize(N);
    for (size_t i = 0; i < N; ++i)
    {
        result[i] = out[i];
    }
}

template <typename T>
void DoDFT(T *src, COMPLEX *result, size_t size)
{
    ARRAY_COMPLEX out(size);
    MakeFFT(src, src + size, out.data());
    const size_t N = internal::isComplex<T>() ? size : size / 2;
    for (size_t i = 0; i < N; ++i)
    {
        result[i] = out[i];
    }
}

template <typename It>
using value_type_t = typename std::iterator_traits<It>::value_type;

template <typename T, typename R = double>
void DoAbs(const T *src, R *result, size_t size)
{
    std::transform(src, src + size, result, [](const T &val)
                   { return std::abs(val); });
}

template <typename T, typename R = double>
void DoAbs(const ARRAY_x<T> &src, ARRAY_x<R> &result)
{
    result.resize(src.size());
    DoAbs(src.data(), result.data(), src.size());
}

template <typename T>
struct DefaultCompare
{
    bool operator()(const T &a, const T &b) const { return a < b; }
};

template <>
struct DefaultCompare<COMPLEX>
{
    bool operator()(const COMPLEX &a, const COMPLEX &b) const
    {
        return std::abs(a) < std::abs(b);
    }
};

template <typename T>
struct MinMaxResult
{
    T min_val;
    T max_val;
    ptrdiff_t min_index;
    ptrdiff_t max_index;
};

template <typename ForwardIt, typename Compare = DefaultCompare<value_type_t<ForwardIt>>>
MinMaxResult<value_type_t<ForwardIt>> MinMaxImpl(ForwardIt first, ForwardIt last, Compare comp = Compare{})
{
    using input_t = value_type_t<ForwardIt>;
    MinMaxResult<input_t> result{std::numeric_limits<input_t>::max(), std::numeric_limits<input_t>::min(), -1, -1};
    if (first != last)
    {
        auto minmax_iterators = std::minmax_element(first, last, comp);
        result.min_index = std::distance(first, minmax_iterators.first);
        result.max_index = std::distance(first, minmax_iterators.second);
        result.min_val = *minmax_iterators.first;
        result.max_val = *minmax_iterators.second;
    }
    return result;
}

template <typename T>
void DoMinMax(const ARRAY_x<T> &src, T *min, T *max, INT *min_index, INT *max_index)
{
    auto result = MinMaxImpl(src.begin(), src.end());
    *min = result.min_val;
    *max = result.max_val;
    *min_index = result.min_index;
    *max_index = result.max_index;
}

template <typename T>
void DoMinMax(T *src, T *min, T *max, INT *min_index, INT *max_index, size_t size)
{
    DoMinMax(ARRAY_x<T>(src, src + size), min, max, min_index, max_index);
}

template <typename ForwardIt>
constexpr int CalculateBitWidth(ForwardIt first, ForwardIt last)
{
    using input_t = value_type_t<ForwardIt>;
    const size_t size = std::distance(first, last);
    if (size == 0)
    {
        throw std::invalid_argument("Source vector is empty.");
    }

    int bit_width = 0;
    size_t curret_value = size;
    while (curret_value > 1)
    {
        curret_value >>= 1; // 位移操作
        ++bit_width;
    }

    // 如果采样数不是2的幂，增加一个比特宽度
    if ((size & (size - 1)) != 0)
    {
        ++bit_width;
    }

    return bit_width;
}

#endif // DSP_UTILITY_HPP
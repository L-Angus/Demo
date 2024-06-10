// #include "blackman.hpp"
// #include "flattop.hpp"
// #include "hamming.hpp"
// #include "hanning.hpp"
// #include "rectangular.hpp"

#include "types.h"
#include <unordered_map>
#include <functional>

namespace window
{
    template <typename OutputIt>
    constexpr void rectangular(OutputIt first, OutputIt last) {}

    template <typename OutputIt>
    constexpr void hanning(OutputIt first, OutputIt last) {}

    template <typename OutputIt>
    constexpr void hamming(OutputIt first, OutputIt last) {}

    template <typename OutputIt>
    constexpr void blackman(OutputIt first, OutputIt last) {}

    template <typename OutputIt>
    constexpr void flattop(OutputIt first, OutputIt last) {}
};

namespace internal
{

    template <WindowType Type>
    struct _build_window
    {
    };

    template <>
    struct _build_window<WindowType::Blackman>
    {
        template <typename OutputIt>
        constexpr void operator()(OutputIt first, OutputIt last)
        {
            window::blackman(first, last);
        }
    };

    template <>
    struct _build_window<WindowType::FlatTop>
    {
        template <typename OutputIt>
        constexpr void operator()(OutputIt first, OutputIt last)
        {
            window::flattop(first, last);
        }
    };

    template <>
    struct _build_window<WindowType::Hamming>
    {
        template <typename OutputIt>
        constexpr void operator()(OutputIt first, OutputIt last)
        {
            window::hamming(first, last);
        }
    };

    template <>
    struct _build_window<WindowType::Hanning>
    {
        template <typename OutputIt>
        constexpr void operator()(OutputIt first, OutputIt last)
        {
            window::hanning(first, last);
        }
    };

    template <>
    struct _build_window<WindowType::Rectangular>
    {
        template <typename OutputIt>
        constexpr void operator()(OutputIt first, OutputIt last)
        {
            window::rectangular(first, last);
        }
    };
}; // namespace internal

/**
 * @brief Computes a window of the given type and length N and stores the result in the range, beginning at d_first.
 * @tparam Type Type of window to be computed
 * @param first Input iterator defining the beginning of the output range.
 * @param last Input iterator defining the ending of the output range.
 */
template <WindowType Type, typename OutputIt>
constexpr void make_window(OutputIt first, OutputIt last)
{
    return internal::_build_window<Type>{}(first, last);
}

template <typename OutputIt>
void generate_window(OutputIt first, OutputIt last, WindowType window)
{
    static std::unordered_map<WindowType, std::function<void(OutputIt, OutputIt)>> window_map = {
        {WindowType::Rectangular, [&](OutputIt first, OutputIt last)
         { make_window<WindowType::Rectangular>(first, last); }},
        {WindowType::Hanning, [&](OutputIt first, OutputIt last)
         { make_window<WindowType::Hanning>(first, last); }},
        {WindowType::Hamming, [&](OutputIt first, OutputIt last)
         { make_window<WindowType::Hamming>(first, last); }},
        {WindowType::Blackman, [&](OutputIt first, OutputIt last)
         { make_window<WindowType::Blackman>(first, last); }},
        {WindowType::FlatTop, [&](OutputIt first, OutputIt last)
         { make_window<WindowType::FlatTop>(first, last); }}};
    auto hit = window_map.find(window);
    if (hit == window_map.end())
    {
        throw std::runtime_error("Not Find Such Window Function.");
    }
    hit->second(first, last);
}

template <typename Signal>
void apply_window_to_signal(Signal *signal, size_t N, WindowType window)
{
    ARRAY_D windows(N);
    generate_window(windows.begin(), windows.end(), window);
    for (size_t i = 0; i < N; ++i)
    {
        signal[i] *= windows[i];
    }
}

template <typename Signal>
void apply_window_to_signal(ARRAY_x<Signal> &signal, WindowType window)
{
    apply_window_to_signal(signal.data(), signal.size(), window);
}
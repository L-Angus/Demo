#include <complex>
#include <vector>

#include "fftw3.h"

template<typename T>
using ARRAY_x = std::vector<T>;

using COMPLEX = std::complex<double>;
using ARRAY_I = ARRAY_x<int>;
using ARRAY_D = ARRAY_x<double>;
using ARRAY_COMPLEX = ARRAY_x<COMPLEX>;

typedef int INT;
typedef double DOUBLE;

// #include "window.h"

// enum class WindowType {
//     Blackman = 0,        /*!< Blackman Window */
//     FlatTop,         /*!< FlatTop Window */
//     Hamming,         /*!< Hamming Window */
//     Hanning,         /*!< Hanning Window */
//     Rectangular     /*!< Rectangular Window */
// };


//     namespace internal {

//         template <WindowType Type>
//         struct _build_window {};

//         template <>
//         struct _build_window<WindowType::Blackman> {
//             template <typename OutputIt>
//             constexpr void operator()(OutputIt first, OutputIt last) {
//                 blackman(first, last);
//             }
//         };

//         template <>
//         struct _build_window<WindowType::FlatTop> {
//             template <typename OutputIt>
//             constexpr void operator()(OutputIt first, OutputIt last) {
//                 flattop(first, last);
//             }
//         };

//         template <>
//         struct _build_window<WindowType::Hamming> {
//             template <typename OutputIt>
//             constexpr void operator()(OutputIt first, OutputIt last) {
//                 hamming(first, last);
//             }
//         };

//         template <>
//         struct _build_window<WindowType::Hanning> {
//             template <typename OutputIt>
//             constexpr void operator()(OutputIt first, OutputIt last) {
//                 hanning(first, last);
//             }
//         };

//         template <>
//         struct _build_window<WindowType::Rectangular> {
//             template <typename OutputIt>
//             constexpr void operator()(OutputIt first, OutputIt last) {
//                 rectangular(first, last);
//             }
//         };
//     }; // namespace internal

//     /**
//      * @brief Computes a window of the given type and length N and stores the result in the range, beginning at d_first.
//      * @tparam Type Type of window to be computed
//      * @param first Input iterator defining the beginning of the output range.
//      * @param last Input iterator defining the ending of the output range.
//      */
//     template <WindowType Type, typename OutputIt>
//     constexpr void make_window(OutputIt first, OutputIt last) {
//         return internal::_build_window<Type>{}(first, last);
//     }


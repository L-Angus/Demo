#ifndef DSP_TYPES_H
#define DSP_TYPES_H

#include <complex>
#include <vector>

#include "fftw3.h"

template <typename T>
using ARRAY_x = std::vector<T>;

using COMPLEX = std::complex<double>;
using ARRAY_I = ARRAY_x<int>;
using ARRAY_D = ARRAY_x<double>;
using ARRAY_COMPLEX = ARRAY_x<COMPLEX>;

typedef int INT;
typedef double DOUBLE;

enum class WindowType
{
    Blackman = 0, /*!< Blackman Window */
    FlatTop,      /*!< FlatTop Window */
    Hamming,      /*!< Hamming Window */
    Hanning,      /*!< Hanning Window */
    Rectangular   /*!< Rectangular Window */
};

struct LINERtype
{
    double dmax;
    double dmin;
    double damax;
    double imax;
    double imin;
    double iamax;

    int dmax_index;
    int dmin_index;
    int damax_index;
    int imax_index;
    int imin_index;
    int iamax_index;
};

enum class ILEType
{
    END_PT_LSB,
    END_PT,
    ZERO_PT_LSB,
    ZERO_PT,
    MIN_RMS_LSB,
    MIN_RMS
};

enum class LSBUsage
{
    LSB,
    NO_LSB
};

enum class ILEMethod
{
    END_PT_ILE,
    ZERO_PT_ILE,
    MIN_RMS_ILE
};

#endif // DSP_TYPES_H
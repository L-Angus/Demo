#include "fftw3.h"
#include "types.h"

namespace FFT{
    void DSP_FFT(ARRAY_I& src, ARRAY_COMPLEX& result, WindowType window){
        
    }
    void DSP_FFT(INT *src, COMPLEX *result, INT size, WindowType window){}
    void DSP_FFT(ARRAY_D& src, ARRAY_COMPLEX& result, WindowType window){}
    void DSP_FFT(DOUBLE *src, COMPLEX *result, INT size, WindowType window){}
    void DSP_FFT(ARRAY_COMPLEX& src, ARRAY_COMPLEX& result, WindowType window){}
    void DSP_FFT(COMPLEX *src, COMPLEX *result, INT size, WindowType window){}
};
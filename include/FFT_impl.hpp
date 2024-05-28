#ifndef FFT_IMPL_HPP
#define FFT_IMPL_HPP

#include "fftw3.h"
#include "types.h"

namespace fft{

    namespace internal{
        template<typename T>
        inline T* fftw_cast(const T* p) noexcept {
            return const_cast<T*>(p);
        }
        inline fftw_complex* fftw_cast(const COMPLEX* p) {
            return const_cast<fftw_complex*>(reinterpret_cast<const fftw_complex*>(p));
        }

        inline fftw_complex* fftw_cast(const DOUBLE* p, size_t size) {
            fftw_complex * input = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*size);
            for(size_t i = 0; i < size; ++i){
                input[i][0] = i;
                input[i][1] = 0.0;
            }
            return input;
        }
    };

    template<typename T>
    struct FFT_impl{};

    template<>
    struct FFT_impl<DOUBLE>{
        using value_type = DOUBLE;
        using complex_type = COMPLEX;
        using size_type = size_t;
        explicit FFT_impl(size_t ndft) : ndft_(ndft){}
        ~FFT_impl(){
            if(plan_){
                fftw_destroy_plan(plan_);
            }
        }
        inline void DoDFT(const value_type* src, complex_type* dst){
            if(!plan_){
                plan_ = fftw_plan_dft_1d(ndft_, internal::fftw_cast(src, ndft_), internal::fftw_cast(dst), FFTW_FORWARD,FFTW_ESTIMATE);
            }
            fftw_execute_dft(plan_, internal::fftw_cast(src, ndft_), internal::fftw_cast(dst));
        }

        inline void DoDFT(const ARRAY_x<value_type>& src, complex_type * dst){
            DoDFT(src.data(), dst);
        }

        inline void DoDFT(const value_type*src, ARRAY_x<complex_type>& dst){
            DoDFT(src, dst.data());      
        }

        inline void DoDFT(const ARRAY_x<value_type>& src, ARRAY_x<complex_type>& dst){
            DoDFT(src.data(), dst.data());      
        }

        private:
            size_t ndft_;
            fftw_plan plan_ = nullptr;
    };

    template<>
    struct FFT_impl<COMPLEX>{
        using value_type = COMPLEX;
        using complex_type = value_type;
        using size_type = size_t;

        explicit FFT_impl(size_t ndft) : ndft_(ndft){}
        ~FFT_impl(){
            if(plan_){
                fftw_destroy_plan(plan_);
            }
        }

        inline void DoDFT(const value_type* src, complex_type* dst){
            if(!plan_){
                plan_ = fftw_plan_dft_1d(ndft_, internal::fftw_cast(src), internal::fftw_cast(dst), FFTW_FORWARD, FFTW_ESTIMATE);
            }
            fftw_execute_dft(plan_, internal::fftw_cast(src), internal::fftw_cast(dst));
        }
        
        inline void DoDFT(const value_type* src, ARRAY_x<complex_type>& dst){
           return DoDFT(src, dst.data());
        }
        inline void DoDFT(const ARRAY_x<value_type>& src, complex_type* dst){
           return DoDFT(src.data(), dst);
        }
        inline void DoDFT(const ARRAY_x<value_type>& src, ARRAY_x<complex_type>& dst){
           return DoDFT(src.data(), dst.data());
        }

        private:
            size_t ndft_;
            fftw_plan plan_ = nullptr;
    };

};


#endif
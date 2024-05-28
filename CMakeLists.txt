cmake_minimum_required(VERSION 3.0)
project(fft)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(INCLUDE_DIR ./include $ENV{FFTW_INC})
link_directories(./lib $ENV{FFTW_LIB})
aux_source_directory(./src SRC)
add_executable(fft ${SRC})
target_include_directories(fft PUBLIC ${INCLUDE_DIR})
target_link_libraries(fft fftw3-3)



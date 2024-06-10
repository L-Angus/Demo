cmake_minimum_required(VERSION 3.20)
project(fft)
# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
# 包含目录
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
# 添加源文件目录
aux_source_directory(${CMAKE_SOURCE_DIR}/src SRC)
link_directories($ENV{FFTW_LIB})

# 指定可执行文件
add_executable(fft ${SRC})

# 添加包含目录
target_include_directories(fft PUBLIC ${INCLUDE_DIR} $ENV{FFTW_INC})
target_link_libraries(fft fftw3-3)

add_custom_command(TARGET fft POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/lib/libfftw3-3.dll"
    $<TARGET_FILE_DIR:fft>)

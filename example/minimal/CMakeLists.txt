cmake_minimum_required(VERSION 3.16)

project(ToastMinimalExample LANGUAGES CXX)

find_package(Toast CONFIG REQUIRED)

add_executable(ToastMinimalExample
    main.cpp
)

target_compile_features(ToastMinimalExample PRIVATE cxx_std_20)

target_link_libraries(ToastMinimalExample
    PRIVATE
        Toast::Toast
)

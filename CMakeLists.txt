cmake_minimum_required(VERSION 3.10)

project(gauss_solver C)

add_executable(gauss
    main.c
    gauss.c
    matr.c
    norma.c
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
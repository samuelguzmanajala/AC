cmake_minimum_required(VERSION 3.17)
project(pgnetica C)

set(CMAKE_C_STANDARD 11)



include_directories(.)

add_executable(pgnetica
        defineg.h
        fun.h
        fun_s.c
        gengrupos_s.c
        VersionParalela/gengrupos_p.c
        VersionParalela/fun_p.c)
target_link_libraries(pgnetica m)
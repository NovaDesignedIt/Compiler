cmake_minimum_required(VERSION 3.17)
project(Assignement_2_Compilers C)

set(CMAKE_C_STANDARD 99)

add_executable(Assignement_2_Compilers  evalParser.c parser.c scanner.c buffer.c parser.h table.h token.h buffer.h)
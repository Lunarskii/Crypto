cmake_minimum_required(VERSION 3.26)
project(crypto)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(crypto-des
        des/Des.cc
        des/Des.h
        des/des_cli.cc
        des/arrays.h
)

add_executable(crypto-huffman
        huffman/huffman_cli.cc
        huffman/Huffman.cc
        huffman/Huffman.h)

add_executable(tests
        des/Des.cc
        des/Des.h
        des/arrays.h
        huffman/Huffman.cc
        huffman/Huffman.h
        tests/tests.h
        tests/tests.cc
        rsa/generator.cc
        rsa/generator.h
        rsa/rsa.h
        rsa/rsa.cc)

add_executable(crypto-rsa
        rsa/generator.cc
        rsa/generator.h
        rsa/rsa.h
        rsa/rsa.cc)

target_link_libraries(crypto-rsa gmpxx gmp)

target_link_libraries(tests gtest gtest_main gmpxx gmp)

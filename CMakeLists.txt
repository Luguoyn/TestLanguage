cmake_minimum_required(VERSION 3.19)
project(TestLanguage)

set(CMAKE_CXX_STANDARD 11)

add_executable(TestLanguage main.cpp TestScan.cpp TestScan.h Data.cpp Data.h Util.cpp Util.h TestParse.cpp TestParse.h TestSemanticAnalysis.cpp TestSemanticAnalysis.h)
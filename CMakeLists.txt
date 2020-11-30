cmake_minimum_required(VERSION 3.17)
project(IFJ_FINAL C)

set(CMAKE_C_STANDARD 99)

add_executable(IFJ_FINAL main.c scanner.c scanner.h parser.c error.h symtable.c symtable.h parser.h)
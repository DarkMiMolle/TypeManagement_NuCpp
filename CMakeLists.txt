cmake_minimum_required(VERSION 3.21)
project(Vars2)

set(CMAKE_CXX_STANDARD 23)

add_executable(Vars2 main.cpp mystd.h NuType.h BuiltinTypes.h Var.h Typeof.h Any.h)

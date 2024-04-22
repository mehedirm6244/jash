cmake_minimum_required(VERSION 3.7)

project(
  jash
  VERSION 0.1.1
  LANGUAGES C
)

include_directories(./include)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lreadline")

add_executable(
  jash
  src/builtins/cd.c
  src/builtins/exit.c
  src/builtins/help.c
  src/builtins.c
  src/config.c
  src/jash.c
  src/loop.c
  src/main.c
)

set_property(TARGET jash PROPERTY C_STANDARD 11)

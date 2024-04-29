cmake_minimum_required(VERSION 3.7)

project(
  jash
  VERSION 0.1.1
  LANGUAGES C
)

include_directories(./include)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -lreadline")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -rdynamic")

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
    src/jstring.c
    src/readline.c
    src/trace.c
    src/utils.c
)

set_property(TARGET jash PROPERTY C_STANDARD 11)

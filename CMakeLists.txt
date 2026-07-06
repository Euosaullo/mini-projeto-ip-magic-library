cmake_minimum_required(VERSION 3.15)

project(
    MagicLibrary
    VERSION 1.0
    DESCRIPTION "Magic Library game backend"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(SOURCES
    main.c
    library.c
    files.c
    encryption.c
    utils.c
)

add_executable(
    MagicLibrary
    ${SOURCES}
)

target_include_directories(
    MagicLibrary
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(
    MagicLibrary
    PROPERTIES
    OUTPUT_NAME biblioteca
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

if(MSVC)
    target_compile_options(
        MagicLibrary
        PRIVATE
        /W4
    )
else()
    target_compile_options(
        MagicLibrary
        PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

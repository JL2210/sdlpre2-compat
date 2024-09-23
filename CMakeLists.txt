# Copyright (c) 2024 James R Larrowe
# SPDX-License-Identifier: Zlib

cmake_minimum_required(VERSION 3.6)
project(sdlpre2-compat)

set(CMAKE_C_STANDARD 23)
set(FIND_LIBRARY_USE_LIB32_PATHS ON)

add_compile_options(-m32 -pedantic
-Wall -Wextra -Winit-self -Wimplicit-fallthrough
-Wc++-compat -Wdouble-promotion -Wshadow -Wformat=2)
if(CMAKE_COMPILER_IS_GNUCC)
	add_compile_options(-Wabi=2 -Wrestrict)
endif()
add_link_options(-m32)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_library(ds_sdl2_shim MODULE)
set_target_properties(ds_sdl2_shim PROPERTIES PREFIX "")
target_sources(ds_sdl2_shim PRIVATE
	audit.c
	event-converter.c
)
target_compile_definitions(ds_sdl2_shim PRIVATE _GNU_SOURCE)

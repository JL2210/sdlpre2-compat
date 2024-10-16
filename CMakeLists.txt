# Copyright (c) 2024 James R Larrowe
# SPDX-License-Identifier: Zlib

cmake_minimum_required(VERSION 3.6)
project(sdlpre2-compat)
include(GNUInstallDirs)

set(CMAKE_C_STANDARD 23)

option(NO_BUILD_MULTILIB "only build one copy of library (for packagers)" OFF)

add_compile_options(-pedantic
	-Wall -Wextra -Winit-self -Wimplicit-fallthrough
	-Wc++-compat -Wdouble-promotion -Wshadow -Wformat=2
)
if(CMAKE_COMPILER_IS_GNUCC)
	add_compile_options(-Wabi=2 -Wrestrict)
endif()
add_compile_definitions(_GNU_SOURCE)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
set(SOURCES
	audit.c
	event-converter.c
	oldevents.h
	upstream.h
)

if(NOT NO_BUILD_MULTILIB)
	add_library(ds_sdl2_shim64 MODULE ${SOURCES})
	set_target_properties(ds_sdl2_shim64 PROPERTIES PREFIX "")

	add_library(ds_sdl2_shim32 MODULE ${SOURCES})
	set_target_properties(ds_sdl2_shim32 PROPERTIES PREFIX "")
	target_compile_options(ds_sdl2_shim32 PRIVATE -m32)
	target_link_options(ds_sdl2_shim32 PRIVATE -m32)
else()
	add_library(ds_sdl2_shim MODULE ${SOURCES})
	set_target_properties(ds_sdl2_shim PROPERTIES PREFIX "")
	install(TARGETS ds_sdl2_shim)
endif()

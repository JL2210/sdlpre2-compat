/*
 * Copyright (c) 2024 James R Larrowe
 * SPDX-License-Identifier: Zlib
 */

#include "oldevents.h"	// SDL_PollEvent_t, real_SDL_PollEvent
#include <assert.h>	// static assert, assert
#include <inttypes.h>	// PRI/X/N printf macros
#include <link.h>	// la_*
#include <stdio.h>	// fprintf, stderr
#include <string.h>	// GNU basename()

#if __ELF_NATIVE_CLASS == 32
# define la_symbind la_symbind32
#elif __ELF_NATIVE_CLASS == 64
# define la_symbind la_symbind64
#else // ???
# error __ELF_NATIVE_CLASS must be 32 or 64
#endif

// NB: Don't depend on this having side effects
#ifndef NDEBUG
# define fprintd(stream, ...) fprintf(stream, __VA_ARGS__)
#else
# define fprintd(stream, ...)
#endif

#define SDL2_SO_NAME		"libSDL2-2.0.so.0"
#define POLLEVENT_SYMNAME	"SDL_PollEvent"
static_assert(SDL2_SO_COOKIE != EXECUTABLE_COOKIE, "sdl2 and executable cookies must be different!");

unsigned int la_version(unsigned int version) {
	(void)version;
	// LAV_CURRENT guarantees abi compat, not api compat
	static_assert(LAV_CURRENT == 2, "Code intended to be used with LA_AUDIT version 2");
	fprintd(stderr, "la_version %u\n", version);
	return LAV_CURRENT;
}

void la_preinit(uintptr_t *cookie) {
	assert(*cookie == EXECUTABLE_COOKIE);
	fprintd(stderr, "la_preinit\n");
	(void)cookie;
}

char *la_objsearch(const char *name, uintptr_t *cookie,
		   unsigned int flag) {
	(void)cookie;
	if((flag == LA_SER_LIBPATH || flag == LA_SER_RUNPATH) &&
	   strcmp(basename(name), SDL2_SO_NAME) == 0) {
	        fprintd(stderr, "name: %s, flag: %s\n", name,
        	        (flag == LA_SER_ORIG    ?       "LA_SER_ORIG"           :
                	 flag == LA_SER_LIBPATH ?       "LA_SER_LIBPATH"        :
                	 flag == LA_SER_RUNPATH ?       "LA_SER_RUNPATH"        :
                	                                "Other"));
		return NULL;
	}

	return (char *)name;
}

unsigned int la_objopen(struct link_map *map, Lmid_t lmid,
			uintptr_t *cookie) {
	fprintd(stderr, "objopen %s\n", map->l_name);
	if(*map->l_name == '\0') {
		// l_name is the empty string, so this is the main executable
		*cookie = EXECUTABLE_COOKIE;
		fprintd(stderr, "main executable\n");
		return LA_FLG_BINDFROM;
	}

	if(lmid == LM_ID_BASE && strcmp(basename(map->l_name), SDL2_SO_NAME) == 0) {
		fprintd(stderr, "is sdl2\n");
		*cookie = SDL2_SO_COOKIE;
		return LA_FLG_BINDTO;
	}

	return 0;
}

/*
 * this function:
 * 1. stores a copy of the original SDL_PollEvent
 * 2. redirects the main executable's calls of SDL_PollEvent to shim_SDL_PollEvent
 * 3. allows all calls outside of the main executable to go to the real SDL_PollEvent
 * should be error-hardy for use in other games
 * doesn't break stuff that doesn't use SDL
 * maybe not thread-safe? Eh.
 */
SDL_PollEvent_t *real_SDL_PollEvent = NULL; // used by EventPre2to2
uintptr_t la_symbind(ElfW(Sym) *sym, unsigned int ndx,
                     uintptr_t *refcook, uintptr_t *defcook,
                     unsigned int *flags, const char *symname) {
	(void)ndx;

	*flags |= LA_SYMB_NOPLTENTER | LA_SYMB_NOPLTEXIT;

	if(*defcook != SDL2_SO_COOKIE) {
		return sym->st_value;
	}

	fprintd(stderr, "found sdl2 symbol %s\n", symname);

	if(strcmp(symname, POLLEVENT_SYMNAME) == 0) {
		fprintd(stderr, "is pollevent\n");

		if(!(*flags & LA_SYMB_ALTVALUE)) {
			// this is the original SDL_PollEvent, store a copy
			real_SDL_PollEvent = (SDL_PollEvent_t *)sym->st_value;
		}

		if(*refcook != EXECUTABLE_COOKIE) {
			// return the real function for use inside sdl2 or our library
			return (uintptr_t)real_SDL_PollEvent;
		} else {
			// otherwise shim it for the game
			return (uintptr_t)shim_SDL_PollEvent;
		}
	}

	return sym->st_value;
}

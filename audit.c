/*
 * Copyright (c) 2024 James R Larrowe
 * SPDX-License-Identifier: Zlib
 */

#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif
#include "oldevents.h"
#include <assert.h>
#include <dlfcn.h>
#include <inttypes.h>
#include <link.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SDL2_SO_NAME		"libSDL2-2.0.so.0"
#define POLLEVENT_SYMNAME	"SDL_PollEvent"
#define SDL2_SO_COOKIE		14084	// random number
#define EXECUTABLE_COOKIE	48600	// ^^
_Static_assert(SDL2_SO_COOKIE != EXECUTABLE_COOKIE, "sdl2 and executable cookies must be different!");

SDL_PollEvent_t *real_SDL_PollEvent = NULL;

#ifndef NDEBUG
# define fprintd(stream, ...) fprintf(stream, __VA_ARGS__)
#else
# define fprintd(stream, ...)
#endif

unsigned int la_version(unsigned int version) {
	(void)version;
	_Static_assert(LAV_CURRENT == 2); // LAV_CURRENT guarantees abi compat, not api compat
	fprintd(stderr, "la_version %u\n", version);
	return LAV_CURRENT;
}

void la_preinit(uintptr_t *cookie) {
	assert(*cookie == EXECUTABLE_COOKIE);
	fprintd(stderr, "la_preinit\n");
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
		*cookie = EXECUTABLE_COOKIE;
		fprintd(stderr, "base executable\n");
		return LA_FLG_BINDFROM;
	}

	if(lmid == LM_ID_BASE && strcmp(basename(map->l_name), SDL2_SO_NAME) == 0) {
		fprintd(stderr, "is sdl2\n");
		*cookie = SDL2_SO_COOKIE;
		return LA_FLG_BINDTO;
	}

	return 0;
}

uintptr_t la_symbind32(Elf32_Sym *sym, unsigned int ndx,
                       uintptr_t *refcook, uintptr_t *defcook,
                       unsigned int *flags, const char *symname) {
	uintptr_t retval = sym->st_value;
	int return_real = 0;
	(void)ndx;

	*flags |= LA_SYMB_NOPLTENTER | LA_SYMB_NOPLTEXIT;

	if(*defcook != SDL2_SO_COOKIE) {
		return retval;
	}

	if(*refcook != EXECUTABLE_COOKIE) {
		fprintd(stderr, "not main program, returning real symbol\n");
		return_real = 1;
	}

	fprintd(stderr, "found sdl2 symbol %s\n", symname);

	if(strcmp(symname, POLLEVENT_SYMNAME) == 0) {
		fprintd(stderr, "is pollevent\n");

		if(!(*flags & LA_SYMB_ALTVALUE)) {
			real_SDL_PollEvent = (SDL_PollEvent_t *)sym->st_value;
		}

		if(return_real) {
			retval = (uintptr_t)real_SDL_PollEvent;
		} else {
			retval = (uintptr_t)convert_sdl_event_to2;
		}
	}

	return retval;
}

#if 0
/* This function, if present, causes the dynamic linker
 * to enable its profiling hooks. Currently, this causes
 * SDL to crash inside glibc. Don't want to spend the time
 * or effort required to debug this right now, so I take
 * a different approach above.
 */
unsigned int la_i86_gnu_pltexit(Elf32_Sym *sym, unsigned int ndx,
				uintptr_t *refcook, uintptr_t *defcook,
				const La_i86_regs *inregs, La_i86_retval *outregs,
				const char *symname) {
	assert(*refcook != SDL2_SO_COOKIE &&
	       *defcook == SDL2_SO_COOKIE &&
	       strcmp(symname, pollevent_symname) == 0);
	(void)sym;
	(void)ndx;
	(void)outregs;

	fprintd(stderr, "converting event...\n");
	// XXX: outdated, convert_sdl_event_to2 calls SDL_PollEvent itself now
	//      will probably crash if used
	convert_sdl_event_to2((SDL_Event *)((void **)inregs->lr_esp)[1]);
	fprintd(stderr, "done\n");
	return outregs->lrv_eax; // this is ignored
}
#endif

/*
 * Copyright (c) 2024 James R Larrowe
 * SPDX-License-Identifier: Zlib
 */

#include "oldevents.h"
#include <assert.h>
#include <inttypes.h>
#include <link.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define SDL2_SO_COOKIE		14084	// random number
#define EXECUTABLE_COOKIE	48600	// ^^

#if __ELF_NATIVE_CLASS == 32
# define la_symbind la_symbind32
# define la_pltexit la_i86_gnu_pltexit
# define La_regs La_i86_regs
# define La_retval La_i86_retval
# define lrv_ax lrv_eax
#elif __ELF_NATIVE_CLASS == 64
# define la_symbind la_symbind64
# define la_pltexit la_x86_64_gnu_pltexit
# define La_regs La_x86_64_regs
# define La_retval La_x86_64_retval
# define lrv_ax lrv_rax
#else // ???
# error __ELF_NATIVE_CLASS must be 32 or 64
#endif

#ifndef NDEBUG
# define fprintd(stream, ...) fprintf(stream, __VA_ARGS__)
#else
# define fprintd(stream, ...) ((void)stream, (void)__VA_ARGS__)
#endif

_Static_assert(SDL2_SO_COOKIE != EXECUTABLE_COOKIE, "sdl2 and executable cookies must be different!");

static int event_retval = 0;
static int empty(SDL_Event *event) { (void)event; return event_retval; }
unsigned int la_pltexit(ElfW(Sym) *sym, unsigned int ndx,
				   uintptr_t *refcook, uintptr_t *defcook,
				   const La_regs *inregs, La_retval *outregs,
				   const char *symname) {
	SDL_PollEvent_t *saved = NULL;

	assert(*refcook == EXECUTABLE_COOKIE &&
	       *defcook == SDL2_SO_COOKIE &&
	       strcmp(symname, POLLEVENT_SYMNAME) == 0);
	(void)sym;
	(void)ndx;
	(void)refcook;
	(void)defcook;
	(void)symname;

	// doubly hacky: replace real_SDL_PollEvent with an empty function
	SDL_PollEvent_t *saved = real_SDL_PollEvent;
	event_retval = outregs->lrv_ax;
	real_SDL_PollEvent = empty;

	int retval = // continued
#if __ELF_NATIVE_CLASS == 32
	convert_sdl_event_to2((SDL_Event *)((void **)inregs->lr_esp)[1]);
#elif __ELF_NATIVE_CLASS == 64
	convert_sdl_event_to2((SDL_Event *)inregs->lr_rdi);
#else
# error __ELF_NATIVE_CLASS must be 32 or 64
#endif
	real_SDL_PollEvent = saved;
	event_retval = 0;

	return retval; // this is ignored
}

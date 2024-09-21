#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include "oldevents.h"
#include <assert.h>
#include <inttypes.h>
#include <link.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SDL2_SO_COOKIE 0x00000001

static const char *const sdl2_name = "libSDL2-2.0.so.0";

unsigned int la_version(unsigned int version) {
	fprintf(stderr, "la_version %u, LAV_CURRENT %u\n", version, LAV_CURRENT);
	return LAV_CURRENT;
}

char *la_objsearch(const char *name, uintptr_t *cookie,
		   unsigned int flag) {
	fprintf(stderr, "library: %s\n"
			"cookie: 0x%" PRIxPTR "\n"
			"flag: %x\n", name, *cookie, flag);

	if(flag == LA_SER_RUNPATH && strcmp(basename(name), sdl2_name) == 0) {
		fputs("In the loop\n", stderr);
		return NULL;
	}

	return (char *)name;
}

unsigned int la_objopen(struct link_map *map, Lmid_t lmid,
			uintptr_t *cookie) {
	fprintf(stderr, "objopen %s\n", map->l_name);
	if(lmid == LM_ID_BASE && strcmp(basename(map->l_name), sdl2_name) == 0) {
		*cookie = SDL2_SO_COOKIE;
		return LA_FLG_BINDTO;
	}

	return 0;
}

uintptr_t la_symbind32(Elf32_Sym *sym, unsigned int ndx,
                       uintptr_t *refcook, uintptr_t *defcook,
                       unsigned int *flags, const char *symname) {
	assert(*defcook == SDL2_SO_COOKIE);
	(void)ndx;
	(void)refcook;
	(void)defcook;
	(void)flags;
	(void)symname;
	return sym->st_value;
}

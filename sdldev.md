ABI compat shim for pre-2.0.0 games

I'm trying to write a little wrapper using `LD_PRELOAD`/`LD_AUDIT` (haven't decided which yet) to fix Don't Starve. It uses pre-stable version of SDL2 (somewhere from [[`4149992`](https://github.com/libsdl-org/SDL-historical-archive/commit/4149992ac26e739dd843f78a5a7bbe2dc5bcca71), [`d151ba0`](https://github.com/libsdl-org/SDL-historical-archive/commit/d151ba09266de92d2a4f631c86441d637f853e9e)) in historical-archive), and it's using the 32-bit x86 architecture on Linux.

The main issue that I've seen with it is the insertions of mouse IDs inside the `SDL_Mouse.*Event` structures after [`9707138`](https://github.com/libsdl-org/SDL-historical-archive/commit/9707138ff7ba19217d00fb03d7e9c1d9ac77dcf0). As a temporary fix I wrote a patch to move those to the end of the structure, but I'd rather not have a whole separate build of SDL2 (that I have to keep updated) for *one* game. It'll also probably break when SDL2 is replaced with sdl2-compat, too.

So I decided to create wrappers for those when interfacing in/out of SDL2, 

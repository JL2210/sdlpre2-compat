SDL ABI compat shim for Don't Starve
---

This wrapper uses the `LD_AUDIT` interface to allow the game Don't Starve to
run with modern versions of SDL2. Don't Starve ships with a pre-2.0.0 release
of SDL2 (somewhere from [[`4149992`](https://github.com/libsdl-org/SDL-historical-archive/commit/4149992ac26e739dd843f78a5a7bbe2dc5bcca71),
[`d151ba0`](https://github.com/libsdl-org/SDL-historical-archive/commit/d151ba09266de92d2a4f631c86441d637f853e9e))).
It was built for the 32-bit x86 architecture on Linux.

To build (release build heavily recommended for playable experience):

```bash
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

To use, put this in your Steam launch arguments:

```
LD_AUDIT=/path/to/sdlpre2-compat/build/audit.so %command%
```

Now you can add other arguments, e.g. `SDL_VIDEODRIVER=wayland` or
`SDL_AUDIODRIVER=pipewire`.

There are several issues mainly relating to event struct layout changes before
2.0.0's release. I have [a different repo](https://github.com/JL2210/dont-starve-sdl2)
where I found out what changes were causing these problems. In that repo I made
a temporary fix to SDL2 that restored the old struct layouts.

However, that wasn't a perfect solution; it required rebuilding SDL every time
that it was updated and needed a separate copy of the library solely for Don't
Starve. Past that, it would also have issues once sdl3 became the standard and
would need to be rewritten to work with sdl2-compat.

Some self-imposed stipulations I had for this project:

- it could not modify the Steam depot for Don't Starve (including the executable)
- it shouldn't link against SDL2, only require the header files to be built
- it should be self-contained rather than e.g. be a fork of SDL2 (like the patch)

In order to meet these requirements I decided on using the runtime linker's
auditing interface. This is fairly niche and usually used for security auditing
(as the name implies). The documentation for it can be found at [`rtld-audit(7)`](https://man7.org/linux/man-pages/man7/rtld-audit.7.html).

`LD_AUDIT` allows the auditing library to view or change just about everything
in the child process' execution environment. Some of the main features I used
in this project include:

- overriding libraries used (such as ones forced with `DT_RPATH`)
- selectively redirecting symbols to ones provided by the auditor

One I considered using was `la_pltexit`, which allows the auditor to modify
the return value of a function before it returns to the caller. However, when
this function exists, glibc forces on its profiling hooks. This led me to
discover what seems like [a bug in SDL](https://github.com/libsdl-org/SDL/issues/10918),
and I had to abandon that approach in favor of a (slightly jankier) solution
using `la_symbind`.

More details about the specific layout changes can be found in [NOTES.txt].

Symbols that I determined were potentially affected by ABI changes are listed
in [problematic-syms-list.txt]. This list was created with
`objdump -T ~/.local/share/Steam/steamapps/common/dont_starve/bin/dontstarve_steam`
and whittled down by hand by comparing the list against the changes in
`include/` from `4149992..release-2.0.0` (again in SDL-historical-archive).

[audit.c] is the main source of the auditing library and does the work to
ignore the rpath for `libSDL2-2.0.so.0` and redirect `SDL_PollEvent` to
[event-converter.c].

[event-converter.c] does some macro magic and `memmove`ing to make the new
events look like the old ones. The macros are a bit confusing but they do what they
say.

This project is licensed under the [Zlib license](LICENSE.txt), the same as SDL2.
The structures in [`upstream.h`] are taken almost verbatim from SDL2 at the first
commit mentioned above, and any modifications made are probably not copyrightable.

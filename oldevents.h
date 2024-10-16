/*
 * Copyright (c) 2024 James R Larrowe
 * SPDX-License-Identifier: Zlib
 */

#ifndef OLDEVENTS_H
#define OLDEVENTS_H 1

#include <SDL_events.h>
#include <SDL_gamecontroller.h>

#define SDL2_SO_COOKIE		14084	// random number
#define EXECUTABLE_COOKIE	48600	// ^^
static_assert(SDL2_SO_COOKIE != EXECUTABLE_COOKIE, "sdl2 and executable cookies must be different!");

typedef int SDL_PollEvent_t(SDL_Event *);

extern SDL_PollEvent_t *real_SDL_PollEvent;
extern SDL_PollEvent_t shim_SDL_PollEvent;

#if 0
#define SDL_MouseMotionEvent		SDL_OldMouseMotionEvent
#define SDL_MouseButtonEvent		SDL_OldMouseButtonEvent
#define SDL_MouseWheelEvent		SDL_OldMouseWheelEvent
#endif
#define SDL_JoyAxisEvent		SDL_OldJoyAxisEvent
#define SDL_JoyBallEvent		SDL_OldJoyBallEvent
#define SDL_JoyHatEvent			SDL_OldJoyHatEvent
#define SDL_JoyButtonEvent		SDL_OldJoyButtonEvent
#define SDL_ControllerAxisEvent		SDL_OldControllerAxisEvent
#define SDL_ControllerButtonEvent	SDL_OldControllerButtonEvent
#if 0
#define SDL_ControllerDeviceEvent	SDL_OldControllerDeviceEvent
#define SDL_TouchFingerEvent		SDL_OldTouchFingerEvent
#define SDL_MultiGestureEvent		SDL_OldMultiGestureEvent
#define SDL_DollarGestureEvent		SDL_OldDollarGestureEvent
#define SDL_TouchButtonEvent		SDL_OldTouchButtonEvent
#endif
#define SDL_CONTROLLER_AXIS		SDL_GameControllerAxis
#define SDL_CONTROLLER_BUTTON		SDL_GameControllerButton

#include "upstream.h"

#if 0
#undef SDL_MouseMotionEvent
#undef SDL_MouseButtonEvent
#undef SDL_MouseWheelEvent
#endif
#undef SDL_JoyAxisEvent
#undef SDL_JoyBallEvent
#undef SDL_JoyHatEvent
#undef SDL_JoyButtonEvent
#undef SDL_ControllerAxisEvent
#undef SDL_ControllerButtonEvent
#if 0
#undef SDL_ControllerDeviceEvent
#undef SDL_TouchFingerEvent
#undef SDL_MultiGestureEvent
#undef SDL_DollarGestureEvent
#undef SDL_TouchButtonEvent
#endif

#endif // _OLDEVENTS_H

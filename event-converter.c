/*
 * Copyright (c) 2024 James R Larrowe
 * SPDX-License-Identifier: Zlib
 */

#include <SDL_events.h>
#include "oldevents.h"
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

// NOTE: use memmove instead of memcpy!

#define remove_member(remove, event, _event) \
	_Static_assert(__builtin_types_compatible_p(typeof(event), SDL_Event *), "event must be of type SDL_Event*"); \
	assert(event == (SDL_Event *)_event && "event and _event must be equal"); \
	memmove(&_event->remove, &_event->remove+1, (char *)(event+1)-(char *)(&_event->remove+1))

// automatically set up _event
#define simple_remove_member(type, remove, event) \
	SDL_##type##Event *_event = (typeof(_event))event; \
	remove_member(remove, event, _event)

// assumes little-endian (this is x86 anyway so point is moot)
#define truncate_member(type, member, event, _event) \
	_Static_assert(__builtin_types_compatible_p(typeof(event), SDL_Event *), "event must be of type SDL_Event*"); \
	assert(event == (SDL_Event *)_event && "event and _event must be equal"); \
	SDL_Old##type##Event *_oldevent = (typeof(_oldevent))event; \
	_oldevent->member = (typeof(_oldevent->member))_event->member; \
	memmove(&_oldevent->member+1, &_event->member+1, (char *)(event+1)-(char *)(&_event->member+1))

// automatically set up _event
#define simple_truncate_member(type, member, event) \
	SDL_##type##Event *_event = (typeof(_event))event; \
	truncate_member(type, member, event, _event)

// modifies the event in-place
int convert_sdl_event_to2(SDL_Event *event) {
	int retval = real_SDL_PollEvent(event);
	switch(event->type) {
		case SDL_MOUSEMOTION: {
			simple_remove_member(MouseMotion, which, event);
		}
		break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP: {
			simple_remove_member(MouseButton, which, event);
		}
		break;
		case SDL_MOUSEWHEEL: {
			simple_remove_member(MouseWheel, which, event);
		}
		break;
		case SDL_JOYAXISMOTION: {
			SDL_JoyAxisEvent *_event = (SDL_JoyAxisEvent *)event;
			int value = _event->value;
			truncate_member(JoyAxis, which, event, _event);
			_oldevent->value = value;
		}
		break;
		case SDL_JOYBALLMOTION: {
			SDL_JoyBallEvent *_event = (SDL_JoyBallEvent *)event;
			int xrel = _event->xrel, yrel = _event->yrel;
			truncate_member(JoyBall, which, event, _event);
			_oldevent->xrel = xrel; _oldevent->yrel = yrel;
		}
		break;
		case SDL_JOYHATMOTION: {
			simple_truncate_member(JoyHat, which, event);
		}
		break;
		case SDL_JOYBUTTONDOWN:
		case SDL_JOYBUTTONUP: {
			simple_truncate_member(JoyButton, which, event);
		}
		break;
		case SDL_CONTROLLERAXISMOTION: {
			SDL_ControllerAxisEvent *_event = (SDL_ControllerAxisEvent *)event;
			SDL_OldControllerAxisEvent oldevent = {
				.type		=	_event->type,
				.timestamp	=	_event->timestamp,
				.which		=	(Uint8)_event->which,
				.axis		=	(SDL_CONTROLLER_AXIS)_event->axis,
				.value		=	_event->value,
			};
			memcpy(_event, &oldevent, sizeof(oldevent));
		}
		break;
		case SDL_CONTROLLERBUTTONDOWN:
		case SDL_CONTROLLERBUTTONUP: {
			SDL_ControllerButtonEvent *_event = (SDL_ControllerButtonEvent *)event;
			SDL_OldControllerButtonEvent oldevent = {
				.type           =       _event->type,
				.timestamp      =       _event->timestamp,
				.which		=	(Uint8)_event->which,
				.button		=	(SDL_CONTROLLER_BUTTON)_event->button,
				.state		=	_event->state,
			};
			memcpy(_event, &oldevent, sizeof(oldevent));
		}
		break;
		/* After this there's a bunch of touch stuff which I don't
		 * think is implemented in DS. Regardless, there are removed
		 * fields which I don't think I can reliably recreate with
		 * the info that I'm provided, so I don't implement those.
		 */
		default: /* no changes, do nothing */
		break;
	}
	return retval;
}

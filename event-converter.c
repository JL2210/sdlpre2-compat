#include <SDL2/SDL_events.h>
#include "oldevents.h"
#include <stddef.h>
#include <stdlib.h>

// NOTE: use memmove instead of memcpy!

// requires event to be set up
#define remove_member(type, remove) \
	SDL_##type *_event = event; \
	memmove(&_event->remove, &_event->remove+1, sizeof(SDL_Event)-offsetof(SDL_##type, remove)-sizeof(_event->remove))

// requires event and _event to be set up
#define change_type_before(type, nextmember) \
	SDL_Old##type *_oldevent = (void *)event; \
	memmove(&_oldevent->nextmember, &_event->nextmember, sizeof(SDL_Event)-offsetof(SDL_##type, nextmember))

// sets up _event for you
#define s_change_type_before(type, nextmember) \
	SDL_##type *_event = event; \
	change_type_before(type, nextmember)



// modifies the event in-place
void convert_sdl_event_to2(SDL_Event *event) {
	switch(event->type) {
		case SDL_MOUSEMOTION: {
			remove_member(MouseMotionEvent, which);
		}
		break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP: {
			remove_member(MouseButtonEvent, which);
		}
		break;
		case SDL_MOUSEWHEEL: {
			remove_member(MouseWheelEvent, which);
		}
		break;
		case SDL_JOYAXISMOTION: {
			SDL_JoyAxisEvent *_event = event;
			int value = _event->value;
			change_type_before(JoyAxisEvent, axis);
			_oldevent->value = value;
		}
		break;
		case SDL_JOYBALLMOTION: {
			SDL_JoyBallEvent *_event = event;
			int xrel = _event->xrel, yrel = _event->yrel;
			change_type_before(JoyBallEvent, ball);
			_oldevent->xrel = xrel; _oldevent->yrel = yrel;
		}
		break;
		case SDL_JOYHATMOTION: {
			s_change_type_before(JoyHatEvent, hat);
		}
		break;
		case SDL_JOYBUTTONDOWN:
		case SDL_JOYBUTTONUP: {
			s_change_type_before(JoyButtonEvent, button);
		}
		break;
		case SDL_CONTROLLERAXISMOTION: {
			SDL_ControllerAxisEvent *_event = event;
			SDL_OldControllerAxisEvent oldevent = {
				.type		=	_event->type,
				.timestamp	=	_event->timestamp,
				.which		=	_event->which,
				.axis		=	_event->axis,
				.value		=	_event->value,
			};
			memcpy(_event, &oldevent, sizeof(oldevent));
		}
		break;
		case SDL_CONTROLLERBUTTONDOWN:
		case SDL_CONTROLLERBUTTONUP: {
			SDL_ControllerButtonEvent *_event = event;
			SDL_OldControllerAxisEvent oldevent = {
				.type           =       _event->type,
				.timestamp      =       _event->timestamp,
				.which		=	_event->which,
				.button		=	_event->button,
				.state		=	_event->state,
			};
			memcpy(_event, &oldevent, sizeof(oldevent);
		}
		break;
	}
}

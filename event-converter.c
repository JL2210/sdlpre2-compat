#include <SDL_events.h>
#include "oldevents.h"
#include <stddef.h>
#include <stdlib.h>

// NOTE: use memmove instead of memcpy!

// requires event to be set up
#define remove_member(type, remove) \
	SDL_##type *_event = (SDL_##type *)event; \
	memmove(&_event->remove, &_event->remove+1, sizeof(SDL_Event)-offsetof(SDL_##type, remove)-sizeof(_event->remove))

// requires event and _event to be set up
#define change_type_before(type, nextmember) \
	SDL_Old##type *_oldevent = (SDL_Old##type *)event; \
	memmove(&_oldevent->nextmember, &_event->nextmember, sizeof(SDL_Event)-offsetof(SDL_##type, nextmember))

// sets up _event for you
#define s_change_type_before(type, nextmember) \
	SDL_##type *_event = (SDL_##type *)event; \
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
			SDL_JoyAxisEvent *_event = (SDL_JoyAxisEvent *)event;
			int value = _event->value;
			change_type_before(JoyAxisEvent, axis);
			_oldevent->value = value;
		}
		break;
		case SDL_JOYBALLMOTION: {
			SDL_JoyBallEvent *_event = (SDL_JoyBallEvent *)event;
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
			SDL_ControllerAxisEvent *_event = (SDL_ControllerAxisEvent *)event;
			SDL_OldControllerAxisEvent oldevent = {
				.type		=	_event->type,
				.timestamp	=	_event->timestamp,
				.which		=	_event->which,
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
				.which		=	_event->which,
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
}

#ifndef OLDEVENTS_H
#define OLDEVENTS_H 1

#include <SDL_events.h>
#include <SDL_gamecontroller.h>

extern void convert_sdl_event_to2(SDL_Event *event);

#define SDL_MouseMotionEvent		SDL_OldMouseMotionEvent
#define SDL_MouseButtonEvent		SDL_OldMouseButtonEvent
#define SDL_MouseWheelEvent		SDL_OldMouseWheelEvent
#define SDL_JoyAxisEvent		SDL_OldJoyAxisEvent
#define SDL_JoyBallEvent		SDL_OldJoyBallEvent
#define SDL_JoyHatEvent			SDL_OldJoyHatEvent
#define SDL_JoyButtonEvent		SDL_OldJoyButtonEvent
#define SDL_ControllerAxisEvent		SDL_OldControllerAxisEvent
#define SDL_ControllerButtonEvent	SDL_OldControllerButtonEvent
#define SDL_ControllerDeviceEvent	SDL_OldControllerDeviceEvent
#define SDL_TouchFingerEvent		SDL_OldTouchFingerEvent
#define SDL_MultiGestureEvent		SDL_OldMultiGestureEvent
#define SDL_DollarGestureEvent		SDL_OldDollarGestureEvent
#define SDL_TouchButtonEvent		SDL_OldTouchButtonEvent
#define SDL_CONTROLLER_AXIS		SDL_GameControllerAxis
#define SDL_CONTROLLER_BUTTON		SDL_GameControllerButton

#include "upstream.h"

#undef SDL_MouseMotionEvent
#undef SDL_MouseButtonEvent
#undef SDL_MouseWheelEvent
#undef SDL_JoyAxisEvent
#undef SDL_JoyBallEvent
#undef SDL_JoyHatEvent
#undef SDL_JoyButtonEvent
#undef SDL_ControllerAxisEvent
#undef SDL_ControllerButtonEvent
#undef SDL_ControllerDeviceEvent
#undef SDL_TouchFingerEvent
#undef SDL_MultiGestureEvent
#undef SDL_DollarGestureEvent
#undef SDL_TouchButtonEvent

#endif /* _OLDEVENTS_H */

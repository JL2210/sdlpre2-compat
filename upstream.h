#ifndef OLDEVENTS_H
#error This file should only be included from oldevents.h
#endif

typedef struct SDL_MouseMotionEvent
{
    Uint32 type;        /**< ::SDL_MOUSEMOTION */
    Uint32 timestamp;
    Uint32 windowID;    /**< The window with mouse focus, if any */
    Uint8 state;        /**< The current button state */
    Uint8 padding1;
    Uint8 padding2;
    Uint8 padding3;
    int x;              /**< X coordinate, relative to window */
    int y;              /**< Y coordinate, relative to window */
    int xrel;           /**< The relative motion in the X direction */
    int yrel;           /**< The relative motion in the Y direction */
} SDL_MouseMotionEvent;

typedef struct SDL_MouseButtonEvent
{
    Uint32 type;        /**< ::SDL_MOUSEBUTTONDOWN or ::SDL_MOUSEBUTTONUP */
    Uint32 timestamp;
    Uint32 windowID;    /**< The window with mouse focus, if any */
    Uint8 button;       /**< The mouse button index */
    Uint8 state;        /**< ::SDL_PRESSED or ::SDL_RELEASED */
    Uint8 padding1;
    Uint8 padding2;
    int x;              /**< X coordinate, relative to window */
    int y;              /**< Y coordinate, relative to window */
} SDL_MouseButtonEvent;

typedef struct SDL_MouseWheelEvent
{
    Uint32 type;        /**< ::SDL_MOUSEWHEEL */
    Uint32 timestamp;
    Uint32 windowID;    /**< The window with mouse focus, if any */
    int x;              /**< The amount scrolled horizontally */
    int y;              /**< The amount scrolled vertically */
} SDL_MouseWheelEvent;

typedef struct SDL_JoyAxisEvent
{
    Uint32 type;        /**< ::SDL_JOYAXISMOTION */
    Uint32 timestamp;
    Uint8 which;        /**< The joystick instance id */
    Uint8 axis;         /**< The joystick axis index */
    Uint8 padding1;
    Uint8 padding2;
    int value;          /**< The axis value (range: -32768 to 32767) */
} SDL_JoyAxisEvent;

typedef struct SDL_JoyBallEvent
{
    Uint32 type;        /**< ::SDL_JOYBALLMOTION */
    Uint32 timestamp;
    Uint8 which;        /**< The joystick instance id */
    Uint8 ball;         /**< The joystick trackball index */
    Uint8 padding1;
    Uint8 padding2;
    int xrel;           /**< The relative motion in the X direction */
    int yrel;           /**< The relative motion in the Y direction */
} SDL_JoyBallEvent;

typedef struct SDL_JoyHatEvent
{
    Uint32 type;        /**< ::SDL_JOYHATMOTION */
    Uint32 timestamp;
    Uint8 which;        /**< The joystick instance id */
    Uint8 hat;          /**< The joystick hat index */
    Uint8 value;        /**< The hat position value.
                         *   \sa ::SDL_HAT_LEFTUP ::SDL_HAT_UP ::SDL_HAT_RIGHTUP
                         *   \sa ::SDL_HAT_LEFT ::SDL_HAT_CENTERED ::SDL_HAT_RIGHT
                         *   \sa ::SDL_HAT_LEFTDOWN ::SDL_HAT_DOWN ::SDL_HAT_RIGHTDOWN
                         *
                         *   Note that zero means the POV is centered.
                         */
    Uint8 padding1;
} SDL_JoyHatEvent;

typedef struct SDL_JoyButtonEvent
{
    Uint32 type;        /**< ::SDL_JOYBUTTONDOWN or ::SDL_JOYBUTTONUP */
    Uint32 timestamp;
    Uint8 which;        /**< The joystick instance id */
    Uint8 button;       /**< The joystick button index */
    Uint8 state;        /**< ::SDL_PRESSED or ::SDL_RELEASED */
    Uint8 padding1;
} SDL_JoyButtonEvent;

typedef struct SDL_ControllerAxisEvent
{
    Uint32 type;        /**< ::SDL_CONTROLLERAXISMOTION */
    Uint32 timestamp;
    Uint8 which;        /**< The joystick instance id */
    SDL_CONTROLLER_AXIS axis;         /**< The joystick axis index */
    int value;          /**< The axis value (range: -32768 to 32767) */
} SDL_ControllerAxisEvent;

typedef struct SDL_ControllerButtonEvent
{
    Uint32 type;        /**< ::SDL_CONTROLLERBUTTONDOWN or ::SDL_CONTROLLERBUTTONUP */
    Uint32 timestamp;
    Uint8 which;        /**< The joystick instance id */
    SDL_CONTROLLER_BUTTON button;       /**< The joystick button index */
    Uint8 state;        /**< ::SDL_PRESSED or ::SDL_RELEASED */
} SDL_ControllerButtonEvent;

typedef struct SDL_ControllerDeviceEvent
{
        Uint32 type;        /**< ::SDL_CONTROLLERDEVICEADDED or ::SDL_CONTROLLERDEVICEREMOVED */
        Uint32 timestamp;
        Uint32 which;        /**< The joystick device index for ADD, instance_id for REMOVE*/
} SDL_ControllerDeviceEvent;

typedef struct SDL_TouchFingerEvent
{
    Uint32 type;        /**< ::SDL_FINGERMOTION OR
                           SDL_FINGERDOWN OR SDL_FINGERUP*/
    Uint32 timestamp;
    Uint32 windowID;    /**< The window with mouse focus, if any */
    SDL_TouchID touchId;        /**< The touch device id */
    SDL_FingerID fingerId;
    Uint8 state;        /**< The current button state */
    Uint8 padding1;
    Uint8 padding2;
    Uint8 padding3;
    Uint16 x;
    Uint16 y;
    Sint16 dx;
    Sint16 dy;
    Uint16 pressure;
} SDL_TouchFingerEvent;

typedef struct SDL_MultiGestureEvent
{
    Uint32 type;        /**< ::SDL_MULTIGESTURE */
    Uint32 timestamp;
    Uint32 windowID;    /**< The window with mouse focus, if any */
    SDL_TouchID touchId;        /**< The touch device index */
    float dTheta;
    float dDist;
    float x;  /* currently 0...1. Change to screen coords? */
    float y;  
    Uint16 numFingers;
    Uint16 padding;
} SDL_MultiGestureEvent;

typedef struct SDL_DollarGestureEvent
{
    Uint32 type;        /**< ::SDL_DOLLARGESTURE */
    Uint32 timestamp;
    Uint32 windowID;    /**< The window with mouse focus, if any */
    SDL_TouchID touchId;        /**< The touch device index */
    SDL_GestureID gestureId;
    Uint32 numFingers;
    float error;
  /*
    //TODO: Enable to give location?
    float x;  //currently 0...1. Change to screen coords?
    float y;  
  */
} SDL_DollarGestureEvent;

// REMOVED
typedef struct SDL_TouchButtonEvent
{
    Uint32 type;        /**< ::SDL_TOUCHBUTTONUP OR SDL_TOUCHBUTTONDOWN */
    Uint32 timestamp;   
    Uint32 windowID;    /**< The window with mouse focus, if any */
    SDL_TouchID touchId;        /**< The touch device index */
    Uint8 state;        /**< The current button state */
    Uint8 button;        /**< The button changing state */
    Uint8 padding1;
    Uint8 padding2;
} SDL_TouchButtonEvent;

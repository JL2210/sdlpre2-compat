ABI notes:

good: SDL_AudioCVT has same size before and after packing on 32-bit (only)
bad: most of SDL_*Event have different sizes/layouts and need reorganizing

(fixed)
```
@@ -196,14 +220,12 @@ typedef struct SDL_MouseMotionEvent
     Uint32 type;        /**< ::SDL_MOUSEMOTION */
     Uint32 timestamp;
     Uint32 windowID;    /**< The window with mouse focus, if any */
-    Uint8 state;        /**< The current button state */
-    Uint8 padding1;
-    Uint8 padding2;
-    Uint8 padding3;
-    int x;              /**< X coordinate, relative to window */
-    int y;              /**< Y coordinate, relative to window */
-    int xrel;           /**< The relative motion in the X direction */
-    int yrel;           /**< The relative motion in the Y direction */
+    Uint32 which;       /**< The mouse instance id, or SDL_TOUCH_MOUSEID */
+    Uint32 state;       /**< The current button state */
+    Sint32 x;           /**< X coordinate, relative to window */
+    Sint32 y;           /**< Y coordinate, relative to window */
+    Sint32 xrel;        /**< The relative motion in the X direction */
+    Sint32 yrel;        /**< The relative motion in the Y direction */
```

(fixed)
```
@@ -214,12 +236,13 @@ typedef struct SDL_MouseButtonEvent
     Uint32 type;        /**< ::SDL_MOUSEBUTTONDOWN or ::SDL_MOUSEBUTTONUP */
     Uint32 timestamp;
     Uint32 windowID;    /**< The window with mouse focus, if any */
+    Uint32 which;       /**< The mouse instance id, or SDL_TOUCH_MOUSEID */
     Uint8 button;       /**< The mouse button index */
     Uint8 state;        /**< ::SDL_PRESSED or ::SDL_RELEASED */
     Uint8 padding1;
     Uint8 padding2;
-    int x;              /**< X coordinate, relative to window */
-    int y;              /**< Y coordinate, relative to window */
+    Sint32 x;           /**< X coordinate, relative to window */
+    Sint32 y;           /**< Y coordinate, relative to window */
```

(fixed, untested)
```
@@ -230,8 +253,9 @@ typedef struct SDL_MouseWheelEvent
     Uint32 type;        /**< ::SDL_MOUSEWHEEL */
     Uint32 timestamp;
     Uint32 windowID;    /**< The window with mouse focus, if any */
-    int x;              /**< The amount scrolled horizontally */
-    int y;              /**< The amount scrolled vertically */
+    Uint32 which;       /**< The mouse instance id, or SDL_TOUCH_MOUSEID */
+    Sint32 x;           /**< The amount scrolled horizontally */
+    Sint32 y;           /**< The amount scrolled vertically */
```

(fixed, untested)
```
@@ -241,11 +265,13 @@ typedef struct SDL_JoyAxisEvent
 {
     Uint32 type;        /**< ::SDL_JOYAXISMOTION */
     Uint32 timestamp;
-    Uint8 which;        /**< The joystick instance id */
+    SDL_JoystickID which; /**< The joystick instance id */
     Uint8 axis;         /**< The joystick axis index */
     Uint8 padding1;
     Uint8 padding2;
-    int value;          /**< The axis value (range: -32768 to 32767) */
+    Uint8 padding3;
+    Sint16 value;       /**< The axis value (range: -32768 to 32767) */
+    Uint16 padding4;
```

(fixed, untested)
```
@@ -255,12 +281,13 @@ typedef struct SDL_JoyBallEvent
 {
     Uint32 type;        /**< ::SDL_JOYBALLMOTION */
     Uint32 timestamp;
-    Uint8 which;        /**< The joystick instance id */
+    SDL_JoystickID which; /**< The joystick instance id */
     Uint8 ball;         /**< The joystick trackball index */
     Uint8 padding1;
     Uint8 padding2;
-    int xrel;           /**< The relative motion in the X direction */
-    int yrel;           /**< The relative motion in the Y direction */
+    Uint8 padding3;
+    Sint16 xrel;        /**< The relative motion in the X direction */
+    Sint16 yrel;        /**< The relative motion in the Y direction */
```

(fixed, untested)
```
@@ -270,7 +297,7 @@ typedef struct SDL_JoyHatEvent
 {
     Uint32 type;        /**< ::SDL_JOYHATMOTION */
     Uint32 timestamp;
-    Uint8 which;        /**< The joystick instance id */
+    SDL_JoystickID which; /**< The joystick instance id */
     Uint8 hat;          /**< The joystick hat index */
     Uint8 value;        /**< The hat position value.
                          *   \sa ::SDL_HAT_LEFTUP ::SDL_HAT_UP ::SDL_HAT_RIGHTUP
@@ -280,6 +307,7 @@ typedef struct SDL_JoyHatEvent
                          *   Note that zero means the POV is centered.
                          */
     Uint8 padding1;
+    Uint8 padding2;
```

(fixed, untested)
```
@@ -289,10 +317,11 @@ typedef struct SDL_JoyButtonEvent
 {
     Uint32 type;        /**< ::SDL_JOYBUTTONDOWN or ::SDL_JOYBUTTONUP */
     Uint32 timestamp;
-    Uint8 which;        /**< The joystick instance id */
+    SDL_JoystickID which; /**< The joystick instance id */
     Uint8 button;       /**< The joystick button index */
     Uint8 state;        /**< ::SDL_PRESSED or ::SDL_RELEASED */
     Uint8 padding1;
+    Uint8 padding2;
```

(fixed, untested)
```
@@ -313,9 +342,13 @@ typedef struct SDL_ControllerAxisEvent
 {
     Uint32 type;        /**< ::SDL_CONTROLLERAXISMOTION */
     Uint32 timestamp;
-    Uint8 which;        /**< The joystick instance id */
-    SDL_CONTROLLER_AXIS axis;         /**< The joystick axis index */
-    int value;          /**< The axis value (range: -32768 to 32767) */
+    SDL_JoystickID which; /**< The joystick instance id */
+    Uint8 axis;         /**< The controller axis (SDL_GameControllerAxis) */
+    Uint8 padding1;
+    Uint8 padding2;
+    Uint8 padding3;
+    Sint16 value;       /**< The axis value (range: -32768 to 32767) */
+    Uint16 padding4;
```

(fixed, untested)
```
@@ -326,9 +359,11 @@ typedef struct SDL_ControllerButtonEvent
 {
     Uint32 type;        /**< ::SDL_CONTROLLERBUTTONDOWN or ::SDL_CONTROLLERBUTTONUP */
     Uint32 timestamp;
-    Uint8 which;        /**< The joystick instance id */
-    SDL_CONTROLLER_BUTTON button;       /**< The joystick button index */
+    SDL_JoystickID which; /**< The joystick instance id */
+    Uint8 button;       /**< The controller button (SDL_GameControllerButton) */
     Uint8 state;        /**< ::SDL_PRESSED or ::SDL_RELEASED */
+    Uint8 padding1;
+    Uint8 padding2;
```

(no fix needed)
```
@@ -337,51 +372,29 @@ typedef struct SDL_ControllerButtonEvent
  */
 typedef struct SDL_ControllerDeviceEvent
 {
-       Uint32 type;        /**< ::SDL_CONTROLLERDEVICEADDED or ::SDL_CONTROLLERDEVICEREMOVED */
+    Uint32 type;        /**< ::SDL_CONTROLLERDEVICEADDED, ::SDL_CONTROLLERDEVICEREMOVED, or ::SDL_CONTROLLERDEVICEREMAPPED */
     Uint32 timestamp;
-       Uint32 which;        /**< The joystick device index for ADD, instance_id for REMOVE*/
+    Sint32 which;       /**< The joystick device index for the ADDED event, instance id for the REMOVED or REMAPPED event */
```

(not fixed, probably unused)
```
 } SDL_ControllerDeviceEvent;
 
 
 /**
- *  \brief Touch finger motion/finger event structure (event.tfinger.*)
+ *  \brief Touch finger event structure (event.tfinger.*)
  */
 typedef struct SDL_TouchFingerEvent
 {
-    Uint32 type;        /**< ::SDL_FINGERMOTION OR 
-                          SDL_FINGERDOWN OR SDL_FINGERUP*/
+    Uint32 type;        /**< ::SDL_FINGERMOTION or ::SDL_FINGERDOWN or ::SDL_FINGERUP */
     Uint32 timestamp;
-    Uint32 windowID;    /**< The window with mouse focus, if any */
     SDL_TouchID touchId; /**< The touch device id */
     SDL_FingerID fingerId;
-    Uint8 state;        /**< The current button state */
-    Uint8 padding1;
-    Uint8 padding2;
-    Uint8 padding3;
-    Uint16 x;
-    Uint16 y;
-    Sint16 dx;
-    Sint16 dy;
-    Uint16 pressure;
+    float x;            /**< Normalized in the range 0...1 */
+    float y;            /**< Normalized in the range 0...1 */
+    float dx;           /**< Normalized in the range 0...1 */
+    float dy;           /**< Normalized in the range 0...1 */
+    float pressure;     /**< Normalized in the range 0...1 */
```

(not fixed, probably unused)
```
@@ -389,31 +402,29 @@ typedef struct SDL_MultiGestureEvent
 {
     Uint32 type;        /**< ::SDL_MULTIGESTURE */
     Uint32 timestamp;
-    Uint32 windowID;    /**< The window with mouse focus, if any */
     SDL_TouchID touchId; /**< The touch device index */
     float dTheta;
     float dDist;
-    float x;  /* currently 0...1. Change to screen coords? */
+    float x;
     float y;
     Uint16 numFingers;
     Uint16 padding;
 } SDL_MultiGestureEvent;
 
-/* (event.dgesture.*) */
+
+/**
+ * \brief Dollar Gesture Event (event.dgesture.*)
+ */
 typedef struct SDL_DollarGestureEvent
 {
     Uint32 type;        /**< ::SDL_DOLLARGESTURE */
     Uint32 timestamp;
-    Uint32 windowID;    /**< The window with mouse focus, if any */
-    SDL_TouchID touchId;        /**< The touch device index */
+    SDL_TouchID touchId; /**< The touch device id */
     SDL_GestureID gestureId;
     Uint32 numFingers;
     float error;
-  /*
-    //TODO: Enable to give location?
-    float x;  //currently 0...1. Change to screen coords?
-    float y;  
-  */
+    float x;            /**< Normalized center of gesture */
+    float y;            /**< Normalized center of gesture */
```


(not fixed, probably unused)
(would be too hard to do anyway)
```
-/**
- *  \brief Touch finger motion/finger event structure (event.tbutton.*)
- */
-typedef struct SDL_TouchButtonEvent
-{
-    Uint32 type;        /**< ::SDL_TOUCHBUTTONUP OR SDL_TOUCHBUTTONDOWN */
-    Uint32 timestamp;
-    Uint32 windowID;    /**< The window with mouse focus, if any */
-    SDL_TouchID touchId;        /**< The touch device index */
-    Uint8 state;        /**< The current button state */
-    Uint8 button;        /**< The button changing state */
-    Uint8 padding1;
-    Uint8 padding2;
-} SDL_TouchButtonEvent;
```

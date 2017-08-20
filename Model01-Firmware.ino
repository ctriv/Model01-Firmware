 // -*- mode: c++ -*z
// Copyright 2016 Keyboardio, inc. <jesse@keyboard.io>
// See "LICENSE" for license details

#define DEBUG_SERIAL false

#ifndef BUILD_INFORMATION
#define BUILD_INFORMATION "locally built"
#endif


#include "Kaleidoscope-MouseKeys.h"
#include "Kaleidoscope-Macros.h"
#include "Kaleidoscope-LEDControl.h"

#include "Kaleidoscope-Numlock.h"
#include "Kaleidoscope.h"

#include "LED-Off.h"
#include "Kaleidoscope-LEDEffect-BootGreeting.h"
#include "Kaleidoscope-LEDEffect-SolidColor.h"
#include "Kaleidoscope-LEDEffect-Breathe.h"
#include "Kaleidoscope-LEDEffect-Chase.h"
#include "Kaleidoscope-LEDEffect-Rainbow.h"
#include "Kaleidoscope-LED-Stalker.h"
#include "Kaleidoscope-LED-AlphaSquare.h"
#include "Kaleidoscope-Model01-TestMode.h"

#include "Kaleidoscope-FPS.h"

#define MACRO_VERSION_INFO 1
#define Macro_VersionInfo M(MACRO_VERSION_INFO)
#define MACRO_ANY 2
#define Macro_Any M(MACRO_ANY)
#define NUMPAD_KEYMAP 2
#define FPS_KEYMAP 3

#define GENERIC_FN2  KEYMAP_STACKED ( \
___,      Key_F1,           Key_F2,      Key_F3,     Key_F4,        Key_F5,           XXX,         \
Key_Tab,  ___,              Key_mouseUp, ___,        Key_mouseBtnR, Key_mouseWarpEnd, Key_mouseWarpNE, \
Key_Home, Key_mouseL,       Key_mouseDn, Key_mouseR, Key_mouseBtnL, Key_mouseWarpNW,                   \
Key_End,  Key_PrintScreen,  Key_Insert,  ___,        Key_mouseBtnM, Key_mouseWarpSW,  Key_mouseWarpSE, \
                                            Key_LeftControl, Key_Delete, Key_LeftGui, Key_LeftShift,  \
                                                                ___,   \
\
Consumer_ScanPreviousTrack, Key_F6,                 Key_F7,                   Key_F8,                  Key_F9,          Key_F10,          Key_ToggleFPS, \
Consumer_PlaySlashPause,    Consumer_ScanNextTrack, Key_LeftCurlyBracket,     Key_RightCurlyBracket,   Key_LeftBracket, Key_RightBracket, Key_F11, \
                            Key_LeftArrow,          Key_DownArrow,            Key_UpArrow,             Key_RightArrow,  ___,              Key_F12, \
Key_PcApplication,          Key_Mute,               Consumer_VolumeDecrement, Consumer_VolumeIncrement, ___,            Key_Backslash,    Key_Pipe, \
Key_RightShift, Key_RightAlt, Key_Enter, Key_RightControl, \
___ \
)



#define NUMPAD KEYMAP_STACKED  (\
    ___, ___, ___, ___, ___, ___, ___,  \
    ___, ___, ___, ___, ___, ___, ___,  \
    ___, ___, ___, ___, ___, ___,       \
    ___, ___, ___, ___, ___, ___, ___,  \
               ___, ___, ___, ___,  \
                 Key_Keymap1_Momentary, \
\
\
    Macro_VersionInfo,  ___, Key_Keypad7, Key_Keypad8,   Key_Keypad9,        Key_KeypadSubtract, ___, \
    ___, ___, Key_Keypad4, Key_Keypad5,   Key_Keypad6,        Key_KeypadAdd,      ___, \
         ___, Key_Keypad1, Key_Keypad2,   Key_Keypad3,        Key_Equals,         Key_Quote, \
    ___, ___, Key_Keypad0, Key_KeypadDot, Key_KeypadMultiply, Key_KeypadDivide,   Key_Enter, \
    ___, ___, ___, ___, \
    Key_Keymap1_Momentary \
)

#define QWERTY KEYMAP_STACKED ( \
    ___,          Key_1, Key_2, Key_3, Key_4, Key_5, Key_LEDEffectNext, \
    Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,           \
    Key_PageUp,   Key_A, Key_S, Key_D, Key_F, Key_G,                    \
    Key_PageDown, Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Escape,        \
    Key_LeftControl, Key_Backspace, Key_LeftGui, Key_LeftShift,         \
                          Key_Keymap1_Momentary,     \
\
    Macro_Any,       Key_6, Key_7, Key_8,     Key_9,      Key_0,         Key_ToggleNumlock, \
    Key_Enter,     Key_Y, Key_U, Key_I,     Key_O,      Key_P,         Key_Equals,       \
                   Key_H, Key_J, Key_K,     Key_L,      Key_Semicolon, Key_Quote,       \
    Key_ToggleFPS,  Key_N, Key_M, Key_Comma, Key_Period, Key_Slash,     Key_Minus,       \
    Key_RightShift, Key_LeftAlt, Key_Spacebar, Key_RightControl,                   \
    Key_Keymap1_Momentary \
)

#define FPS_MAP KEYMAP_STACKED ( \
    ___,    Key_5,           Key_6,          Key_7,          Key_8,            Key_9,     Key_0, \
    ___,   Key_3,           Key_4,          Key_FPS_fwd,    Key_FPS_reload,    ___,       Key_FPS_board, \
    Key_2,  Key_FPS_crouch,  Key_FPS_left, Key_FPS_bkwds,   Key_FPS_right,  Key_FPS_use1,      \
    Key_1,  Key_FPS_zoom,      ___,            ___,         Key_FPS_use2,   Key_FPS_use3, Key_FPS_console, \
    Key_FPS_crouch, Key_FPS_jump, Key_FPS_walk, Key_FPS_run,         \
                          Key_FPS_run,     \
\
    ___,       ___, ___, ___,     ___,      ___,         Key_ToggleFPS, \
    ___,     ___, ___, ___,     ___,      ___,         ___,       \
                   ___, ___, ___,     ___,      ___, ___,       \
    ___,  ___, ___, ___, ___, ___,     ___,       \
    ___, ___, ___, ___,                   \
    ___ \
)


const Key keymaps[][ROWS][COLS] PROGMEM = {
  QWERTY,
  GENERIC_FN2,
  NUMPAD,
  FPS_MAP
};

static kaleidoscope::LEDSolidColor solidRed(160, 0, 0);
static kaleidoscope::LEDSolidColor solidOrange(140, 70, 0);
static kaleidoscope::LEDSolidColor solidYellow(130, 100, 0);
static kaleidoscope::LEDSolidColor solidGreen(0, 160, 0);
static kaleidoscope::LEDSolidColor solidBlue(0, 70, 130);
static kaleidoscope::LEDSolidColor solidIndigo(0, 0, 170);
static kaleidoscope::LEDSolidColor solidViolet(130, 0, 120);

const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState) {
  if (macroIndex == TOGGLENUMLOCK && keyToggledOn(keyState)) {
    return NumLock.toggle();
  } else if (macroIndex == TOGGLEFPS && keyToggledOn(keyState)) {
    // because we have this on function-num, we have to go back to the QWERTY
    // layer before we toggle to the FPS layer
    //Layer.previous();
    return FPS.toggle();
  } else if (macroIndex == 1 && keyToggledOn(keyState)) {
    Macros.type(PSTR("Keyboardio Model 01 - Kaleidoscope "));
    Macros.type(PSTR(BUILD_INFORMATION));
  } else if (macroIndex == MACRO_ANY) {
    return MACRODOWN(
        D(LeftControl), D(LeftGui), D(L),
        U(L), U(LeftGui), U(LeftControl)
    );
  }
  return MACRO_NONE;
}

void setup() {
  Kaleidoscope.setup(KEYMAP_SIZE);
  BootKeyboard.begin();
  Kaleidoscope.use(
    &BootGreetingEffect,
    &TestMode,
    &LEDControl, &LEDOff,
    &LEDRainbowEffect, &LEDRainbowWaveEffect, &LEDChaseEffect,
    &solidRed, &solidOrange, &solidYellow, &solidGreen, &solidBlue, &solidIndigo, &solidViolet,
    &LEDBreatheEffect,
    &AlphaSquareEffect,
    &StalkerEffect,
    &NumLock,
    &Macros,
    &MouseKeys,
    &FPS,
    NULL);

  NumLock.numPadLayer = NUMPAD_KEYMAP;
  FPS.FPSLayer = FPS_KEYMAP;
  AlphaSquare.color = { 255, 0, 0 };
  LEDRainbowEffect.brightness(150);
  LEDRainbowWaveEffect.brightness(150);
  StalkerEffect.variant = STALKER(BlazingTrail);
  LEDOff.activate();
}


void loop() {
  Kaleidoscope.loop();
}

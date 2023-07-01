#pragma once
#include "Tassel/Core/Base.h"

namespace Tassel
{
	class TASSEL_API Input
	{
	public:
		static bool IsKeyDown(uint32_t keycode);
		static bool IsMouseButtonDown(uint32_t button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}

/* The unknown key */
#define TASSEL_KEY_UNKNOWN            -1

/* Printable keys */
#define TASSEL_KEY_SPACE              32
#define TASSEL_KEY_APOSTROPHE         39  /* ' */
#define TASSEL_KEY_COMMA              44  /* , */
#define TASSEL_KEY_MINUS              45  /* - */
#define TASSEL_KEY_PERIOD             46  /* . */
#define TASSEL_KEY_SLASH              47  /* / */
#define TASSEL_KEY_0                  48
#define TASSEL_KEY_1                  49
#define TASSEL_KEY_2                  50
#define TASSEL_KEY_3                  51
#define TASSEL_KEY_4                  52
#define TASSEL_KEY_5                  53
#define TASSEL_KEY_6                  54
#define TASSEL_KEY_7                  55
#define TASSEL_KEY_8                  56
#define TASSEL_KEY_9                  57
#define TASSEL_KEY_SEMICOLON          59  /* ; */
#define TASSEL_KEY_EQUAL              61  /* = */
#define TASSEL_KEY_A                  65
#define TASSEL_KEY_B                  66
#define TASSEL_KEY_C                  67
#define TASSEL_KEY_D                  68
#define TASSEL_KEY_E                  69
#define TASSEL_KEY_F                  70
#define TASSEL_KEY_G                  71
#define TASSEL_KEY_H                  72
#define TASSEL_KEY_I                  73
#define TASSEL_KEY_J                  74
#define TASSEL_KEY_K                  75
#define TASSEL_KEY_L                  76
#define TASSEL_KEY_M                  77
#define TASSEL_KEY_N                  78
#define TASSEL_KEY_O                  79
#define TASSEL_KEY_P                  80
#define TASSEL_KEY_Q                  81
#define TASSEL_KEY_R                  82
#define TASSEL_KEY_S                  83
#define TASSEL_KEY_T                  84
#define TASSEL_KEY_U                  85
#define TASSEL_KEY_V                  86
#define TASSEL_KEY_W                  87
#define TASSEL_KEY_X                  88
#define TASSEL_KEY_Y                  89
#define TASSEL_KEY_Z                  90
#define TASSEL_KEY_LEFT_BRACKET       91  /* [ */
#define TASSEL_KEY_BACKSLASH          92  /* \ */
#define TASSEL_KEY_RIGHT_BRACKET      93  /* ] */
#define TASSEL_KEY_GRAVE_ACCENT       96  /* ` */
#define TASSEL_KEY_WORLD_1            161 /* non-US #1 */
#define TASSEL_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define TASSEL_KEY_ESCAPE             256
#define TASSEL_KEY_ENTER              257
#define TASSEL_KEY_TAB                258
#define TASSEL_KEY_BACKSPACE          259
#define TASSEL_KEY_INSERT             260
#define TASSEL_KEY_DELETE             261
#define TASSEL_KEY_RIGHT              262
#define TASSEL_KEY_LEFT               263
#define TASSEL_KEY_DOWN               264
#define TASSEL_KEY_UP                 265
#define TASSEL_KEY_PAGE_UP            266
#define TASSEL_KEY_PAGE_DOWN          267
#define TASSEL_KEY_HOME               268
#define TASSEL_KEY_END                269
#define TASSEL_KEY_CAPS_LOCK          280
#define TASSEL_KEY_SCROLL_LOCK        281
#define TASSEL_KEY_NUM_LOCK           282
#define TASSEL_KEY_PRINT_SCREEN       283
#define TASSEL_KEY_PAUSE              284
#define TASSEL_KEY_F1                 290
#define TASSEL_KEY_F2                 291
#define TASSEL_KEY_F3                 292
#define TASSEL_KEY_F4                 293
#define TASSEL_KEY_F5                 294
#define TASSEL_KEY_F6                 295
#define TASSEL_KEY_F7                 296
#define TASSEL_KEY_F8                 297
#define TASSEL_KEY_F9                 298
#define TASSEL_KEY_F10                299
#define TASSEL_KEY_F11                300
#define TASSEL_KEY_F12                301
#define TASSEL_KEY_F13                302
#define TASSEL_KEY_F14                303
#define TASSEL_KEY_F15                304
#define TASSEL_KEY_F16                305
#define TASSEL_KEY_F17                306
#define TASSEL_KEY_F18                307
#define TASSEL_KEY_F19                308
#define TASSEL_KEY_F20                309
#define TASSEL_KEY_F21                310
#define TASSEL_KEY_F22                311
#define TASSEL_KEY_F23                312
#define TASSEL_KEY_F24                313
#define TASSEL_KEY_F25                314
#define TASSEL_KEY_KP_0               320
#define TASSEL_KEY_KP_1               321
#define TASSEL_KEY_KP_2               322
#define TASSEL_KEY_KP_3               323
#define TASSEL_KEY_KP_4               324
#define TASSEL_KEY_KP_5               325
#define TASSEL_KEY_KP_6               326
#define TASSEL_KEY_KP_7               327
#define TASSEL_KEY_KP_8               328
#define TASSEL_KEY_KP_9               329
#define TASSEL_KEY_KP_DECIMAL         330
#define TASSEL_KEY_KP_DIVIDE          331
#define TASSEL_KEY_KP_MULTIPLY        332
#define TASSEL_KEY_KP_SUBTRACT        333
#define TASSEL_KEY_KP_ADD             334
#define TASSEL_KEY_KP_ENTER           335
#define TASSEL_KEY_KP_EQUAL           336
#define TASSEL_KEY_LEFT_SHIFT         340
#define TASSEL_KEY_LEFT_CONTROL       341
#define TASSEL_KEY_LEFT_ALT           342
#define TASSEL_KEY_LEFT_SUPER         343
#define TASSEL_KEY_RIGHT_SHIFT        344
#define TASSEL_KEY_RIGHT_CONTROL      345
#define TASSEL_KEY_RIGHT_ALT          346
#define TASSEL_KEY_RIGHT_SUPER        347
#define TASSEL_KEY_MENU               348

#define TASSEL_KEY_LAST               TASSEL_KEY_MENU

// Mouse Button
#define TASSEL_MOUSE_BUTTON_1         0
#define TASSEL_MOUSE_BUTTON_2         1
#define TASSEL_MOUSE_BUTTON_3         2
#define TASSEL_MOUSE_BUTTON_4         3
#define TASSEL_MOUSE_BUTTON_5         4
#define TASSEL_MOUSE_BUTTON_6         5
#define TASSEL_MOUSE_BUTTON_7         6
#define TASSEL_MOUSE_BUTTON_8         7
#define TASSEL_MOUSE_BUTTON_LAST      TASSEL_MOUSE_BUTTON_8
#define TASSEL_MOUSE_BUTTON_LEFT      TASSEL_MOUSE_BUTTON_1
#define TASSEL_MOUSE_BUTTON_RIGHT     TASSEL_MOUSE_BUTTON_2
#define TASSEL_MOUSE_BUTTON_MIDDLE    TASSEL_MOUSE_BUTTON_3

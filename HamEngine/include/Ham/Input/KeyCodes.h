#pragma once

#include <stdint.h>
#include <iostream>

namespace Ham
{

	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		SPACE = 32,
		APOSTROPHE = 39, /* ' */
		COMMA = 44,		 /* , */
		MINUS = 45,		 /* - */
		PERIOD = 46,	 /* . */
		SLASH = 47,		 /* / */

		DIGIT_0 = 48, /* 0 */
		DIGIT_1 = 49, /* 1 */
		DIGIT_2 = 50, /* 2 */
		DIGIT_3 = 51, /* 3 */
		DIGIT_4 = 52, /* 4 */
		DIGIT_5 = 53, /* 5 */
		DIGIT_6 = 54, /* 6 */
		DIGIT_7 = 55, /* 7 */
		DIGIT_8 = 56, /* 8 */
		DIGIT_9 = 57, /* 9 */

		SEMICOLON = 59, /* ; */
		EQUAL = 61,		/* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LEFT_BRACKET = 91,	/* [ */
		BACKSLASH = 92,		/* \ */
		RIGHT_BRACKET = 93, /* ] */
		TILDE = 96,			/* ` */

		WORRLD_1 = 161, /* non-US #1 */
		WORRLD_2 = 162, /* non-US #2 */

		/* Function keys */
		ESCAPE = 256,
		ENTER = 257,
		TAB = 258,
		BACKSPACE = 259,
		INSERT = 260,
		DELETE = 261,
		RIGHT = 262,
		LEFT = 263,
		DOWN = 264,
		UP = 265,
		PAGE_UP = 266,
		PAGE_DOWN = 267,
		HOME = 268,
		END = 269,
		CAPS_LOCK = 280,
		SCROLL_LOCK = 281,
		NUM_LOCK = 282,
		PRINT_SCREEN = 283,
		PAUSE = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KEYPAD_0 = 320,
		KEYPAD_1 = 321,
		KEYPAD_2 = 322,
		KEYPAD_3 = 323,
		KEYPAD_4 = 324,
		KEYPAD_5 = 325,
		KEYPAD_6 = 326,
		KEYPAD_7 = 327,
		KEYPAD_8 = 328,
		KEYPAD_9 = 329,
		KEYPAD_DECIMAL = 330,
		KEYPAD_SLASH = 331,
		KEYPAD_ASTRIX = 332,
		KEYPAD_MINUS = 333,
		KEYPAD_PLUS = 334,
		KEYPAD_ENTER = 335,
		KEYPAD_EQUAL = 336,

		LEFT_SHIFT = 340,
		LEFT_CONTROL = 341,
		LEFT_ALT = 342,
		LEFT_SUPER = 343,
		RIGHT_SHIFT = 344,
		RIGHT_CONTROL = 345,
		RIGHT_ALT = 346,
		RIGHT_SUPER = 347,
		MENU = 348
	} Key;

	enum class KeyState
	{
		NONE = -1,
		PRESSED,
		HELD,
		RELEASED
	};

	enum class CursorMode
	{
		NORMAL = 0,
		HIDDEN,
		CAPTURED
	};

	typedef enum class MouseButton : uint16_t
	{
		BUTTON_0 = 0,
		BUTTON_1 = 1,
		BUTTON_2 = 2,
		BUTTON_3 = 3,
		BUTTON_4 = 4,
		BUTTON_5 = 5,
		LEFT = BUTTON_0,
		RIGHT = BUTTON_1,
		MIDDLE = BUTTON_2
	} Button;

	inline std::ostream &operator<<(std::ostream &os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}

	inline std::ostream &operator<<(std::ostream &os, MouseButton button)
	{
		os << static_cast<int32_t>(button);
		return os;
	}
}

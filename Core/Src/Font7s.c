// Font size 7 is a 7 segment font intended to display numbers and time
// This font only contains characters [space] 0 1 2 3 4 5 6 7 8 9 : .
// All other characters print as a space
#include "fonts.h"
const uint8_t Fontf7s_Table[] =          // character width table
		{ 12, 12, 12, 12, 12, 12, 12, 12,     // char 32 - 39
				12, 12, 12, 12, 12, 12, 12, 12,     // char 40 - 47
				32, 32, 32, 32, 32, 32, 32, 32,     // char 48 - 55
				32, 32, 12, 12, 12, 12, 12, 12,     // char 56 - 63
				12, 12, 12, 12, 12, 12, 12, 12,     // char 64 - 71
				12, 12, 12, 12, 12, 12, 12, 12,     // char 72 - 79
				12, 12, 12, 12, 12, 12, 12, 12,     // char 80 - 87
				12, 12, 12, 12, 12, 12, 12, 12,     // char 88 - 95
				12, 12, 12, 12, 12, 12, 12, 12,     // char 96 - 103
				12, 12, 12, 12, 12, 12, 12, 12,     // char 104 - 111
				12, 12, 12, 12, 12, 12, 12, 12,     // char 112 - 119
				12, 12, 12, 12, 12, 12, 12, 12      // char 120 - 127
		};

sFONT Font7s = { Fontf7s_Table, 5, /* Width */
8, /* Height */
1, /* Size = bytes per pixel row */
};

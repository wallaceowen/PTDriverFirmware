#ifndef __cfield_h
#define __cfield_h

enum Align { AlignLeft, AlignRight, AlignCenter };

struct Field
{
	Field(uint8_t w, uint16_t _x, uint16_t _y, Align a) :
		width(w), x(_x), y(_y), align(a)
	{
	}

    uint8_t width;
	uint16_t x, y;
	Align align;
};

extern void render_field(U8G2 &disp, const Field &field, const char *txt);

#endif

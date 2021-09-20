
#include <U8g2lib.h>  // LCD driver library

#include "cfield.h"

void render_field(U8G2 &disp, const Field &field, const char *txt)
{
    u8g2_uint_t w = disp.getStrWidth(txt);
    int cx;
    switch(field.align)
    {
        case AlignLeft:
        {
            cx = field.x;
            break;
        }
        case AlignRight:
        {
            cx = field.x+(field.width-w);
            break;
        }
        case AlignCenter:
        {
            cx = (field.x+field.width/2)-(w/2);
            break;
        }
    }

    int height = disp.getMaxCharHeight();
    disp.drawStr(cx, height+field.y, txt);
}

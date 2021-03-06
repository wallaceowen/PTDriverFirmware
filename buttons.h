// buttons.h

#ifndef __buttons_h
#define __buttons_h

#include <stdint.h>

typedef void (*ButtonsCB)(int button, bool state, void *arg);

class Buttons
{
public:
    enum Constants { DownButton=0, UpButton=1, Btn_Num=2 };

    Buttons();

    Buttons(ButtonsCB cb, void*arg);

    void set_cb(ButtonsCB cb, void *arg);

    bool pressed(int which);

    void loop(void);
private:
    ButtonsCB m_cb;
    void     *m_cbarg;
    int       m_last[Btn_Num];
};


#endif

#ifndef _FRAME_GO_H_
#define _FRAME_GO_H_

#include "frame_base.h"
#include "../epdgui/epdgui.h"

class Frame_Go : public Frame_Base
{
public:
    Frame_Go();
    ~Frame_Go();
    int init(epdgui_args_vector_t &args);
    int run();
    void exit();

private:
    M5EPD_Canvas *cells_0;
    M5EPD_Canvas *cells_1;
    M5EPD_Canvas *canvas_0;
    M5EPD_Canvas *canvas_1;
};

#endif //_FRAME_GO_H_
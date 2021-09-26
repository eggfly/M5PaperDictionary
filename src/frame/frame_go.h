#ifndef _FRAME_GO_H_
#define _FRAME_GO_H_

#include "frame_base.h"
#include "../epdgui/epdgui.h"
#include "../epdgui/epdgui_checkerboard.h"
#include "../epdgui/epdgui_button.h"

class Frame_Go : public Frame_Base
{
public:
    Frame_Go();
    ~Frame_Go();
    int init(epdgui_args_vector_t &args);
    int run();
    void exit();
    EPDGUI_Checkerboard *_checkerboard;

private:
    EPDGUI_Button *_black_button;
    EPDGUI_Button *_white_button;
    EPDGUI_Button *_remove_button;
};

#endif //_FRAME_GO_H_
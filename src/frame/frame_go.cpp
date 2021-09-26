#include "frame_go.h"

int Frame_Go::run()
{
    Frame_Base::run();
    M5.update();
    if (M5.BtnP.wasReleased())
    {
        EPDGUI_UpdateGlobalLastActiveTime();
        _checkerboard->Randomize();
    }
    _checkerboard->UpdateDraw();
    return 1;
}

void Frame_Go::exit()
{
}

Frame_Go::Frame_Go(void)
{
    _frame_name = "Frame_Go";

    uint8_t language = GetLanguage();
    if (language == LANGUAGE_JA)
    {
        exitbtn("ホーム");
        _canvas_title->drawString("囲碁/五目並べ", 270, 34);
    }
    else if (language == LANGUAGE_ZH)
    {
        exitbtn("主页");
        _canvas_title->drawString("围棋/五子棋", 270, 34);
    }
    else
    {
        exitbtn("Home");
        _canvas_title->drawString("Go/FIR", 270, 34);
    }

    _key_exit->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void *)(&_is_run));
    _key_exit->Bind(EPDGUI_Button::EVENT_RELEASED, &Frame_Base::exit_cb);
}

Frame_Go::~Frame_Go(void)
{
}

void go_black_button_cb(epdgui_args_vector_t &args)
{
    log_d("WriteCurrentAsBlack");
    Frame_Go *thiz = (Frame_Go *)(args[0]);
    thiz->_checkerboard->WriteCurrentAsBlack();
}

void go_white_button_cb(epdgui_args_vector_t &args)
{
    log_d("WriteCurrentAsWhite");
    Frame_Go *thiz = (Frame_Go *)(args[0]);
    thiz->_checkerboard->WriteCurrentAsWhite();
}

void go_remove_button_cb(epdgui_args_vector_t &args)
{
    log_d("RemoveCurrent");
    Frame_Go *thiz = (Frame_Go *)(args[0]);
    thiz->_checkerboard->RemoveCurrent();
}

int Frame_Go::init(epdgui_args_vector_t &args)
{
    _is_run = 1;
    _checkerboard = new EPDGUI_Checkerboard();
    const uint32_t BTN_WIDTH = 130;
    const uint32_t BTN_HEIGHT = 80;
    const uint32_t BTN_TOP = 800;
    const uint32_t BTN_MARGIN = 35;
    _black_button = new EPDGUI_Button("Black", BTN_MARGIN, BTN_TOP, BTN_WIDTH, BTN_HEIGHT);
    _black_button->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void *)this);
    _black_button->Bind(EPDGUI_Button::EVENT_RELEASED, go_black_button_cb);
    _white_button = new EPDGUI_Button("White", BTN_MARGIN + BTN_MARGIN + BTN_WIDTH, BTN_TOP, BTN_WIDTH, BTN_HEIGHT);
    _white_button->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void *)this);
    _white_button->Bind(EPDGUI_Button::EVENT_RELEASED, go_white_button_cb);
    _remove_button = new EPDGUI_Button("Remove", BTN_MARGIN + (BTN_MARGIN + BTN_WIDTH) * 2, BTN_TOP, BTN_WIDTH, BTN_HEIGHT);
    _remove_button->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void *)this);
    _remove_button->Bind(EPDGUI_Button::EVENT_RELEASED, go_remove_button_cb);
    M5.EPD.Clear();
    _canvas_title->pushCanvas(0, 8, UPDATE_MODE_NONE);
    EPDGUI_AddObject(_key_exit);
    EPDGUI_AddObject(_checkerboard);
    EPDGUI_AddObject(_black_button);
    EPDGUI_AddObject(_white_button);
    EPDGUI_AddObject(_remove_button);
    return 3;
}
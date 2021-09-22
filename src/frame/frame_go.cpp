#include "frame_go.h"

// #define CELL_W 10
// #define CELL_H 10
// #define CELL_X 54 + 2
// #define CELL_Y 88 + 2


#define CELL_W 4
#define CELL_H 4
#define CELL_X 135 + 2
#define CELL_Y 222 + 2

// #define CELL_W 2
// #define CELL_H 2
// #define CELL_X 270 + 2
// #define CELL_Y 444 + 2

static uint8_t lifegame_cells_flag = 0;
static M5EPD_Canvas lifegame_cells_0(&M5.EPD);
static M5EPD_Canvas lifegame_cells_1(&M5.EPD);
static M5EPD_Canvas lifegame_canvas_0(&M5.EPD);
static M5EPD_Canvas lifegame_canvas_1(&M5.EPD);

void Go_Randomize()
{
    lifegame_cells_flag = 0;
    uint8_t randbuf[CELL_X];
    for (int y = 0; y < CELL_Y; y++)
    {
        esp_fill_random(randbuf, CELL_X);
        for (int x = 0; x < CELL_X; x++)
        {
            lifegame_cells_0.drawPixel(x, y, randbuf[x] & 0x01 ? 15 : 0);
        }
    }
    lifegame_cells_1.fillCanvas(0);
}

int Frame_Go::run()
{
    Frame_Base::run();
    M5.update();
    if(M5.BtnP.wasReleased())
    {
        EPDGUI_UpdateGlobalLastActiveTime();
        Go_Randomize();
    }
    if(lifegame_cells_flag == 0)
    {
        lifegame_canvas_1.pushCanvas(0, 72, UPDATE_MODE_DU4);
    }
    else
    {
        lifegame_canvas_0.pushCanvas(0, 72, UPDATE_MODE_DU4);
    }
    log_d("");
    return 1;
}

void Frame_Go::exit()
{
    lifegame_cells_0.deleteCanvas();
    lifegame_cells_1.deleteCanvas();
    lifegame_canvas_0.deleteCanvas();
    lifegame_canvas_1.deleteCanvas();
}

Frame_Go::Frame_Go(void)
{
    _frame_name = "Frame_Go";
 
    uint8_t language = GetLanguage();
    if(language == LANGUAGE_JA)
    {
        exitbtn("ホーム");
        _canvas_title->drawString("囲碁", 270, 34);
    }
    else if(language == LANGUAGE_ZH)
    {
        exitbtn("主页");
        _canvas_title->drawString("围棋", 270, 34);
    }
    else
    {
        exitbtn("Home");
        _canvas_title->drawString("Go", 270, 34);
    }

    _key_exit->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _key_exit->Bind(EPDGUI_Button::EVENT_RELEASED, &Frame_Base::exit_cb);
}

Frame_Go::~Frame_Go(void)
{
}

int Frame_Go::init(epdgui_args_vector_t &args)
{
    _is_run = 1;

    lifegame_cells_0.createCanvas(CELL_X, CELL_Y);
    lifegame_cells_1.createCanvas(CELL_X, CELL_Y);
    lifegame_canvas_0.createCanvas(540, 888);
    lifegame_canvas_1.createCanvas(540, 888);

    lifegame_cells_0.fillCanvas(0);
    lifegame_cells_1.fillCanvas(0);
    lifegame_canvas_0.fillCanvas(0);
    lifegame_canvas_1.fillCanvas(0);

    lifegame_cells_0.pushImage(2, 2, 132, 120, ImageResource_lifegame_seeder_132x120);
    lifegame_cells_flag = 0;

    M5.EPD.Clear();
    _canvas_title->pushCanvas(0, 8, UPDATE_MODE_NONE);
    EPDGUI_AddObject(_key_exit);
    return 3;
}
#ifndef _FRAME_MUSIC_PLAYER_H_
#define _FRAME_MUSIC_PLAYER_H_

#include "frame_base.h"
#include "../epdgui/epdgui.h"

#include "player/player.hpp"


class Frame_MusicPlayer : public Frame_Base {
   public:
    Frame_MusicPlayer();
    ~Frame_MusicPlayer();
    int init(epdgui_args_vector_t &args);
    int run();
    void exit();

   private:
    M5EPD_Canvas *cells_0;
    M5EPD_Canvas *cells_1;
    M5EPD_Canvas *canvas_0;
    M5EPD_Canvas *canvas_1;

    singsang::CPlayer player;

};

#endif  //_FRAME_MUSIC_PLAYER_H_

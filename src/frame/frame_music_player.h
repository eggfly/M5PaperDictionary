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
    M5EPD_Canvas *_info_canvas;
    singsang::CPlayer player;
    const char * m_currentSongPath;
    size_t m_vinylAnimIndex = 0;
    int32_t m_lastAnimTime = 0;
};

#endif  //_FRAME_MUSIC_PLAYER_H_

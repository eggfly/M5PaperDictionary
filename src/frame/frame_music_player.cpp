#include "frame_music_player.h"


int Frame_MusicPlayer::run() {
    Frame_Base::run();
    M5.update();
    // Serial.println("l");
    player.loop();
    if (M5.BtnR.wasReleased()) {
        player.increaseVolume();
    } else if (M5.BtnL.wasReleased()) {
        player.decreaseVolume();
    }
    if (M5.BtnP.wasReleased()) {
        player.startNextSong();
        EPDGUI_UpdateGlobalLastActiveTime();
    }
    return 1;
}

void Frame_MusicPlayer::exit() {
    // lifegame_cells_0.deleteCanvas();
    // lifegame_cells_1.deleteCanvas();
    // lifegame_canvas_0.deleteCanvas();
    // lifegame_canvas_1.deleteCanvas();
}

Frame_MusicPlayer::Frame_MusicPlayer(void) {
    _frame_name = "Frame_MusicPlayer";

    uint8_t language = GetLanguage();
    if (language == LANGUAGE_JA) {
        exitbtn("ホーム");
        _canvas_title->drawString("音楽プレーヤー", 270, 34);
    } else if (language == LANGUAGE_ZH) {
        exitbtn("主页");
        _canvas_title->drawString("音乐播放器", 270, 34);
    } else {
        exitbtn("Home");
        _canvas_title->drawString("Music Player", 270, 34);
    }

    _key_exit->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void *)(&_is_run));
    _key_exit->Bind(EPDGUI_Button::EVENT_RELEASED, &Frame_Base::exit_cb);
}

Frame_MusicPlayer::~Frame_MusicPlayer(void) {
}

int Frame_MusicPlayer::init(epdgui_args_vector_t &args) {
    _is_run = 1;
    setWakeLock(true);

    M5.EPD.Clear();
    _canvas_title->pushCanvas(0, 8, UPDATE_MODE_NONE);
    EPDGUI_AddObject(_key_exit);
    player.begin();
    return 3;
}
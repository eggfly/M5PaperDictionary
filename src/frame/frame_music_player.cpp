#include "frame_music_player.h"
#include "../utils.h"

char music_title[128];
char music_album[128];
char music_artist[128];

bool audioInfoNeedUpdate = false;

const uint8_t *kVinylAnim[] = {
    ImageResource_vinyl_0_200x200,
    ImageResource_vinyl_1_200x200,
    ImageResource_vinyl_2_200x200, 
    ImageResource_vinyl_3_200x200,
    ImageResource_vinyl_4_200x200,
    ImageResource_vinyl_5_200x200,
    ImageResource_vinyl_6_200x200,
    ImageResource_vinyl_7_200x200,
    ImageResource_vinyl_8_200x200,
    ImageResource_vinyl_9_200x200,
    ImageResource_vinyl_10_200x200,
    ImageResource_vinyl_11_200x200,
};

void updateSongInfoCanvas(M5EPD_Canvas* canvas) {
  canvas->pushCanvas(0, 960 - 300,
                               UPDATE_MODE_GC16); // UPDATE_MODE_GC16
}

int Frame_MusicPlayer::run() {
  Frame_Base::run();
  M5.update();
  // Serial.println("l");
  player.loop();
  const char *currSongPath = player.getCurrentSongPath();
  bool needRedraw = false;
  if (m_currentSongPath != currSongPath) {
    m_currentSongPath = currSongPath;
    needRedraw = true;
  }
  if (audioInfoNeedUpdate) {
    needRedraw = true;
    audioInfoNeedUpdate = false;
  }
  if (needRedraw) {
    _info_canvas->fillCanvas(0);
    _info_canvas->drawString(m_currentSongPath, 80, 20);
    _info_canvas->drawString(music_title, 80, 20 + 32);
    _info_canvas->drawString(music_artist, 80, 20 + 32 * 2);
    _info_canvas->drawString(music_album, 80, 20 + 32 * 3);
    updateSongInfoCanvas(_info_canvas);
    log_d("pushCanvas: %s", currSongPath);
  }
  auto t = millis();
  if (t - m_lastAnimTime > 1000) {
    uint16_t x = (540 - 200) / 2;
    uint16_t y = (960 - 200) / 2;
    uint16_t size = 200;
    M5.EPD.WritePartGram4bpp(x, y, size, size, kVinylAnim[m_vinylAnimIndex]);
    M5.EPD.UpdateArea(x, y, size, size,
                      UPDATE_MODE_A2); // UPDATE_MODE_GC16, UPDATE_MODE_GL16,
                                       // UPDATE_MODE_DU4, UPDATE_MODE_A2
    m_lastAnimTime = t;
    m_vinylAnimIndex++;
    m_vinylAnimIndex %= 12;
  }
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
}

const uint16_t kAudioInfoTextSize = 26;

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

    _canvas_title->pushCanvas(0, 8, UPDATE_MODE_NONE);
    EPDGUI_AddObject(_key_exit);

    _info_canvas = new M5EPD_Canvas(&M5.EPD);
    _info_canvas->createCanvas(540, 32 * 4);
    _info_canvas->setTextDatum(CC_DATUM);
    _info_canvas->createRender(kAudioInfoTextSize, 128);
    // _info_canvas->fillRect(0, 0, 540, 32 * 4, 8);
    _info_canvas->setTextSize(kAudioInfoTextSize);
    _info_canvas->setTextColor(15);
}

Frame_MusicPlayer::~Frame_MusicPlayer(void) {
}

int Frame_MusicPlayer::init(epdgui_args_vector_t &args) {
    _is_run = 1;
    setWakeLock(true);

    M5.EPD.Clear();

    player.begin();
    return 3;
}

void audio_id3data(const char *info) {
  Serial.print("id3data     ");
  Serial.println(info);

  if (startsWithIgnoreCase("TITLE", info)) {
    strncpy(music_title, info + sizeof("TITLE") + 1, sizeof(music_title) / sizeof(music_title[0]));
    audioInfoNeedUpdate = true;
  } else if (startsWithIgnoreCase("ALBUM", info)) {
    strncpy(music_album, info + sizeof("ALBUM") + 1, sizeof(music_album) / sizeof(music_album[0]));
    audioInfoNeedUpdate = true;
  } else if (startsWithIgnoreCase("ARTIST", info)) {
    strncpy(music_artist, info + sizeof("ARTIST") + 1, sizeof(music_artist) / sizeof(music_artist[0]));
    audioInfoNeedUpdate = true;
  } else {
    audioInfoNeedUpdate = false;
  }
  if (audioInfoNeedUpdate) {
    // M5.Lcd.fillRect(0, 240 - 81, 320, 81, TFT_BLACK);
    // M5.Lcd.drawString(music_title, 0, 240 - 81);
    // M5.Lcd.drawString(music_artist, 0, 240 - 54);
    // M5.Lcd.drawString(music_album, 0, 240 - 27);
  }
}

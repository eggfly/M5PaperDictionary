#include "frame_main.h"
#include "frame_setting.h"
#include "frame_keyboard.h"
#include "frame_factorytest.h"
#include "frame_wifiscan.h"
#include "frame_lifegame.h"
#include "frame_fileindex.h"
#include "frame_compare.h"
#include "frame_home.h"
#include "frame_music_player.h"

enum LauncherApps {
    kKeyFactoryTest = 0,
    kKeySetting,
    kKeyKeyboard,
    kKeyWifiScan,
    kKeySDFile,
    kKeyCompare,
    kKeyHome,
    kKeyLifeGame,
    kKeyMusicPlayer,
    kAppsCountAsLastItem,
};


#define KEY_W 92
#define KEY_H 92

void key_setting_cb(epdgui_args_vector_t &args) {
    Frame_Base *frame = EPDGUI_GetFrame("Frame_Setting");
    if (frame == NULL) {
        frame = new Frame_Setting();
        EPDGUI_AddFrame("Frame_Setting", frame);
    }
    EPDGUI_PushFrame(frame);
    *((int *)(args[0])) = 0;
}

void key_keyboard_cb(epdgui_args_vector_t &args) {
    Frame_Base *frame = EPDGUI_GetFrame("Frame_Keyboard");
    if (frame == NULL) {
        frame = new Frame_Keyboard(0);
        EPDGUI_AddFrame("Frame_Keyboard", frame);
    }
    EPDGUI_PushFrame(frame);
    *((int *)(args[0])) = 0;
}

void key_factorytest_cb(epdgui_args_vector_t &args) {
    Frame_Base *frame = EPDGUI_GetFrame("Frame_FactoryTest");
    if (frame == NULL) {
        frame = new Frame_FactoryTest();
        EPDGUI_AddFrame("Frame_FactoryTest", frame);
    }
    EPDGUI_PushFrame(frame);
    *((int *)(args[0])) = 0;
}

void key_wifiscan_cb(epdgui_args_vector_t &args) {
    Frame_Base *frame = EPDGUI_GetFrame("Frame_WifiScan");
    if (frame == NULL) {
        frame = new Frame_WifiScan();
        EPDGUI_AddFrame("Frame_WifiScan", frame);
    }
    EPDGUI_PushFrame(frame);
    *((int *)(args[0])) = 0;
}

void key_lifegame_cb(epdgui_args_vector_t &args) {
    Frame_Base *frame = EPDGUI_GetFrame("Frame_Lifegame");
    if (frame == NULL) {
        frame = new Frame_Lifegame();
        EPDGUI_AddFrame("Frame_Lifegame", frame);
    }
    EPDGUI_PushFrame(frame);
    *((int *)(args[0])) = 0;
}

void key_sdfile_cb(epdgui_args_vector_t &args) {
    Frame_Base *frame = new Frame_FileIndex("/");
    EPDGUI_PushFrame(frame);
    *((int *)(args[0])) = 0;
}

void key_compare_cb(epdgui_args_vector_t &args) {
    Frame_Base *frame = EPDGUI_GetFrame("Frame_Compare");
    if (frame == NULL) {
        frame = new Frame_Compare();
        EPDGUI_AddFrame("Frame_Compare", frame);
    }
    EPDGUI_PushFrame(frame);
    *((int *)(args[0])) = 0;
}

void key_home_cb(epdgui_args_vector_t &args) {
    Frame_Base *frame = EPDGUI_GetFrame("Frame_Home");
    if (frame == NULL) {
        frame = new Frame_Home();
        EPDGUI_AddFrame("Frame_Home", frame);
    }
    EPDGUI_PushFrame(frame);
    *((int *)(args[0])) = 0;
}


void key_music_player_cb(epdgui_args_vector_t &args) {
    Frame_Base *frame = new Frame_MusicPlayer();
    EPDGUI_PushFrame(frame);
    *((int *)(args[0])) = 0;
}


Frame_Main::Frame_Main(void) : Frame_Base(false) {
    _frame_name = "Frame_Main";
    _frame_id   = 1;

    log_e("before new M5EPD_Canvas");
    _bar = new M5EPD_Canvas(&M5.EPD);
    log_e("before createCanvas");
    _bar->createCanvas(540, 44);
    log_e("before setTextSize");
    _bar->setTextSize(26);

    log_e("before M5EPD_Canvas 2");
    _names = new M5EPD_Canvas(&M5.EPD);
    _names->createCanvas(540, 32);
    _names->setTextDatum(CC_DATUM);

    _player = new M5EPD_Canvas(&M5.EPD);
    _player->createCanvas(540, 380);

    log_e("before for loop 1");
    for (int i = 0; i < 4; i++) {
        _key[i] = new EPDGUI_Button("测试", 20 + i * 136, 90, KEY_W, KEY_H);
    }

    log_e("before for loop 2");
    for (int i = 0; i < 4; i++) {
        _key[i + 4] =
            new EPDGUI_Button("测试", 20 + i * 136, 90+150, KEY_W, KEY_H);
    }
    // 9. MusicPlayer
    _key[8] = new EPDGUI_Button("测试", 20 + 0 * 136, 90+150*2, KEY_W, KEY_H);

    _key[kKeySetting]->CanvasNormal()->pushImage(
        0, 0, 92, 92, ImageResource_main_icon_setting_92x92);
    *(_key[kKeySetting]->CanvasPressed()) =
        *(_key[kKeySetting]->CanvasNormal());
    _key[kKeySetting]->CanvasPressed()->ReverseColor();
    _key[kKeySetting]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0,
                               (void *)(&_is_run));
    _key[kKeySetting]->Bind(EPDGUI_Button::EVENT_RELEASED, key_setting_cb);

    _key[kKeyKeyboard]->CanvasNormal()->pushImage(
        0, 0, 92, 92, ImageResource_main_icon_keyboard_92x92);
    *(_key[kKeyKeyboard]->CanvasPressed()) =
        *(_key[kKeyKeyboard]->CanvasNormal());
    _key[kKeyKeyboard]->CanvasPressed()->ReverseColor();
    _key[kKeyKeyboard]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0,
                                (void *)(&_is_run));
    _key[kKeyKeyboard]->Bind(EPDGUI_Button::EVENT_RELEASED, key_keyboard_cb);

    _key[kKeyFactoryTest]->CanvasNormal()->pushImage(
        0, 0, 92, 92, ImageResource_main_icon_factorytest_92x92);
    *(_key[kKeyFactoryTest]->CanvasPressed()) =
        *(_key[kKeyFactoryTest]->CanvasNormal());
    _key[kKeyFactoryTest]->CanvasPressed()->ReverseColor();
    _key[kKeyFactoryTest]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0,
                                   (void *)(&_is_run));
    _key[kKeyFactoryTest]->Bind(EPDGUI_Button::EVENT_RELEASED,
                                key_factorytest_cb);

    _key[kKeyWifiScan]->CanvasNormal()->pushImage(
        0, 0, 92, 92, ImageResource_main_icon_wifi_92x92);
    *(_key[kKeyWifiScan]->CanvasPressed()) =
        *(_key[kKeyWifiScan]->CanvasNormal());
    _key[kKeyWifiScan]->CanvasPressed()->ReverseColor();
    _key[kKeyWifiScan]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0,
                                (void *)(&_is_run));
    _key[kKeyWifiScan]->Bind(EPDGUI_Button::EVENT_RELEASED, key_wifiscan_cb);

    _key[kKeyLifeGame]->CanvasNormal()->pushImage(
        0, 0, 92, 92, ImageResource_main_icon_lifegame_92x92);
    *(_key[kKeyLifeGame]->CanvasPressed()) =
        *(_key[kKeyLifeGame]->CanvasNormal());
    _key[kKeyLifeGame]->CanvasPressed()->ReverseColor();
    _key[kKeyLifeGame]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0,
                                (void *)(&_is_run));
    _key[kKeyLifeGame]->Bind(EPDGUI_Button::EVENT_RELEASED, key_lifegame_cb);

    _key[kKeySDFile]->CanvasNormal()->pushImage(
        0, 0, 92, 92, ImageResource_main_icon_sdcard_92x92);
    *(_key[kKeySDFile]->CanvasPressed()) = *(_key[kKeySDFile]->CanvasNormal());
    _key[kKeySDFile]->CanvasPressed()->ReverseColor();
    _key[kKeySDFile]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0,
                              (void *)(&_is_run));
    _key[kKeySDFile]->Bind(EPDGUI_Button::EVENT_RELEASED, key_sdfile_cb);

    _key[kKeyCompare]->CanvasNormal()->pushImage(
        0, 0, 92, 92, ImageResource_main_icon_compare_92x92);
    *(_key[kKeyCompare]->CanvasPressed()) =
        *(_key[kKeyCompare]->CanvasNormal());
    _key[kKeyCompare]->CanvasPressed()->ReverseColor();
    _key[kKeyCompare]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0,
                               (void *)(&_is_run));
    _key[kKeyCompare]->Bind(EPDGUI_Button::EVENT_RELEASED, key_compare_cb);

    _key[kKeyHome]->CanvasNormal()->pushImage(
        0, 0, 92, 92, ImageResource_main_icon_home_92x92);
    *(_key[kKeyHome]->CanvasPressed()) = *(_key[kKeyHome]->CanvasNormal());
    _key[kKeyHome]->CanvasPressed()->ReverseColor();
    _key[kKeyHome]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0,
                            (void *)(&_is_run));
    _key[kKeyHome]->Bind(EPDGUI_Button::EVENT_RELEASED, key_home_cb);

    _key[kKeyMusicPlayer]->CanvasNormal()->pushImage(
        0, 0, 92, 92, ImageResource_player_icon_92x92);
    *(_key[kKeyMusicPlayer]->CanvasPressed()) =
        *(_key[kKeyMusicPlayer]->CanvasNormal());
    _key[kKeyMusicPlayer]->CanvasPressed()->ReverseColor();
    _key[kKeyMusicPlayer]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0,
                               (void *)(&_is_run));
    _key[kKeyMusicPlayer]->Bind(EPDGUI_Button::EVENT_RELEASED, key_music_player_cb);

    _time             = 0;
    _next_update_time = 0;
    log_e("Frame_Main::Frame_Main() finished");
}

Frame_Main::~Frame_Main(void) {
    for (int i = 0; i < kAppsCountAsLastItem; i++) {
        delete _key[i];
    }
}

void Frame_Main::AppName(m5epd_update_mode_t mode) {
    uint16_t render_size = 22;
    if (!_names->isRenderExist(render_size)) {
        _names->createRender(render_size, 26);
    }
    _names->setTextSize(render_size);
    _names->fillCanvas(0);
    uint8_t language = GetLanguage();
    _names->drawString("WLAN", 20 + 46 + 3 * 136, 16);
    if (language == LANGUAGE_JA) {
        _names->drawString("工場テスト", 20 + 46, 16);
        _names->drawString("設定", 20 + 46 + 136, 16);
        _names->drawString("鍵盤", 20 + 46 + 2 * 136, 16);
    } else if (language == LANGUAGE_ZH) {
        _names->drawString("出厂测试", 20 + 46, 16);
        _names->drawString("设定", 20 + 46 + 136, 16);
        _names->drawString("键盘", 20 + 46 + 2 * 136, 16);
    } else {
        _names->drawString("Test", 20 + 46, 16);
        _names->drawString("Setting", 20 + 46 + 136, 16);
        _names->drawString("Keyboard", 20 + 46 + 2 * 136, 16);
    }
    _names->pushCanvas(0, 186, mode);

    _names->fillCanvas(0);
    if (language == LANGUAGE_JA) {
        _names->drawString("メモリー", 20 + 46, 16);
        _names->drawString("刷新比較", 20 + 46 + 136, 16);
        _names->drawString("家", 20 + 46 + 2 * 136, 16);
        _names->drawString("ライフゲーム", 20 + 46 + 3 * 136, 16);
    } else if (language == LANGUAGE_ZH) {
        _names->drawString("存储", 20 + 46, 16);
        _names->drawString("刷新比较", 20 + 46 + 136, 16);
        _names->drawString("家", 20 + 46 + 2 * 136, 16);
        _names->drawString("生命游戏", 20 + 46 + 3 * 136, 16);
    } else {
        _names->drawString("Storage", 20 + 46, 16);
        _names->drawString("Compare", 20 + 46 + 136, 16);
        _names->drawString("Home", 20 + 46 + 2 * 136, 16);
        _names->drawString("LifeGame", 20 + 46 + 3 * 136, 16);
    }
    // _player->pushImage(0,0, 540, 380, ImageResource_player_540x380);
    // Serial.println("my push1");
    _names->pushCanvas(0, 186 + 151, mode);

    _names->fillCanvas(0);
    if (language == LANGUAGE_JA) {
        _names->drawString("音楽", 20 + 46, 16);
    } else if (language == LANGUAGE_ZH) {
        _names->drawString("音乐播放器", 20 + 46, 16);
    } else {
        _names->drawString("Music", 20 + 46, 16);
    }
    _names->pushCanvas(0, 186 + 151 * 2, mode);
    // _player->pushCanvas(0, 0, mode);
}

void Frame_Main::StatusBar(m5epd_update_mode_t mode) {
    if ((millis() - _time) < _next_update_time) {
        return;
    }
    char buf[20];
    _bar->fillCanvas(0);
    _bar->drawFastHLine(0, 43, 540, 15);
    _bar->setTextDatum(CL_DATUM);
    _bar->drawString("M5Paper", 10, 27);

    // Battery
    _bar->setTextDatum(CR_DATUM);
    _bar->pushImage(498, 8, 32, 32, ImageResource_status_bar_battery_32x32);
    uint32_t vol = M5.getBatteryVoltage();

    if (vol < 3300) {
        vol = 3300;
    } else if (vol > 4350) {
        vol = 4350;
    }
    float battery = (float)(vol - 3300) / (float)(4350 - 3300);
    if (battery <= 0.01) {
        battery = 0.01;
    }
    if (battery > 1) {
        battery = 1;
    }
    uint8_t px = battery * 25;
    sprintf(buf, "%d%%", (int)(battery * 100));
    // _bar->drawString(buf, 498 - 10, 27);
    _bar->fillRect(498 + 3, 8 + 10, px, 13, 15);
    // _bar->pushImage(498, 8, 32, 32, 2,
    // ImageResource_status_bar_battery_charging_32x32);

    // Time
    rtc_time_t time_struct;
    rtc_date_t date_struct;
    M5.RTC.getTime(&time_struct);
    M5.RTC.getDate(&date_struct);
    sprintf(buf, "%2d:%02d", time_struct.hour, time_struct.min);
    _bar->setTextDatum(CC_DATUM);
    _bar->drawString(buf, 270, 27);
    _bar->pushCanvas(0, 0, mode);

    _time             = millis();
    _next_update_time = (60 - time_struct.sec) * 1000;
}

int Frame_Main::init(epdgui_args_vector_t &args) {
    _is_run = 1;
    M5.EPD.WriteFullGram4bpp(GetWallpaper());
    for (int i = 0; i < kAppsCountAsLastItem; i++) {
        EPDGUI_AddObject(_key[i]);
    }
    _time             = 0;
    _next_update_time = 0;
    StatusBar(UPDATE_MODE_NONE);
    AppName(UPDATE_MODE_NONE);
    return 9;
}

int Frame_Main::run() {
    Frame_Base::run();
    StatusBar(UPDATE_MODE_GL16);
    return 1;
}
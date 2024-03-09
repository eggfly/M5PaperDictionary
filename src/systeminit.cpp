#include "systeminit.h"
#include "epdgui/epdgui.h"
#include "frame/frame.h"
#include "Free_Fonts.h"
#include "global_setting.h"
#include "resources/binaryttf.h"
#include "resources/font_hei_ttf.header"
#include "resources/font_zhunyuan_ttf.header"

#include <SPIFFS.h>
#include <WiFi.h>

M5EPD_Canvas _initcanvas(&M5.EPD);

QueueHandle_t xQueue_Info = xQueueCreate(20, sizeof(uint32_t));

void WaitForUser(void) {
    SysInit_UpdateInfo("$ERR");
    while (1) {
        M5.update();
        if (M5.BtnP.wasReleased()) {
            SysInit_UpdateInfo("$RESUME");
            return;
        }
    }
}

void Screen_Test(void) {
    Serial.println("Start Screen Life Test...");
    _initcanvas.createCanvas(540, 960);
    _initcanvas.setTextSize(4);
    delay(100);
    float min = 0;
    while (1) {
        for (uint8_t pos = 0; pos < 2; pos++) {
            for (uint8_t index = 0; index < 16; index++) {
                _initcanvas.fillRect(0, index * 60, 540, 60, index);
            }
            int possition = random(960);
            _initcanvas.drawString("Test Time: " + String(min) + "min", 20,
                                   possition);
            _initcanvas.pushCanvas(0, 0, UPDATE_MODE_GC16);
            delay(10000);
            for (uint8_t index = 0; index < 16; index++) {
                _initcanvas.fillRect(0, index * 60, 540, 60, (15 - index));
            }
            _initcanvas.drawString("Test Time: " + String(min) + "min", 20,
                                   possition);
            _initcanvas.pushCanvas(0, 0, UPDATE_MODE_GC16);
            delay(10000);
        }
        min += 1;
    }
}

void SysInit_Start(void) {
    bool ret = false;
    Serial.begin(115200);
    Serial.flush();
    delay(50);
    Serial.print("M5EPD initializing...");

    // if (!SPIFFS.begin(false)) {
    //     log_e("SPIFFS Mount Failed");
    //     while (1);
    // }
    pinMode(M5EPD_EXT_PWR_EN_PIN, OUTPUT);
    pinMode(M5EPD_EPD_PWR_EN_PIN, OUTPUT);
    pinMode(M5EPD_KEY_RIGHT_PIN, INPUT);
    pinMode(M5EPD_KEY_PUSH_PIN, INPUT);
    pinMode(M5EPD_KEY_LEFT_PIN, INPUT);
    M5.enableEPDPower();
    delay(100);

    M5.enableEXTPower();

    M5.EPD.begin(M5EPD_SCK_PIN, M5EPD_MOSI_PIN, M5EPD_MISO_PIN, M5EPD_CS_PIN,
                 M5EPD_BUSY_PIN);
    M5.EPD.Clear(true);
    M5.EPD.SetRotation(M5EPD_Driver::ROTATE_90);
    M5.TP.SetRotation(GT911::ROTATE_90);
    // splash screen
    const uint16_t kPosy = 548;
    const uint8_t *kLD[] = {
        ImageResource_loading_01_96x96, ImageResource_loading_02_96x96,
        ImageResource_loading_03_96x96, ImageResource_loading_04_96x96,
        ImageResource_loading_05_96x96, ImageResource_loading_06_96x96,
        ImageResource_loading_07_96x96, ImageResource_loading_08_96x96,
        ImageResource_loading_09_96x96, ImageResource_loading_10_96x96,
        ImageResource_loading_11_96x96, ImageResource_loading_12_96x96,
        ImageResource_loading_13_96x96, ImageResource_loading_14_96x96,
        ImageResource_loading_15_96x96, ImageResource_loading_16_96x96};

    M5EPD_Canvas LoadingIMG(&M5.EPD);
    M5EPD_Canvas Info(&M5.EPD);
    LoadingIMG.createCanvas(96, 96);
    Info.createCanvas(540, 50);
    Info.setFreeFont(FF18);
    Info.setTextSize(1);
    Info.setTextDatum(CC_DATUM);
    Info.setTextColor(15);

    M5.EPD.WritePartGram4bpp(92, 182, 356, 300, ImageResource_logo_356x300);
    M5.EPD.UpdateFull(UPDATE_MODE_GC16);

    int i = 0;
    char *p;
    uint32_t time = 0;
    // eof 

    if (!digitalRead(39)) {
        delay(10);
        if (!digitalRead(39)) {
            Screen_Test();
        }
    }
    bool is_factory_test;
    SPI.begin(14, 13, 12, 4);
    ret = SD.begin(4, SPI, 20000000);
    if (ret == false) {
        is_factory_test = true;
        SetInitStatus(0, 0);
        // log_e("Failed to initialize SD card.");
        // SysInit_UpdateInfo("[ERROR] Failed to initialize SD card.");
        // WaitForUser();
    } else {
        is_factory_test = SD.exists("/__factory_test_flag__");
    }

    if ((!is_factory_test)) {
        SetTTFLoaded(true);
        // if (SPIFFS.exists("/font.ttf")) {
        //     log_d("loadFont from SPIFFS.");
        //     _initcanvas.loadFont("/font.ttf", SPIFFS);
        // }
        if (SD.exists("/wqy.ttf")) {
            log_d("loadFont from SD card.");
            _initcanvas.loadFont("/wqy.ttf", SD);
        } else {
            log_d("don't loadFont from PROGMEM.");
            SetTTFLoaded(false);
            // drom0_0_seg max 0x330000?
            // https://transfonter.org/
            // _initcanvas.loadFont(font_zhunyuan_ttf, sizeof(font_zhunyuan_ttf));
            // _initcanvas.loadFont(font_hei_ttf, sizeof(font_hei_ttf));
        }
    } else {
        log_d("loadFont binaryttf from PROGMEM.");
        _initcanvas.loadFont(binaryttf, sizeof(binaryttf));
        SetTTFLoaded(false);
        SetLanguage(LANGUAGE_EN);
        is_factory_test = true;
    }
    log_d("before createRender()");
    _initcanvas.createRender(36);
    _initcanvas.createRender(26, 128);
    log_d("createRender() done");
    
    Info.drawString("Starting up...", 270, 20);

    disableCore0WDT();
    // xTaskCreatePinnedToCore(SysInit_Loading, "SysInit_Loading", 4096, NULL, 1,
    //                         NULL, 0);
    // SysInit_UpdateInfo("Initializing SD card...");

    delay(1*1000);
    log_e("Touch pad initialization start.");
    SysInit_UpdateInfo("Initializing Touch pad...");
    log_e("Touch pad initialization start 1.");
    if (M5.TP.begin(21, 22, 36) != ESP_OK) {
        SetInitStatus(1, 0);
        log_e("Touch pad initialization failed.");
        SysInit_UpdateInfo("[ERROR] Failed to initialize Touch pad.");
        WaitForUser();
    }
    log_e("Touch pad initialization done.");

    M5.BatteryADCBegin();
    LoadSetting();
    log_e("LoadSetting done.");
    if (is_factory_test) {
        SysInit_UpdateInfo("$OK");
    } else {
        SysInit_UpdateInfo("Initializing system...");
    }
    log_e("is_factory_test done.");

    log_e("before new Frame_Main().");
    Frame_Main *frame_main = new Frame_Main();
    log_e("after new Frame_Main().");
    EPDGUI_PushFrame(frame_main);
    // 1. FactoryTest
    log_e("before Frame_FactoryTest.");
    Frame_FactoryTest *frame_factorytest = new Frame_FactoryTest();
    EPDGUI_AddFrame("Frame_FactoryTest", frame_factorytest);
    if (!is_factory_test) {
        // 2. Settings
        Frame_Setting *frame_setting = new Frame_Setting();
        EPDGUI_AddFrame("Frame_Setting", frame_setting);

        // 2.1 Settings Wallpaper
        Frame_Setting_Wallpaper *frame_wallpaper =
            new Frame_Setting_Wallpaper();
        EPDGUI_AddFrame("Frame_Setting_Wallpaper", frame_wallpaper);

        // 2.2 Settings Language
        Frame_Setting_Language *frame_language = new Frame_Setting_Language();
        EPDGUI_AddFrame("Frame_Setting_Language", frame_language);

        // 3. Keyboard
        Frame_Keyboard *frame_keyboard = new Frame_Keyboard(0);
        EPDGUI_AddFrame("Frame_Keyboard", frame_keyboard);

        // 4. WIFI
        Frame_WifiScan *frame_wifiscan = new Frame_WifiScan();
        EPDGUI_AddFrame("Frame_WifiScan", frame_wifiscan);

        // 4.1 WIFI Password
        Frame_WifiPassword *frame_wifipassword = new Frame_WifiPassword();
        EPDGUI_AddFrame("Frame_WifiPassword", frame_wifipassword);
        // 6. Compare
        Frame_Compare *frame_compare = new Frame_Compare();
        EPDGUI_AddFrame("Frame_Compare", frame_compare);
        // 7. Home
        log_e("before Frame_Home.");
        Frame_Home *frame_home = new Frame_Home();
        EPDGUI_AddFrame("Frame_Home", frame_home);
        log_e("after Frame_Home.");
        // 8. Lifegame
        Frame_Lifegame *frame_lifegame = new Frame_Lifegame();
        EPDGUI_AddFrame("Frame_Lifegame", frame_lifegame);

        log_e("isWiFiConfiged()?");
        if (isWiFiConfiged()) {
            SysInit_UpdateInfo("Connect to " + GetWifiSSID() + "...");
            Serial.println("eggfly WIFI");
            WiFi.begin(GetWifiSSID().c_str(), GetWifiPassword().c_str());
            uint32_t t = millis();
            while (1) {
                if (millis() - t > 8000) {
                    break;
                }

                if (WiFi.status() == WL_CONNECTED) {
                    frame_wifiscan->SetConnected(GetWifiSSID(), WiFi.RSSI());
                    break;
                }
            }
        }
    }

    log_d("done");

    // while (uxQueueMessagesWaiting(xQueue_Info));
    log_d("wait xQueue_Info done!");

    if (!is_factory_test) {
        SysInit_UpdateInfo("$OK");
    }

    Serial.println("OK");

    delay(500);
}

void SysInit_Loading(void *pvParameters) {
    const uint16_t kPosy = 548;
    const uint8_t *kLD[] = {
        ImageResource_loading_01_96x96, ImageResource_loading_02_96x96,
        ImageResource_loading_03_96x96, ImageResource_loading_04_96x96,
        ImageResource_loading_05_96x96, ImageResource_loading_06_96x96,
        ImageResource_loading_07_96x96, ImageResource_loading_08_96x96,
        ImageResource_loading_09_96x96, ImageResource_loading_10_96x96,
        ImageResource_loading_11_96x96, ImageResource_loading_12_96x96,
        ImageResource_loading_13_96x96, ImageResource_loading_14_96x96,
        ImageResource_loading_15_96x96, ImageResource_loading_16_96x96};

    M5EPD_Canvas LoadingIMG(&M5.EPD);
    M5EPD_Canvas Info(&M5.EPD);
    LoadingIMG.createCanvas(96, 96);
    Info.createCanvas(540, 50);
    Info.setFreeFont(FF18);
    Info.setTextSize(1);
    Info.setTextDatum(CC_DATUM);
    Info.setTextColor(15);

    M5.EPD.WritePartGram4bpp(92, 182, 356, 300, ImageResource_logo_356x300);
    M5.EPD.UpdateFull(UPDATE_MODE_GC16);
    int i = 0;
    char *p;
    uint32_t time = 0;
    while (1) {
        if (millis() - time > 250) {
            time = millis();
            LoadingIMG.pushImage(0, 0, 96, 96, kLD[i]);
            LoadingIMG.pushCanvas(220, kPosy + 80, UPDATE_MODE_DU4);
            i++;
            if (i == 16) {
                i = 0;
            }
        }

        if (xQueueReceive(xQueue_Info, &p, 0)) {
            String str(p);
            free(p);
            if (str.indexOf("$OK") >= 0) {
                LoadingIMG.pushImage(0, 0, 96, 96,
                                     ImageResource_loading_success_96x96);
                LoadingIMG.pushCanvas(220, kPosy + 80, UPDATE_MODE_GL16);
                break;
            } else if (str.indexOf("$ERR") >= 0) {
                LoadingIMG.pushImage(0, 0, 96, 96,
                                     ImageResource_loading_error_96x96);
                LoadingIMG.pushCanvas(220, kPosy + 80, UPDATE_MODE_GL16);
                LoadingIMG.fillCanvas(0);
                while (1) {
                    if (xQueueReceive(xQueue_Info, &p, 0)) {
                        String str(p);
                        free(p);
                        if (str.indexOf("$RESUME") >= 0) {
                            LoadingIMG.pushCanvas(220, kPosy + 80,
                                                  UPDATE_MODE_GC16);
                            break;
                        }
                    }
                }
            } else {
                Info.fillCanvas(0);
                Info.drawString(str, 270, 20);
                Info.pushCanvas(0, kPosy, UPDATE_MODE_DU);
            }
        }
    }
    log_d("before return");
    vTaskDelete(NULL);
    return;
}

void SysInit_UpdateInfo(String info) {
    char *p = (char *)malloc(info.length() + 1);
    memcpy(p, info.c_str(), info.length());
    p[info.length()] = '\0';
    if (xQueueSend(xQueue_Info, &p, 0) == 0) {
        free(p);
    }
}
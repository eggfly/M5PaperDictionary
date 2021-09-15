#include <M5EPD.h>
#include "epdgui/epdgui.h"
#include "frame/frame.h"
#include "systeminit.h"

void setup()
{
    int core_id = xPortGetCoreID();
    log_d("setup() runs on core_id = %d", core_id);
    pinMode(M5EPD_MAIN_PWR_PIN, OUTPUT);
    M5.enableMainPower();

    SysInit_Start();
}


void loop()
{
    int core_id = xPortGetCoreID();
    log_d("loop() runs on core_id = %d", core_id);
    EPDGUI_MainLoop();
}

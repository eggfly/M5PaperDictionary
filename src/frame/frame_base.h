#ifndef _FRAME_BASE_H_
#define _FRAME_BASE_H_

#include <M5EPD.h>
#include "../epdgui/epdgui_button.h"
#include "../global_setting.h"
#include "../resources/ImageResource.h"
#include "../resources/MyImageResource.h"
#include "../resources/ImageResource_Vinyl.h"

class Frame_Base {
   public:
    Frame_Base(bool _has_title = true);
    void exitbtn(String title, uint16_t width = 150);
    virtual ~Frame_Base();
    virtual int run();
    virtual void exit();
    virtual int init(epdgui_args_vector_t &args) = 0;
    String GetFrameName() {
        return _frame_name;
    }
    int isRun() {
        return _is_run;
    }
    bool hasWakeLock() {
        return _wake_lock_enabled;
    }
    void setWakeLock(bool enableLock) {
        _wake_lock_enabled = enableLock;
    }
    void SetFrameID(uint32_t id) {
        _frame_id = id;
    }
    uint32_t GetFrameID() {
        return _frame_id;
    }

   protected:
    static void exit_cb(epdgui_args_vector_t &args);
    void UpdateLastActiveTime();
    String _frame_name;
    bool _wake_lock_enabled      = false;
    int _is_run                  = 1;
    M5EPD_Canvas *_canvas_title  = NULL;
    M5EPD_Canvas *_canvas_footer = NULL;
    EPDGUI_Button *_key_exit     = NULL;
    uint32_t _frame_id;

   private:
    bool _shutdown_prompt_is_shown = false;
    void CheckAutoPowerSave();
};

#endif
#pragma once

#include "Audio.h"

#include <memory>
#include <vector>

namespace singsang
{
class CPlayer
{
  public:
    CPlayer();

    ~CPlayer() = default;

    void begin();

    void loop();

    void startNextSong();

    void updateVolume(int f_deltaVolume);

    void increaseVolume();

    void decreaseVolume();

  private:
    void handleInactivityTimeout();
    void handleTouchEvents();
    void initializeGui();
    void initializeHardware();
    void populateMusicFileList();
    void populateMusicFileListByDepth(const char * path, size_t depth);
    void updateGui();
    void vibrate();
    void autoPlayNextSong();

    Audio m_audio{};

    bool                m_isPausing{false};
    int                 m_currentVolume{0};
    int                 m_activeSongIdx{ -1};
    unsigned int        m_turnOffAfterInactiveForMilliSec{5 * 60 * 1000};
    unsigned int        m_lastActivityTimestamp{0};
    std::vector<String> m_songFiles{};

    //    CBatteryWidget       m_batteryWidget;
    //    CFileSelectionWidget m_fileSelectionWidget;
    //    CNextSongWidget      m_nextSongWidget;
    //    CProgressWidget      m_progressWidget;
    //    CVolumeDisplayWidget m_volumeDisplayWidget;
    //    CVolumeDownWidget    m_volumeDownWidget;
    //    CVolumeUpWidget      m_volumeUpWidget;
};

} // namespace singsang

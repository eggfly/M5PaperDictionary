#include <Arduino.h>
#include <memory>
#include <vector>
#include <unordered_set>
#include <SD.h>
#include <SPI.h>
#include <FS.h>

#include "player.hpp"
#include "config.h"


namespace singsang
{

CPlayer::CPlayer() {}

void CPlayer::begin()
{
  initializeHardware();
  initializeGui();
}

void CPlayer::loop()
{
  m_audio.loop();

  //  handleTouchEvents();
  //
  //  updateGui();
  //
  autoPlayNextSong();

  //  handleInactivityTimeout();
}

void CPlayer::autoPlayNextSong() {
  if (!m_isPausing && !m_audio.isRunning()) {
    Serial.println("autoPlay: playNextSong()");
    startNextSong();
  }
}
void CPlayer::initializeHardware()
{
  bool alreadyInitialized = true;
  if (!alreadyInitialized) {
    // WiFi.mode(WIFI_OFF);
    // delay(100);
    // EXT 5V Power
    pinMode(5, OUTPUT);
    digitalWrite(5, HIGH);
    //  pinMode(SD_CS, OUTPUT);
    //  digitalWrite(SD_CS, HIGH);
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
    // 20000000, 1000000
    SD.begin(SD_CS); // SD.begin(4, SPI, 20000000);
    // SPI.setFrequency(20000);
  }
  m_audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  m_audio.setVolume(m_currentVolume); // 0...21
  // populateMusicFileList();
  populateMusicFileListByDepth("/music", 1); // Don't use /music/
  // vibrate();
}

const char * CPlayer::getCurrentSongPath() {
  if (m_activeSongIdx < 0 || m_activeSongIdx >= m_songFiles.size()) {
    return nullptr;
  }
  return m_songFiles[m_activeSongIdx].c_str();
}

const char * CPlayer::startNextSong()
{
  if (m_songFiles.size() == 0)
  {
    return nullptr;
  }
  size_t pos = random(m_songFiles.size());
  m_activeSongIdx = pos;
  if (m_activeSongIdx >= m_songFiles.size() || m_activeSongIdx < 0)
  {
    m_activeSongIdx = 0;
  }

  if (m_audio.isRunning())
  {
    m_audio.stopSong();
  }
  const char * song_path = m_songFiles[m_activeSongIdx].c_str();
  Serial.printf("Play: %s\n", song_path);
  m_audio.connecttoSD(song_path);
  return song_path;
}

void CPlayer::initializeGui()
{
  //  M5.Lcd.fillScreen(TFT_BLACK);
  //  M5.Lcd.setTextFont(2);
  //
  //  M5.Lcd.drawJpgFile(SD, "/media/logo.jpg", 60, 20, 200, 200);
  //
  //  m_batteryWidget.draw(false);
  //  m_fileSelectionWidget.draw(false);
  //  m_nextSongWidget.draw(false);
  //  m_progressWidget.draw(false);
  //  m_volumeDisplayWidget.draw(false);
  //  m_volumeDownWidget.draw(false);
  //  m_volumeUpWidget.draw(false);
}

void CPlayer::updateVolume(int f_deltaVolume)
{
  constexpr int minVolume = 1;
  constexpr int maxVolume = 21;

  int newVolume = m_currentVolume + f_deltaVolume;

  if (newVolume < minVolume)
  {
    newVolume = minVolume;
  }
  if (newVolume > maxVolume)
  {
    newVolume = maxVolume;
  }
  Serial.printf("newVolume=%d\n", newVolume);

  m_currentVolume = newVolume;
  m_audio.setVolume(m_currentVolume);
}

void CPlayer::increaseVolume()
{
  updateVolume(+1);
}

void CPlayer::decreaseVolume() {
  updateVolume(-1);
}

void CPlayer::populateMusicFileList() {

  File musicDir = SD.open("/20221125");

  bool nextFileFound;
  do
  {
    nextFileFound = false;

    File entry = musicDir.openNextFile();

    if (entry)
    {
      nextFileFound = true;

      const bool entryIsFile = entry.size() > 4096;
      if (entryIsFile) {
        m_songFiles.push_back(entry.path());
        // Serial.printf("MUSIC: %s size=%d\n", entry.path(), entry.size());
      }

      entry.close();
    }
  } while (nextFileFound);

  Serial.print("MusicFileList length: ");
  Serial.println(m_songFiles.size());
}

int strncmpci(const char * str1, const char * str2, size_t num) {
  int ret_code = 0;
  size_t chars_compared = 0;

  if (!str1 || !str2) {
    ret_code = INT_MIN;
    return ret_code;
  }

  while ((chars_compared < num) && (*str1 || *str2))  {
    ret_code = tolower((int)(*str1)) - tolower((int)(*str2));
    if (ret_code != 0)
    {
      break;
    }
    chars_compared++;
    str1++;
    str2++;
  }

  return ret_code;
}

bool startsWithIgnoreCase(const char *pre, const char *str) {
  return strncmpci(pre, str, strlen(pre)) == 0;
}

bool endsWithIgnoreCase(const char* base, const char* str) {
  int blen = strlen(base);
  int slen = strlen(str);
  return (blen >= slen) && (0 == strncmpci(base + blen - slen, str, strlen(str)));
}


void CPlayer::populateMusicFileListByDepth(const char * path, size_t depth) {
 Serial.printf("search_dir: %s, depth=%d\n", path, depth);
 auto musicDir = SD.open(path);
 bool nextFileFound;
 do {
   nextFileFound = false;
   auto entry = musicDir.openNextFile();
   if (entry) {
    //  char fname[256];
    //  char currpath[256];
     // entry.getName(fname, 255);
    //  if (strcmp(path, "/") == 0) {
    //    snprintf(currpath, 256, "/%s", fname);
    //  } else {
    //    snprintf(currpath, 256, "%s/%s", path, fname);
    //  }
     nextFileFound = true;
     if (entry.isDirectory()) {
       if (depth > 0) {
         populateMusicFileListByDepth(entry.path(), depth - 1);
       }
     } else {
       const bool entryIsFile = entry.size() > 4096;
       if (entryIsFile) {
         if (endsWithIgnoreCase(entry.name(), ".mp3")
             || endsWithIgnoreCase(entry.name(), ".flac")
             // || endsWithIgnoreCase(entry.name(), ".m4a")
             || endsWithIgnoreCase(entry.name(), ".aac")
             || endsWithIgnoreCase(entry.name(), ".wav")
            ) {
           Serial.printf("MUSIC: %s size=%d\n", entry.path(), entry.size());
           m_songFiles.push_back(entry.path());
         } else {
           Serial.printf("OTHERS: %s size=%d\n", entry.path(), entry.size());
         }
       }
     }
     entry.close();
   }
 } while (nextFileFound);
}

bool listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return false;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return false;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        char path[128];
        snprintf(path, 128, "/%s/%s", dirname, file.name());
        Serial.println(path);
        listDir(fs, path, levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
  Serial.println("listDir end");
  return true;
}
}

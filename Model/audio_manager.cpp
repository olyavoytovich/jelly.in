#include "audio_manager.h"

AudioManager::AudioManager() {
  audios_.resize(static_cast<int>(AudioName::kAnyAudio));
  int key = static_cast<int>(AudioName::kButtonClick);
  audios_[key] = std::make_shared<QMediaPlayer>();
  audios_[key]->setAudioRole(QAudio::SonificationRole);
  audios_[key]->setMedia(QUrl("qrc:/audio/menu/button_click.mp3"));
  audios_[key]->setVolume(100);

  key = static_cast<int>(AudioName::kMenuAudio);
  audios_[key] = std::make_shared<QMediaPlayer>();
  QMediaPlaylist* playlist1 = new QMediaPlaylist;
  playlist1->addMedia(QUrl("qrc:/audio/music/menu.mp3"));
  audios_[key]->setAudioRole(QAudio::MusicRole);
  audios_[key]->setPlaylist(playlist1);
  audios_[key]->setVolume(100);

  key = static_cast<int>(AudioName::kBackground);
  audios_[key] = std::make_shared<QMediaPlayer>();
  QMediaPlaylist* playlist2 = new QMediaPlaylist;
  playlist2->addMedia(QUrl("qrc:/audio/music/background.mp3"));
  audios_[key]->setAudioRole(QAudio::MusicRole);
  audios_[key]->setPlaylist(playlist2);
  audios_[key]->setVolume(100);
}

void AudioManager::Play(AudioName name) {
  audios_[static_cast<int>(name)]->play();
}

void AudioManager::Replay(AudioName name) {
  int key = static_cast<int>(name);
  audios_[key]->setPosition(0);
  audios_[key]->play();
}

void AudioManager::Stop(AudioName name) {
  audios_[static_cast<int>(name)]->stop();
}

void AudioManager::Pause(AudioName name) {
  audios_[static_cast<int>(name)]->pause();
}

void AudioManager::SetVolume(AudioName name, int volume) {
  audios_[static_cast<int>(name)]->setVolume(volume);
}

void AudioManager::SetPlayBackMode(AudioName name,
                                   QMediaPlaylist::PlaybackMode mode) {
  audios_[static_cast<int>(name)]->playlist()->setPlaybackMode(mode);
}

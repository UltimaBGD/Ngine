/**********************************************************************************************
*
*   Ngine - A (mainly) 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#include "Sound.h"

namespace NerdThings::Ngine::Audio {
    // Destructor

    TSound::~TSound() {
        if (AudioBuffer != nullptr) {
            UnloadSound(ToRaylibSound());
            AudioBuffer = nullptr;
            Buffer = 0;
            Format = 0;
            Source = 0;
        }
    }

    // Public Methods

    #ifdef INCLUDE_RAYLIB

    Sound TSound::ToRaylibSound() const {
        Sound snd;
        snd.audioBuffer = AudioBuffer;
        snd.buffer = Buffer;
        snd.format = Format;
        snd.source = Source;
        return snd;
    }

    TSound TSound::FromRaylibSound(const Sound snd_) {
        TSound snd;
        snd.AudioBuffer = snd_.audioBuffer;
        snd.Source = snd_.source;
        snd.Buffer = snd_.buffer;
        snd.Format = snd_.format;
        return snd;
    }

    #endif

    TSound *TSound::LoadSound(const std::string &filename_) {
        auto snd = FromRaylibSound(::LoadSound(filename_.c_str()));
        auto ret = new TSound();
        ret->AudioBuffer = snd.AudioBuffer;
        ret->Buffer = snd.Buffer;
        ret->Format = snd.Format;
        ret->Source = snd.Source;
        snd.AudioBuffer = nullptr;
        snd.Buffer = 0;
        snd.Format = 0;
        snd.Source = 0;
        return ret;
    }

}

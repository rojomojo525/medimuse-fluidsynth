//
// Created by Peter Slack on 2020-10-27.
//

#ifndef FLUIDSYNTH_AUDIOENGINE_H
#define FLUIDSYNTH_AUDIOENGINE_H
#include <oboe/Oboe.h>
#include "FluidSynthesizer.h"

using namespace oboe;

class AudioEngine : public AudioStreamCallback{
public:
    void start();

    DataCallbackResult
    onAudioReady(AudioStream *oboeStream, void *audioData, int32_t numFrames) override;

};


#endif //FLUIDSYNTH_AUDIOENGINE_H

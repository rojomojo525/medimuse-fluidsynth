//
// Created by Peter Slack on 2020-10-27.
//

#include "AudioEngine.h"
#include "../../../../external_libraries/oboe/src/common/OboeDebug.h"

using namespace oboe;

void AudioEngine::start() {

    AudioStreamBuilder builder;

    builder.setCallback(this);
    builder.setPerformanceMode(PerformanceMode::LowLatency);
    builder.setSharingMode(SharingMode::Exclusive);
    builder.setChannelCount(2);

    AudioStream *stream = nullptr;

    Result result = builder.openStream(&stream);

    if (result != Result::OK) {
        // handle error
        LOGE("Error opening stream: %s", convertToText(result));
    }

    auto setBufferSizeResult = stream->setBufferSizeInFrames(stream->getFramesPerBurst() *2);
    if (setBufferSizeResult) {
        LOGE("New buffer size is %d frames",setBufferSizeResult.value());
    }

    result = stream->requestStart();

    if (result != Result::OK) {
        // handle error
        LOGE("Error starting stream: %s", convertToText(result));
    }


}

DataCallbackResult
AudioEngine::onAudioReady(AudioStream *oboeStream, void *audioData, int32_t numFrames) {
    return DataCallbackResult::Continue;
}

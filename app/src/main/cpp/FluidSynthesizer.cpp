//
// Created by Peter Slack on 2020-10-27.
//

#include <logging_macros.h>
#include "FluidSynthesizer.h"
#include "MidiMessage.h"
#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <regex>

FluidSynthesizer *FluidSynthesizer::handle = nullptr;

FluidSynthesizer::FluidSynthesizer(int32_t sampleRate, int32_t channelCount) : TappableAudioSource(
        sampleRate, channelCount) {


    settings = new_fluid_settings();
    fluid_settings_setint(settings,"synth.polyphony", 128);
    fluid_settings_setint(settings,"synth.sample-rate",sampleRate);
    fluid_settings_setint(settings,"synth.threadsafe-api",1);
    fluid_settings_setint(settings,"synth.verbose",1);



    synth = new_fluid_synth(settings);



//    fluid_midi_router_rule_t* rule;
//    router = new_fluid_midi_router (settings, fluid_synth_handle_midi_event, synth);
//    fluid_midi_router_clear_rules (router);
//    handler =  new_fluid_cmd_handler(synth, router);


//    shouldExit.store(false);
//    synthConsoleOut = std::thread(&FluidSynthesizer::thread_func,this);
//    synthConsoleOut.detach();

//    // let's do our own sf loader
//    sf_sfloader = new_fluid_defsfloader(settings);
//    fluid_sfloader_set_callbacks(sf_sfloader,
//                                 sf_open,
//                                 sf_read,
//                                 sf_seek,
//                                 sf_tell,
//                                 sf_close);
//    fluid_synth_add_sfloader(synth, sf_sfloader);

}

FluidSynthesizer::~FluidSynthesizer() {

      mIsSynthOn.store(false);

//    delete_fluid_cmd_handler(handler);
//    delete_fluid_midi_router(router);
    std::lock_guard<std::mutex> lock(mLock);
    fluid_synth_system_reset(synth);

    delete_fluid_synth(synth);
    delete_fluid_settings(settings);
}

void FluidSynthesizer::renderAudio(float *audioData, int32_t numFrames) {

//    // don't process if we are muted

    float *buffs[2] = {mBufferLeft.get(), mBufferRight.get()};
    // only process stereo or mono and frames less than buffer size
    if (numFrames <= kSharedBufferSize && mChannelCount == 2) {

        std::fill_n(mBufferLeft.get(), kSharedBufferSize, 0);
        std::fill_n(mBufferRight.get(), kSharedBufferSize, 0);

        // TODO figure out how to do effects processing and mixing
        // TODO Fluid Synth error processing
        if (mIsSynthOn.load()) {
            std::lock_guard<std::mutex> lock(mLock);
            fluid_synth_process(synth, numFrames, 0, nullptr, mChannelCount, buffs);
        }
            for (int j = 0; j < numFrames; ++j) {
                // interleave the data
                audioData[ j * 2 ]     = mBufferLeft[j];
                audioData[ j * 2 + 1 ] = mBufferRight[j];

            }


    }

}

void FluidSynthesizer::tap(bool isDown) {
    mIsSynthOn.store(isDown);
}

void FluidSynthesizer::resetSynth(){
        fluid_synth_system_reset(synth);
}

FILE * FluidSynthesizer::sf_handle;

void *FluidSynthesizer::sf_open(const char *filename)
{
    void *p;
    sf_handle = std::fopen(filename,"rb");

//    // let's test the file
//    tfile.open(filename , std::ios::binary | std::ios::ate);
//
//    if(tfile.is_open()) {
//        // let's do some tests on the file
//        tfile.seekg(0, std::ios::end);
//        long size = tfile.tellg();
//        tfile.seekg(0, std::ios::beg);
//        LOGD("Font File %s opebned is %i bytes" , filename, size);
//
//    } else {
//
//        LOGD("Could not open Font File %s" , filename);
//
//    }

    return sf_handle;
}
int FluidSynthesizer::sf_read(void *buf, int count, void *handle)
{
    int result = std::fread(buf,1,count,(FILE *)handle);
    if (result != count) { return FLUID_ERR; }
    return FLUID_OK;
}
int FluidSynthesizer::sf_seek(void *handle, long offset, int origin)
{
    int result = std::fseek((FILE *)handle,offset,origin);
    if (result != 0) {
        return FLUID_ERR;
    }
    return FLUID_OK;
}
int FluidSynthesizer::sf_close(void *handle)
{
    int result = std::fclose((FILE *)handle);
    if (result != 0) {
        return FLUID_ERR;
    }
    LOGD("Sound Font File Closed");
    return FLUID_OK;
}
long FluidSynthesizer::sf_tell(void *handle)
{

    int result = std::ftell((FILE *) handle);

    return result;
}


void FluidSynthesizer::onSend(uint8_t *msg,int offset,int count, long timeStamp) {
    // don't bother if we are off
    if(!mIsSynthOn.load()) { return; }

    MidiMessage m(msg,offset,count,timeStamp);
    if (m.isChannelCommand()) {
        MidiMessage::CHANNEL_COMMANDS cmd = m.getChannelCommand();

        switch (cmd) {
            case MidiMessage::CHANNEL_COMMANDS::PROGRAM:
                fluid_synth_program_change(synth,m.getChannel(),m.getProgram());
                //LOGD("pgmchange : %i %i" ,m.getChannel(),m.getProgram());
                break;
            case MidiMessage::CHANNEL_COMMANDS::NOTE_ON:
                fluid_synth_noteon(synth,m.getChannel(),m.getNote(),m.getVelocity());
                //LOGD("noteon :%i %i %i" ,m.getChannel(),m.getNote(),m.getVelocity());
                break;
            case MidiMessage::CHANNEL_COMMANDS::NOTE_OFF:
                fluid_synth_noteoff(synth,m.getChannel(),m.getNote());
                //LOGD("noteoff :%i %i" ,m.getChannel(),m.getNote());
                break;
        }
    }
}

int FluidSynthesizer::loadSoundFont(const char *fontFile) {

        if ( fluid_is_soundfont(fontFile) == FLUID_FAILED ) {
            return FLUID_FAILED;
        }
        int rval = FLUID_FAILED;
        if (synth != nullptr) {
            rval = fluid_synth_sfload(synth, fontFile, 1);
            if( rval != FLUID_FAILED) {
                LOGD(" Loaded Sound Font %s" , fontFile);
            } else {
                LOGD(" ERROR LOADING Sound Font %s" , fontFile);
            }


        } else {
            return FLUID_FAILED;
        }

        return rval;
}


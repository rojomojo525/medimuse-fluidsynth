//
// Created by Peter Slack on 2020-10-27.
//

#ifndef FLUIDSYNTH_FLUIDSYNTHESIZER_H
#define FLUIDSYNTH_FLUIDSYNTHESIZER_H

#include "fluidsynth.h"
#include <TappableAudioSource.h>
#include <fstream>
#include <memory>
#include <unistd.h>
#include <thread>


class FluidSynthesizer : public TappableAudioSource{
public:
    static constexpr size_t kSharedBufferSize = 2048;

    ~FluidSynthesizer();

    void tap(bool isDown) override;

    void renderAudio(float *audioData, int32_t numFrames) override;
    /**
     *
     * @param fontFile full path file name
     * @return the id of the loaded font file or FLUID_ERR if error
     */
    int loadSoundFont(const char * fontFile);

    void onSend(uint8_t *msg,int offset,int count, long timeStamp);
/**
 *
 * @param name
 * @return
 */
    int getIntSetting(const char * name) {
        int rval;
        fluid_settings_getint(settings,name,&rval);
        return rval;
    }
    /**
     *
     * @param name
     * @return
     */
    double getNumSetting(const char *name) {
        double rval;
        fluid_settings_getnum(settings,name,&rval);
        return rval;
    }
    /**
     *
     * @param name
     * @param str
     * @param len
     */
    void getStrSetting(const char *name, char * str, int len) {
        fluid_settings_copystr(settings,name,str,len);
    }

    void resetSynth();

    static FluidSynthesizer *getInstance(){
        if (handle == nullptr) {
            handle =  new FluidSynthesizer(44100, 2);
        }
        return handle;
    }

    static FluidSynthesizer *getInstance( int32_t sampleRate, int32_t channelCount){
        if (handle == nullptr) {
            handle =  new FluidSynthesizer(sampleRate, channelCount);
        }
        return handle;
    }

private:

    static std::ifstream tfile;
    static void *sf_open(const char *filename);
    static int sf_read(void *buf, int count, void *handle);
    static int sf_seek(void *handle, long offset, int origin);
    static int sf_close(void *handle);
    static long sf_tell(void *handle);

    static FILE *sf_handle;

    fluid_sfloader_t *sf_sfloader;
    fluid_synth_t *synth;
    fluid_settings_t *settings;
    std::unique_ptr<float[]> mBufferLeft = std::make_unique<float[]>(kSharedBufferSize);
    std::unique_ptr<float[]> mBufferRight = std::make_unique<float[]>(kSharedBufferSize);

    static FluidSynthesizer *handle;
    FluidSynthesizer(int32_t sampleRate, int32_t channelCount);

    std::atomic<bool> mIsSynthOn { false };
    std::thread synthConsoleOut;

    fluid_midi_router_t* router;
    std::atomic_bool shouldExit;

//    static int pfd[2];
//    static pthread_t thr;
//    static const char *tag;
//
//    int start_logger(const char *app_name)
//    {
//        tag = app_name;
//
//        /* make stdout line-buffered and stderr unbuffered */
//        setvbuf(stdout, 0, _IOLBF, 0);
//        setvbuf(stderr, 0, _IONBF, 0);
//
//        /* create the pipe and redirect stdout and stderr */
//        pipe(pfd);
//        dup2(pfd[1], 1);
//        dup2(pfd[1], 2);
//
//        /* spawn the logging thread */
//        if(pthread_create(&thr, 0, thread_func, 0) == -1)
//            return -1;
//        pthread_detach(thr);
//        return 0;
//    }

    void thread_func()
    {

        fluid_usershell(settings,handler);

        //        int fout = fluid_get_stdout();
//
//        ssize_t rdsz;
//        char buf[128];
//        while((rdsz = read(fout, buf, sizeof buf - 1)) > 0 && !shouldExit.load()) {
//            if(buf[rdsz - 1] == '\n') --rdsz;
//            buf[rdsz] = 0;  /* add null-terminator */
//            __android_log_write(ANDROID_LOG_DEBUG, "FSYNTH", buf);
//        }

        return;
    }

fluid_cmd_handler_t *handler;

    std::mutex     mLock;


};


#endif //FLUIDSYNTH_FLUIDSYNTHESIZER_H

/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package net.medimuse.fluidsynth;

import android.media.midi.MidiDeviceService;
import android.media.midi.MidiDeviceStatus;
import android.media.midi.MidiReceiver;
import android.util.Log;

import net.medimuse.fluidsynth.medimuse.audioengine.PlaybackEngine;
import net.medimuse.fluidsynth.medimuse.common.midi.MidiFramer;
import net.medimuse.fluidsynth.medimuse.midiscope.LoggingReceiver;
import net.medimuse.fluidsynth.medimuse.midiscope.MidiPrinter;
import net.medimuse.fluidsynth.medimuse.midiscope.ScopeLogger;

import org.puredata.core.utils.IoUtils;

import java.io.File;
import java.io.IOException;
import java.util.List;

/**
 * Virtual MIDI Device that logs messages to a ScopeLogger.
 */

public class MidiScope extends MidiDeviceService {

    private static ScopeLogger mScopeLogger;
    private MidiReceiver mInputReceiver = new MyReceiver();
    private static MidiFramer mDeviceFramer;
    private static boolean enableLogging = false;

    @Override
    public MidiReceiver[] onGetInputPortReceivers() {
        return new MidiReceiver[] { mInputReceiver };
    }

    public static void setScopeLogger(ScopeLogger logger) {
        if (logger != null) {
            // Receiver that prints the messages.
            LoggingReceiver loggingReceiver = new LoggingReceiver(logger);
            mDeviceFramer = new MidiFramer(loggingReceiver);
        }
        mScopeLogger = logger;
    }

    private static class MyReceiver extends MidiReceiver {
        @Override
        public void onSend(byte[] data, int offset, int count,
                long timestamp) throws IOException {
            PlaybackEngine.onSend(data,offset,count,timestamp);
            if (mScopeLogger != null && enableLogging) {
                // Send raw data to be parsed into discrete messages.
                mDeviceFramer.send(data, offset, count, timestamp);
            }
        }
    }

    private PlaybackEngine mPlayer;

    @Override
    public void onCreate() {
        super.onCreate();
        mPlayer.setBufferSizeInBursts(512);
        mPlayer.setChannelCount(2);
        mPlayer.create(this);

        try {
            loadPdPatch();
        } catch (IOException e) {
            e.printStackTrace();
        }


    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        mPlayer.delete();

    }

    private File basedir;
    private File defaultdir;

    private void loadPdPatch() throws IOException {
        basedir = new File(this.getFilesDir() + File.separator + "synthesizers");
        defaultdir = new File(this.getFilesDir() + File.separator + "synthesizers" + File.separator + "default") ;

        boolean success = true;

        if (!basedir.exists()) {
            success = basedir.mkdirs();
            if (success) {
                success = defaultdir.mkdirs();
            }

        } else {
            if (!defaultdir.exists()) {
                success = defaultdir.mkdirs();
            }
        }


        if (success) {
            File pdPatch = new File(defaultdir, "defaultSoundFont.sf2");
            if(!pdPatch.exists()) {
                List<File> files = IoUtils.extractZipResource(this.getResources().openRawResource(R.raw.soundfont), defaultdir, true);
                Log.e("MIDISCOPE","File extracted " + pdPatch.getAbsolutePath());
            } else {
                Log.e("MIDISCOPE","File exists " + pdPatch.getAbsolutePath());

            }

            PlaybackEngine.loadSoundFont(pdPatch.getAbsolutePath());

        } else {
            throw (new IOException("cannot create patch directory"));
        }

    }

    /**
     * This will get called when clients connect or resetSynth.
     * Log device information.
     */
    @Override
    public void onDeviceStatusChanged(MidiDeviceStatus status) {
        if (mScopeLogger != null) {
            if (status.isInputPortOpen(0)) {
                mScopeLogger.log("=== connected ===");
                mPlayer.setToneOn(true);
                String text = MidiPrinter.formatDeviceInfo(
                        status.getDeviceInfo());
                mScopeLogger.log(text);
            } else {
                mPlayer.setToneOn(false);
                PlaybackEngine.resetSynth();
                mScopeLogger.log("--- disconnected ---");
            }
        }
    }
}

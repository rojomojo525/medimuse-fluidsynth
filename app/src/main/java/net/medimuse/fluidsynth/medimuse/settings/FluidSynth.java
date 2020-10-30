package net.medimuse.fluidsynth.medimuse.settings;

import android.content.Context;

import com.google.gson.annotations.Expose;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;

public class FluidSynth {

    @Expose
    private FluidSettings synth;

    @Expose
    private FluidSettings audio;

    @Expose
    private FluidSettings midi;

    @Expose
    private FluidSettings player;

    @Expose
    private FluidSettings shell;


    public static String readRawTextFile(Context ctx, int resId)
    {
        InputStream inputStream = ctx.getResources().openRawResource(resId);

        InputStreamReader inputreader = new InputStreamReader(inputStream);
        BufferedReader buffreader = new BufferedReader(inputreader);
        String line;
        StringBuilder text = new StringBuilder();

        try {
            while (( line = buffreader.readLine()) != null) {
                text.append(line);
            }
        } catch (IOException e) {
            return null;
        }
        return text.toString();
    }

    public FluidSettings getSynth() {
        return synth;
    }

    public void setSynth(FluidSettings synth) {
        this.synth = synth;
    }

    public FluidSettings getAudio() {
        return audio;
    }

    public void setAudio(FluidSettings audio) {
        this.audio = audio;
    }

    public FluidSettings getMidi() {
        return midi;
    }

    public void setMidi(FluidSettings midi) {
        this.midi = midi;
    }

    public FluidSettings getPlayer() {
        return player;
    }

    public void setPlayer(FluidSettings player) {
        this.player = player;
    }

    public FluidSettings getShell() {
        return shell;
    }

    public void setShell(FluidSettings shell) {
        this.shell = shell;
    }
}

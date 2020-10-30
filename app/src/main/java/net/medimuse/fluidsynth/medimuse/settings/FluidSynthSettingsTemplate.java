package net.medimuse.fluidsynth.medimuse.settings;

import com.google.gson.annotations.Expose;

public class FluidSynthSettingsTemplate {

    @Expose
    private FluidSynth fluidsettings;

    public FluidSynth getFluidsettings() {
        return fluidsettings;
    }

    public void setFluidsettings(FluidSynth fluidsettings) {
        this.fluidsettings = fluidsettings;
    }
}

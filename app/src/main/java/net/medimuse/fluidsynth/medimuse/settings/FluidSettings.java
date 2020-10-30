package net.medimuse.fluidsynth.medimuse.settings;

import com.google.gson.annotations.Expose;

import java.util.ArrayList;

public class FluidSettings {

    @Expose
    private ArrayList<FluidSynthSetting> setting;

    public ArrayList<FluidSynthSetting> getSetting() {
        return setting;
    }

    public void setSetting(ArrayList<FluidSynthSetting> setting) {
        this.setting = setting;
    }
}

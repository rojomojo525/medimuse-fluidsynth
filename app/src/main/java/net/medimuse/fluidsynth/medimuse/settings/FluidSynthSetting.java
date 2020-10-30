package net.medimuse.fluidsynth.medimuse.settings;

import com.google.gson.annotations.Expose;

public class FluidSynthSetting {


    @Expose
    private String name = "";

    @Expose
    private String type = "";

    @Expose
    private String desc = "";

    @Expose
    private String min = "";

    @Expose
    private String max = "";

    @Expose
    private String def = "";

    @Expose
    private String vals = "";

    public String getVals() {
        return vals;
    }

    public void setVals(String vals) {
        this.vals = vals;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public String getDesc() {
        return desc;
    }

    public void setDesc(String desc) {
        this.desc = desc;
    }

    public String getMin() {
        return min;
    }

    public void setMin(String min) {
        this.min = min;
    }

    public String getMax() {
        return max;
    }

    public void setMax(String max) {
        this.max = max;
    }

    public String getDef() {
        return def;
    }

    public void setDef(String def) {
        this.def = def;
    }
}

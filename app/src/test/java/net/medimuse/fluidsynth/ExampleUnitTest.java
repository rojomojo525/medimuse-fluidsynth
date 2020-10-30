package net.medimuse.fluidsynth;

import com.google.gson.FieldNamingPolicy;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

import net.medimuse.fluidsynth.medimuse.settings.FluidSynth;

import org.junit.Test;

import java.util.ArrayList;
import java.util.HashMap;

import static org.junit.Assert.*;

/**
 * Example local unit test, which will execute on the development machine (host).
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */
public class ExampleUnitTest {
    @Test
    public void addition_isCorrect() {
        assertEquals(4, 2 + 2);
    }

    @Test
    public void voidtestSettingsJson() {

//        FluidSynth storage;
//
//    Gson gson = new GsonBuilder()
//
//            .setFieldNamingPolicy(FieldNamingPolicy.UPPER_CAMEL_CASE)
//            .setPrettyPrinting()
//            .setVersion(1.0)
//            .create();
//
//    String json = gson.toJson(storage);
//
//        System.out.println(json);
//
//
//    VoiceConfigurationSet restored = gson.fromJson(json,VoiceConfigurationSet.class);
//
//
//    //ok do our tests
//
//    ArrayList<VoiceChannelConfiguration> configs = restored.getVoiceConfigurationSet();
//    assertEquals(configs.size(),1);
//
//    HashMap<Integer, VoiceChannelConfigurationState> configRestored = configs.get(0).getConfigurationStateMap();
//    assertEquals(configRestored.size(),5);
//
//    ArrayList<VoiceChannel> voicesRestored = configs.get(0).getChannels();
//    assertEquals(voicesRestored.size(),5);
//        for (int i=0;i<voicesRestored.size();i++) {
//        assertEquals(voicesRestored.get(i).getChannelID(), baseChannels.get(i).getChannelID());
//        assertEquals(voicesRestored.get(i).getChannelName(), baseChannels.get(i).getChannelName());
//        assertEquals(voicesRestored.get(i).getMidiID(), baseChannels.get(i).getMidiID());
//    }
}
}
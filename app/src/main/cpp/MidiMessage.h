//
// Created by Peter Slack on 2020-10-29.
//

#ifndef FLUIDSYNTH_MIDIMESSAGE_H
#define FLUIDSYNTH_MIDIMESSAGE_H


#include <cstdint>

class MidiMessage {
public:

    MidiMessage(uint8_t *msg,int offset,int count, long timeStamp);

   uint8_t STATUS_COMMAND_MASK = (uint8_t) 0xF0;
   uint8_t STATUS_CHANNEL_MASK = (uint8_t) 0x0F;

    // Channel voice messages.
   uint8_t STATUS_NOTE_OFF = (uint8_t) 0x80;
   uint8_t STATUS_NOTE_ON = (uint8_t) 0x90;
   uint8_t STATUS_POLYPHONIC_AFTERTOUCH = (uint8_t) 0xA0;
   uint8_t STATUS_CONTROL_CHANGE = (uint8_t) 0xB0;
   uint8_t STATUS_PROGRAM_CHANGE = (uint8_t) 0xC0;
   uint8_t STATUS_CHANNEL_PRESSURE = (uint8_t) 0xD0;
   uint8_t STATUS_PITCH_BEND = (uint8_t) 0xE0;

    // System Common Messages.
   uint8_t STATUS_SYSTEM_EXCLUSIVE = (uint8_t) 0xF0;
   uint8_t STATUS_MIDI_TIME_CODE = (uint8_t) 0xF1;
   uint8_t STATUS_SONG_POSITION = (uint8_t) 0xF2;
   uint8_t STATUS_SONG_SELECT = (uint8_t) 0xF3;
   uint8_t STATUS_TUNE_REQUEST = (uint8_t) 0xF6;
   uint8_t STATUS_END_SYSEX = (uint8_t) 0xF7;

    // System Real-Time Messages
   uint8_t STATUS_TIMING_CLOCK = (uint8_t) 0xF8;
   uint8_t STATUS_START = (uint8_t) 0xFA;
   uint8_t STATUS_CONTINUE = (uint8_t) 0xFB;
   uint8_t STATUS_STOP = (uint8_t) 0xFC;
   uint8_t STATUS_ACTIVE_SENSING = (uint8_t) 0xFE;
   uint8_t STATUS_RESET = (uint8_t) 0xFF;

    /** Number of bytes in a message nc from 8c to Ec */
  int CHANNEL_BYTE_LENGTHS[7] = { 3, 3, 3, 3, 2, 2, 3 };

    /** Number of bytes in a message Fn from F0 to FF */
  int SYSTEM_BYTE_LENGTHS[16] = { 1, 2, 3, 2, 1, 1, 1, 1, 1,
                                                  1, 1, 1, 1, 1, 1, 1 };

    /**
     * MIDI messages, except for SysEx, are 1,2 or 3 bytes long.
     * You can tell how long a MIDI message is from the first status byte.
     * Do not call this for SysEx, which has variable length.
     * @param statusByte
     * @return number of bytes in a complete message, zero if data byte passed
     */
 int getBytesPerMessage(uint8_t statusByte);

    /**
     * @param msg
     * @param offset
     * @param count
     * @return true if the entire message is ActiveSensing commands
     */
 bool isAllActiveSensing(uint8_t *msg, int offset,int count);


enum CHANNEL_COMMANDS {

    NOTE_OFF = 0,
    NOTE_ON,
    POLY_TOUCH,
    CONTROL,
    PROGRAM,
    PRESSURE,
    BEND
};


const char *CHANNEL_COMMAND_NAMES[7] = { "NoteOff", "NoteOn","PolyTouch", "Control", "Program", "Pressure", "Bend" };

enum SYSTEM_COMMANDS {

        SYSEX=0,
        TIMECODE,
        SONGPOS,
        SONGSEL,
        F4,
        F5,
        TUNEREQ,
        ENDSYSEX,
        TIMINGCLOCK,
        F9,
        START,
        CONTINUE,
        STOP,
        FD,
        ACTIVESENSING,
        RESET
};

const char *SYSTEM_COMMAND_NAMES[16] = { "SysEx", // F0
                                                      "TimeCode",    // F1
                                                      "SongPos",     // F2
                                                      "SongSel",     // F3
                                                      "F4",          // F4
                                                      "F5",          // F5
                                                      "TuneReq",     // F6
                                                      "EndSysex",    // F7
                                                      "TimingClock", // F8
                                                      "F9",          // F9
                                                      "Start",       // FA
                                                      "Continue",    // FB
                                                      "Stop",        // FC
                                                      "FD",          // FD
                                                      "ActiveSensing", // FE
                                                      "Reset"        // FF
    };

const char * getName(int status) {
        if (status >= 0xF0) {
            int index = status & 0x0F;
            return SYSTEM_COMMAND_NAMES[index];
        } else if (status >= 0x80) {
            int index = (status >> 4) & 0x07;
            return CHANNEL_COMMAND_NAMES[index];
        } else {
            return "data";
        }
    }


    bool isSystemCommand();

    bool isChannelCommand();

    CHANNEL_COMMANDS getChannelCommand();
    SYSTEM_COMMANDS  getSystemCommand() {return systemCommand;}
    int getNote() {return note;}
    int getVelocity() {return velocity;}
    int getProgram() {return program;}
    int getPressure() {return pressure;}
    int getControllerNumber() {return controllerNumber;}
    int getControllerValue() {return controllerValue;}
    int getChannel() {return channel;}

private:

    uint8_t *msg;
    int offset;
    int count;
    long timeStamp;
    int status;
    bool channelMessage = false;
    bool systemMessage = false;
    bool dataMessage = false;
    uint8_t statusByte;
    int channel = 0;
    int note = 0;
    int velocity = 0;
    int pressure = 0;
    int program = 0;
    int controllerNumber = 0;
    int controllerValue = 0;

    SYSTEM_COMMANDS systemCommand;
    CHANNEL_COMMANDS  channelCommand;


};


#endif //FLUIDSYNTH_MIDIMESSAGE_H

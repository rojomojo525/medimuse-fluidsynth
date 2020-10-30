//
// Created by Peter Slack on 2020-10-29.
//

#include "MidiMessage.h"

int MidiMessage::getBytesPerMessage(uint8_t statusByte) {
    // Java bytes are signed so we need to mask off the high bits
    // to get a value between 0 and 255.
    int statusInt = statusByte & 0xFF;
    if (statusInt >= 0xF0) {
        // System messages use low nibble for size.
        return SYSTEM_BYTE_LENGTHS[statusInt & 0x0F];
    } else if(statusInt >= 0x80) {
        // Channel voice messages use high nibble for size.
        return CHANNEL_BYTE_LENGTHS[(statusInt >> 4) - 8];
    } else {
        return 0; // data byte
    }
}


bool MidiMessage::isAllActiveSensing(uint8_t *msg, int offset,
                        int count) {
    // Count bytes that are not active sensing.
    int goodBytes = 0;
    for (int i = 0; i < count; i++) {
        uint8_t b = msg[offset + i];
        if (b != STATUS_ACTIVE_SENSING) {
            goodBytes++;
        }
    }
    return (goodBytes == 0);
}

MidiMessage::MidiMessage(uint8_t *msg,int offset,int count, long timeStamp){

    this->msg = msg;
    this->offset = offset;
    this->count = count;
    this->timeStamp = timeStamp;

    statusByte = msg[offset++];
    status = statusByte & 0xFF;


    int numData = getBytesPerMessage(statusByte) - 1;
    if ((status >= 0x80) && (status < 0xF0)) { // channel message
        channelMessage = true;
        systemMessage = false;
        dataMessage = false;
        channel = status & 0x0F;
        int cindex = (status >>4) - 8;
        channelCommand = (CHANNEL_COMMANDS) cindex;

        uint8_t data[3];
        for (int i=0; i<numData ; i++) {
            data[i] = msg[offset++];
        }

        switch (channelCommand) {
            case CHANNEL_COMMANDS::NOTE_ON:
                note = data[0];
                velocity = data[1];
                break;
            case CHANNEL_COMMANDS::BEND:
                break;
            case CHANNEL_COMMANDS::CONTROL:
                controllerNumber = data[0];
                controllerValue = data[1];
                break;
            case CHANNEL_COMMANDS::NOTE_OFF:
                note = data[0];
                velocity =data[1];
                break;
            case CHANNEL_COMMANDS::POLY_TOUCH:
                note = data[0];
                pressure = data[1];
                break;
            case CHANNEL_COMMANDS::PRESSURE:
                pressure = data[0];
                break;
            case CHANNEL_COMMANDS::PROGRAM:
                program = data[0];
                break;


        }
    } else if (status >= 0xF0) {
        channelMessage = false;
        systemMessage = true;
        dataMessage = false;
        int sindex = status & 0x0F;
        systemCommand = (SYSTEM_COMMANDS) sindex ;

    } else {
        channelMessage = false;
        systemMessage = false;
        dataMessage = true;

    }


}

bool MidiMessage::isSystemCommand() {
    return systemMessage;
}

bool MidiMessage::isChannelCommand() {
    return channelMessage;
}

MidiMessage::CHANNEL_COMMANDS MidiMessage::getChannelCommand() {
    return channelCommand;
}


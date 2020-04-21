#ifndef __RC_CHANNELS_H
#define __RC_CHANNELS_H

#include "platform.h"
#include "CRSF.h" // N_SWITCHES
#include <stdint.h>

#define OPTIMIZED_SEARCH 1

// expresslrs packet header types
// 00 -> standard 4 channel data packet
// 01 -> switch data packet
// 11 -> tlm packet
// 10 -> sync packet with hop data
enum
{
    RC_DATA_PACKET = 0b00,
    SWITCH_DATA_PACKET = 0b01,
    SYNC_PACKET = 0b10,
    TLM_PACKET = 0b11,
};

class RcChannels
{
public:
    RcChannels() {}

    // TX related
    void ICACHE_RAM_ATTR processChannels(crsf_channels_t const *const channels);
    void ICACHE_RAM_ATTR channels_pack(uint8_t *const output);

    // RX related
    void ICACHE_RAM_ATTR channels_extract(volatile uint8_t const *const input,
                                          crsf_channels_t &output);

private:
    // Switches / AUX channel handling
    uint8_t ICACHE_RAM_ATTR getNextSwitchIndex(void);

    // Channel processing data
    volatile uint16_t ChannelDataIn[N_CHANNELS] = {0};  // range: 0...2048
    volatile uint8_t currentSwitches[N_SWITCHES] = {0}; // range: 0,1,2
#if !OPTIMIZED_SEARCH
    volatile uint8_t sentSwitches[N_SWITCHES] = {0};
#endif

    volatile uint16_t p_auxChannelsChanged = 0; // bitmap of changed switches
    // which switch should be sent in the next rc packet
    volatile uint8_t p_nextSwitchIndex = 0;

#if !defined(HYBRID_SWITCHES_8) && !defined(SEQ_SWITCHES)
    uint32_t SwitchPacketNextSend = 0; //time in ms when the next switch data packet will be send
#define SWITCH_PACKET_SEND_INTERVAL 200u
#endif
};

#endif /* __RC_CHANNELS_H */
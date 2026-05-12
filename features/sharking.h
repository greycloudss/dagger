#pragma once
#include <iostream>
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <array>
#include <unistd.h>
#include <vector>
#include <pthread.h>
#include <pcap.h>
#include "packet.h"

namespace FEAT {
    // typedef enum {
    //     ETH,
    //     WIFI
    //     //BOTH
    // } A_TYPE;

    typedef struct {
        pcap_pkthdr* hdr;
        const u_char* data;
    } dag_packet;

    class Adapter {
        private:
            volatile bool killswitch;

            bool cType; // 0 is wlan, 1 is eth
            char errbuf[PCAP_ERRBUF_SIZE];
            std::string conType;
            pcap_t* cap;

            pcap_if_t* allDevs;
            pcap_if_t* dev;
            const u_char* packet;
            std::vector<Packet*> packets;

            bool findDev();
            void captureLive();
            void looperTrooper();
        public:
            Adapter();
            void capture(bool Live);

            void kill();
            void revive();
            void* displayPacket(pcap_pkthdr* hdr, const u_char* data);

            ~Adapter() = default;
            
    };
};
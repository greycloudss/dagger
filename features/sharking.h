#pragma once
#include <iostream>
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <array>
#include <unistd.h>
#include <vector>

#include <pcap.h>

namespace FEAT {
    // typedef enum {
    //     ETH,
    //     WIFI
    //     //BOTH
    // } A_TYPE;

    class Loopback {
        private:
            bool cType; // 0 is wlan, 1 is eth
            char errbuf[PCAP_ERRBUF_SIZE];
            std::string conType;
            pcap_t* cap;

            pcap_if_t* allDevs;
            pcap_if_t* dev;
            const u_char* packet;

            void findDev();
            void captureDead();
            void captureLive();
        public:
            Loopback();
            void capture(bool Live);
    };
};
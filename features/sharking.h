#pragma once
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <array>
#include <cstdio>
#include <string>

namespace FEAT {
    // typedef enum {
    //     ETH,
    //     WIFI
    //     //BOTH
    // } A_TYPE;

    class Loopback {
        bool cType; // 0 is wlan, 1 is eth
        private:
            std::vector<std::string> captured;
            std::string conType;
        public:
            Loopback();
            bool writeLP();
    };
};
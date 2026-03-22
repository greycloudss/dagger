#pragma once
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>

namespace FEAT {
    // typedef enum {
    //     ETH,
    //     WIFI
    //     //BOTH
    // } A_TYPE;

    class Loopback {
        bool aType;
        private:
            std::vector<std::string> captured;
        public:
            Loopback();
            bool writeLP();
    };
};
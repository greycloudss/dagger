#include "sharking.h"

namespace FEAT {
    std::string findConnectionType() { // is redundant
        std::array<char, 128> buf;
        std::string out;

        FILE* f = popen("ip route | awk '/default/ {print $5; exit}'", "r");
        if (!f) return {};

        while (fgets(buf.data(), buf.size(), f)) out += buf.data();
        return out;
    }

    bool Loopback::findDev() {
        if (pcap_findalldevs(&allDevs, errbuf) == -1) return false;
        
        for (dev = allDevs; dev; dev = dev->next)
            if (dev->flags & PCAP_IF_LOOPBACK != 0) return true;

        return false;
    }

    Loopback::Loopback() {
        std::string type = findConnectionType();

        if (type == std::string({})) {
            throw std::runtime_error("[Error] sort your connection out.");
            return;
        }

        cType = 1; // presume always eth
        errbuf[PCAP_ERRBUF_SIZE] = {};
        conType = nullptr;
        cap = nullptr;

        allDevs = nullptr;
        dev = nullptr;
        packet = nullptr;
        
        this->cType = type.c_str()[0] == 'w' ? 0 : 1;
        
        findDev()
    }

    
    void Loopback::captureDead() {
        cap = pcap_open_dead(dev->name, 65535);
    }

    void Loopback::captureLive() {
        cap = pcap_open_live(dev->name, 65535, 1, 1000, errbuf);
    }

    void Loopback::capture(bool live) {
        if (live) captureLive();
        else captureDead();
    }
}
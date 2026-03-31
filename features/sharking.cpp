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

        printf("[INFO] adapter name: %s\n", type);
        
        findDev();
    }

    void Loopback::looperTrooper() {
        dag_packet pkt = {};
        while (!this->killswitch) {
            pcap_next_ex(cap, pkt->hdr, pkt->data);
        }
    }

    void Loopback::captureLive() {
        cap = pcap_open_live(dev->name, 65535, 1, 1000, errbuf);
        
    }

    void Loopback::displayPacket(pcap_pkthdr* hdr, const u_char* data) {
        
    }

    void Loopback::kill() {
        this->killswitch = false;
    }

    void Loopback::revive() {
        this->killswitch = true;
    }
}


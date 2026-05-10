#include "sharking.h"

#define MAX_PACKETS 10000

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
            if ((dev->flags & PCAP_IF_LOOPBACK) != 0) return true;

        return false;
    }

    Loopback::Loopback() {
        std::string type = findConnectionType();

        if (type.empty()) {
            throw std::runtime_error("[Error] sort your connection out.");
        }

        cType = 1;
        errbuf[0] = '\0';
        conType = type;
        cap = nullptr;

        allDevs = nullptr;
        dev = nullptr;
        packet = nullptr;
        killswitch = true;

        this->cType = type.c_str()[0] == 'w' ? 0 : 1;

        printf("[INFO] adapter name: %s\n", type.c_str());

        findDev();
    }

    void FEAT::Loopback::looperTrooper() {
        dag_packet pkt = {};

        while (killswitch) {
            int res = pcap_next_ex(cap, &pkt.hdr, &pkt.data);

            if (res <= 0)
                continue;

            displayPacket(pkt.hdr, pkt.data);
        }
    }

    void FEAT::Loopback::captureLive() {
        cap = pcap_open_live(dev->name, 65535, 1, 1000, errbuf);

        if (!cap) {
            printf("[ERROR] %s\n", errbuf);
            exit(1);
        }

        printf("[INFO] listening on %s\n", dev->name);
    }

    void* FEAT::Loopback::displayPacket(pcap_pkthdr* hdr, const u_char* data) {
        printf("packet size: %d\n", hdr->len);

        for (u_int i = 0; i < hdr->len && i < 64; i++) {
            printf("%02X ", data[i]);
        }

        printf("\n\n");

        return nullptr;
    }

    void FEAT::Loopback::capture(bool Live) {
        if (Live) {
            captureLive();
            looperTrooper();
        }
    }

    void Loopback::kill() {
        this->killswitch = false;
    }

    void Loopback::revive() {
        this->killswitch = true;
    }
}
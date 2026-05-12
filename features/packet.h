#include <string>
#include <sstream>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include "protocols.h"

namespace FEAT {
    struct EthernetHeader {
        unsigned char destination[6];
        unsigned char source[6];
        uint16_t etherType;
    };

    struct IPv4Header {
        unsigned version;
        unsigned ihl;
        unsigned serviceType;
        unsigned totalLength;
        unsigned identification;
        unsigned flags;
        unsigned fragmentOffset;
        unsigned ttl;
        unsigned protocol;
        unsigned checksum;
        uint32_t source;
        uint32_t destination;
    };

    static std::string byteToHex(unsigned char b) {
        static const char* digits = "0123456789ABCDEF";
        std::string s;
        s += digits[(b >> 4) & 0x0F];
        s += digits[b & 0x0F];
        return s;
    }

    static std::string macToString(const unsigned char* mac) {
        return byteToHex(mac[0]) + ":" +
            byteToHex(mac[1]) + ":" +
            byteToHex(mac[2]) + ":" +
            byteToHex(mac[3]) + ":" +
            byteToHex(mac[4]) + ":" +
            byteToHex(mac[5]);
    }

    static std::string ipToString(uint32_t ip) {
        std::ostringstream out;
        out << ((ip >> 24) & 0xFF) << '.'
            << ((ip >> 16) & 0xFF) << '.'
            << ((ip >> 8) & 0xFF) << '.'
            << (ip & 0xFF);
        return out.str();
    }

    static std::string formatEthernet(const EthernetHeader& h) {
        std::ostringstream out;
        out
            << "Destination MAC: " << macToString(h.destination) << '\n'
            << "Source MAC: " << macToString(h.source) << '\n'
            << "EtherType: 0x" << byteToHex((h.etherType >> 8) & 0xFF) << byteToHex(h.etherType & 0xFF);
        return out.str();
    }

    static std::string guessOSFromTTL(unsigned ttl) {
        if (ttl >= 128 && ttl <= 255) {
            if (ttl == 255) return "Cisco Device";
            if (ttl >= 128 && ttl < 256) return "Windows OS (likely Windows Server or Desktop)";
        } else if (ttl >= 64 && ttl < 128) {
            if (ttl == 64) return "Linux/FreeBSD/macOS/Unix-like";
            if (ttl == 100) return "IBM OS/2";
            if (ttl == 127 || ttl == 190) return "macOS";
        } else if (ttl >= 30 && ttl < 64) {
            if (ttl == 50) return "Windows 95/98/ME";
            if (ttl == 60) return "AIX";
            if (ttl == 48) return "BSDI";
            if (ttl == 30) return "SunOS";
        } else if (ttl >= 200 && ttl < 256) {
            if (ttl == 240) return "Novell";
            if (ttl == 254) return "Solaris/AIX";
            if (ttl == 200) return "HP-UX";
        }

        if (ttl >= 50 && ttl <= 64) return "Likely Unix/Linux or macOS variant";
        if (ttl >= 120 && ttl <= 130) return "Likely Windows variant";
        return "Unknown OS or device";
    }

    static std::string formatIPv4(const IPv4Header& h) {
        std::ostringstream out;
        out
            << "Version: " << h.version << '\n'
            << "IHL: " << h.ihl << '\n'
            << "Service Type: " << h.serviceType << '\n'
            << "Total Length: " << h.totalLength << '\n'
            << "Identification: " << h.identification << '\n'
            << "Flags: " << h.flags << '\n'
            << "Fragment Offset: " << h.fragmentOffset << '\n'
            << "TTL: " << h.ttl << '\n'
            << "Likely Operating System: " << guessOSFromTTL(h.ttl) << '\n'
            << "Protocol: " << h.protocol << '\n'
            << "Protocol Name: " << protocolToString(h.protocol) << '\n'
            << "Header Checksum: " << h.checksum << '\n'
            << "Source: " << ipToString(h.source) << '\n'
            << "Destination: " << ipToString(h.destination);
        return out.str();
    }

    class Packet {
        private:
            unsigned char* rawBytes;
            unsigned int length;
            std::string text;

            static uint16_t read16(const unsigned char* p) {
                return (static_cast<uint16_t>(p[0]) << 8) | static_cast<uint16_t>(p[1]);
            }

            static uint32_t read32(const unsigned char* p) {
                return (static_cast<uint32_t>(p[0]) << 24) |
                    (static_cast<uint32_t>(p[1]) << 16) |
                    (static_cast<uint32_t>(p[2]) << 8) |
                    static_cast<uint32_t>(p[3]);
            }

        EthernetHeader parseEthernet() const {
            if (length < 14) throw std::runtime_error("[\033[31mERROR\033[0m] packet too short for Ethernet header");

            EthernetHeader h;
            std::memcpy(h.destination, rawBytes, 6);
            std::memcpy(h.source, rawBytes + 6, 6);
            h.etherType = read16(rawBytes + 12);
            return h;
        }

        IPv4Header parseIPv4(unsigned offset) const {
            if (length < offset + 20) throw std::runtime_error("[\033[31mERROR\033[0m] packet too short for IPv4 header");

            const unsigned char* p = rawBytes + offset;

            IPv4Header h;
            h.version = (p[0] >> 4) & 0x0F;
            h.ihl = p[0] & 0x0F;
            h.serviceType = p[1];
            h.totalLength = read16(p + 2);
            h.identification = read16(p + 4);
            h.flags = (p[6] >> 5) & 0x07;
            h.fragmentOffset = ((p[6] & 0x1F) << 8) | p[7];
            h.ttl = p[8];
            h.protocol = p[9];
            h.checksum = read16(p + 10);
            h.source = read32(p + 12);
            h.destination = read32(p + 16);
            return h;
        }

        public:
            Packet(const std::string& s) : length(s.size()) {
                rawBytes = static_cast<unsigned char*>(std::malloc(length));
                if (!rawBytes) throw std::bad_alloc();
                
                std::memcpy(rawBytes, s.data(), length);
                this->text = formatEthernet(parseEthernet()) + "\n" + formatIPv4(parseIPv4(14));
            }

            Packet(const unsigned char* rawBytes, unsigned int length) : length(length) {
                if (!rawBytes) throw std::invalid_argument("[\033[31mERROR\033[0m] Invalid byte input to packet constructor");

                this->rawBytes = static_cast<unsigned char*>(std::malloc(length));
                if (!this->rawBytes) throw std::bad_alloc();

                std::memcpy(this->rawBytes, rawBytes, length);
                this->text = formatEthernet(parseEthernet()) + "\n" + formatIPv4(parseIPv4(14));
            }

            ~Packet() {
                std::free(rawBytes);
            }

            const std::string& getText() const {
                return text;
            }
    };
}
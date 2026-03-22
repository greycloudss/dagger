#include "sharking.h"
#include <string>

namespace FEAT {
    std::string findConnectionType() {
        std::array<char, 128> buf;
        std::string out;

        FILE* f = popen("ip route | awk '/default/ {print $5; exit}'", "r");
        if (!f) return {};

        while (fgets(buf.data(), buf.size(), f)) out += buf.data();
        return out;
    }

    

    Loopback::Loopback() {
        std::string type = findConnectionType();

        if (type == std::string({})) {
            throw std::runtime_error("[Error] sort your connection out.");
            return;
        }
        
        this->cType = type.c_str()[0] == 'w' ? 0 : 1;
    }

    bool Loopback::writeLP() {

        return {};
    }
}
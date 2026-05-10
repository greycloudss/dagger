#include <iostream>
#include "main.h"




int main(int argc, const char* argv[]) {
    FEAT::Loopback* dagger = new FEAT::Loopback();

    MAIN::interface(dagger);

    return 0;
}
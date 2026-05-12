#include <iostream>
#include "main.h"




int main(int argc, const char* argv[]) {
    FEAT::Adapter* dagger = new FEAT::Adapter();

    MAIN::interface(dagger);

    return 0;
}
#include <iostream>
#include "main.h"




int main() {
    FEAT::Adapter* dagger = new FEAT::Adapter();

    MAIN::interface(dagger);

    return 0;
}
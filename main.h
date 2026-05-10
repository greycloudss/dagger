#pragma once
#include "features/sharking.h"


namespace MAIN {
    int read_char() {
        int c = getchar();
        int d;

        while ((d = getchar()) != '\n' && d != EOF) {}

        return c;
    }

    void interface(FEAT::Loopback* lpb) {
        unsigned char sel = 0;

        while (true) {
            sel = read_char();


            //temporary cases
            switch (sel) {
                case '1':
                    lpb->revive();
                    lpb->capture(true);
                    break;

                case '2':
                    lpb->revive();
                    lpb->capture(true);
                    break;

                default:
                    printf("[KILL] hit default, meaning kill\n");
                    lpb->kill();
                    return;
                }
            }
    }
}
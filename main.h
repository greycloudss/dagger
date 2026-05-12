#pragma once
#include "features/sharking.h"

namespace MAIN {
    int read_char() {
        int c = getchar();
        int d;

        while ((d = getchar()) != '\n' && d != EOF) {}

        return c;
    }

    void interface(FEAT::Adapter* lpb) {
        unsigned char sel = 0;

        printf("Selection:\n1 - listen to lo adapter\n2 - listen to connection adapter\n\n> :");

        while (true) {
            sel = read_char();
            printf("\n");
            switch (sel) {
                case '1':
                    if (!lpb->selectLoopbackDev()) {
                        printf("[\033[31mERROR\033[0m] failed to select loopback adapter\n");
                        break;
                    }
                    lpb->revive();
                    lpb->capture(true);
                    break;

                case '2':
                    if (!lpb->selectConnectionDev()) {
                        printf("[\033[31mERROR\033[0m] failed to select connection adapter\n");
                        break;
                    }
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
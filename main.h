#pragma once
#include "features/sharking.h"

namespace MAIN {
    void interface(FEAT::Loopback* lpb) {
        unsigned char sel = 0;
        jmper: 
        while (true) {
            sel = getchar();
            switch (sel) {
                case 1: // capture timed
                    continue;
                
                case 2: // capture indef
                    continue;
                
                default: //exit
                    printf("[KILL] hit default, meaning kill\n");
                    lpb->kill();
                    break;
            }
        }
        printf("[INFO] Do you wish to exit? Y/n: ");
        sel = getchar();
        if (sel == 0x6E || sel == 0x4E) return;
        else {
            lpb->revive();
            goto jmper;
        }
    }
};
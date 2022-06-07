#pragma once

#include "defines.h"
#include "seq_queue.h"

class reg_file
{
private:
    uint32_t reg_r0;
    uint32_t reg_r1;
    uint32_t reg_r2;
    uint32_t reg_r3;
    uint32_t reg_r4;
    uint32_t reg_r5;
    uint32_t reg_r6;
    uint32_t reg_r7;
    uint32_t reg_r8;
    uint32_t reg_r9;
    uint32_t reg_r10;
    uint32_t reg_r11;
    uint32_t reg_r12;
    uint32_t reg_r13;
    uint32_t reg_r14;
    uint32_t reg_r15;
    uint32_t reg_r16;
    uint32_t reg_r17;
    uint32_t reg_r18;
    uint32_t reg_r19;
    uint32_t reg_r20;
    uint32_t reg_r21;
    uint32_t reg_r22;
    uint32_t reg_r23;
    uint32_t reg_r24;
    uint32_t reg_r25;
    uint32_t reg_r26;
    uint32_t reg_r27;
    uint32_t reg_r28;
    uint32_t reg_r29;
    uint32_t reg_r30;
    uint32_t reg_r31;

public:
    seq_queue q;

    // TODO: what happens when logic_t and queue should be private?

private:

public:
    void write();
    void read();

};
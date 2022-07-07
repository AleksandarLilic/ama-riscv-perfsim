/*
// Project:         AMA-RISCV-SIM
// File:            asm_test.ld
// Date created:    2022-07-05
// Author:          Aleksandar Lilic
// Description:     Linker script for assembly tests
//
// SPDX-License-Identifier: GPL-3.0-or-later
*/

OUTPUT_ARCH( "riscv" )
ENTRY( _start )

SECTIONS
{
    . = 0x00000000;
    .text : {
        * (.start);
        * (.text);
    }
}

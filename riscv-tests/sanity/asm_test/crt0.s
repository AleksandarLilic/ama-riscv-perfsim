#
# Project:         AMA-RISCV-SIM
# File:            crt0.s
# Date created:    2022-07-05
# Author:          Aleksandar Lilic
# Description:     Assembly test
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

.section    .start
.global     _start

_start:
j r_type

r_type:
j op_add

op_add:
li x11, 35 # load rs1
li x12, 65 # load rs2
add x10, x11, x12 # execute tested op
li x30, 100 # load expected result
li x3, 1 # load test_id
bne x30, x10, fail # test op

op_sub:
li x11, 257 # load rs1
li x12, 56 # load rs2
sub x10, x11, x12 # execute tested op
li x30, 201 # load expected result
li x3, 2 # load test_id
bne x30, x10, fail # test op

op_sll:
li x11, 0xF0 # load rs1
li x12, 3 # load rs2
sll x10, x11, x12 # execute tested op
li x30, 0x780 # load expected result
li x3, 3 # load test_id
bne x30, x10, fail # test op

op_srl:
li x11, 0x1F # load rs1
li x12, 2 # load rs2
srl x10, x11, x12 # execute tested op
li x30, 0x7 # load expected result
li x3, 4 # load test_id
bne x30, x10, fail # test op

op_sra:
li x11, 0x83330000 # load rs1
li x12, 5 # load rs2
sra x10, x11, x12 # execute tested op
li x30, 0xFC199800 # load expected result
li x3, 5 # load test_id
bne x30, x10, fail # test op

op_slt:
li x11, -7 # load rs1
li x12, 7 # load rs2
slt x10, x11, x12 # execute tested op
li x30, 1 # load expected result
li x3, 6 # load test_id
bne x30, x10, fail # test op

op_sltu:
li x11, 775 # load rs1
li x12, 99 # load rs2
sltu x10, x11, x12 # execute tested op
li x30, 0 # load expected result
li x3, 7 # load test_id
bne x30, x10, fail # test op

op_xor:
li x11, 0xF1 # load rs1
li x12, 0xF2 # load rs2
xor x10, x11, x12 # execute tested op
li x30, 0x3 # load expected result
li x3, 8 # load test_id
bne x30, x10, fail # test op

op_or:
li x11, 0xAA # load rs1
li x12, 0x05 # load rs2
or x10, x11, x12 # execute tested op
li x30, 0xAF # load expected result
li x3, 9 # load test_id
bne x30, x10, fail # test op

op_and:
li x11, 0x202 # load rs1
li x12, 0x304 # load rs2
and x10, x11, x12 # execute tested op
li x30, 0x200 # load expected result
li x3, 10 # load test_id
bne x30, x10, fail # test op

i_type:
j op_addi

op_addi:
li x11, 35 # load rs1
addi x10, x11, -36 # execute tested op
li x30, -1 # load expected result
li x3, 11 # load test_id
bne x30, x10, fail # test op

op_slti:
li x11, -18 # load rs1
slti x10, x11, -17 # execute tested op
li x30, 1 # load expected result
li x3, 12 # load test_id
bne x30, x10, fail # test op

op_sltiu:
li x11, 683 # load rs1
sltiu x10, x11, 682 # execute tested op
li x30, 0 # load expected result
li x3, 13 # load test_id
bne x30, x10, fail # test op

op_xori:
li x11, 0xFFFFAAAA # load rs1
xori x10, x11, 0x555 # execute tested op
li x30, 0xFFFFAFFF # load expected result
li x3, 14 # load test_id
bne x30, x10, fail # test op

op_ori:
li x11, 0xFFFF00FF # load rs1
ori x10, x11, 0x100 # execute tested op
li x30, 0xFFFF01FF # load expected result
li x3, 15 # load test_id
bne x30, x10, fail # test op

op_andi:
li x11, 0xA7A70202 # load rs1
andi x10, x11, 0x0FF # execute tested op
li x30, 0x00000002 # load expected result
li x3, 16 # load test_id
bne x30, x10, fail # test op

op_slli:
li x11, 0xFFFF0001 # load rs1
slli x10, x11, 31 # execute tested op
li x30, 0x80000000 # load expected result
li x3, 17 # load test_id
bne x30, x10, fail # test op

op_srli:
li x11, 0xFF001100 # load rs1
srli x10, x11, 29 # execute tested op
li x30, 7 # load expected result
li x3, 18 # load test_id
bne x30, x10, fail # test op

op_srai:
li x11, 0xF0000000 # load rs1
srai x10, x11, 13 # execute tested op
li x30, 0xFFFF8000 # load expected result
li x3, 19 # load test_id
bne x30, x10, fail # test op

load_t:
j op_lb

op_lb:
lla x11, dat1 # load dmem offset
lb x10, 0(x11) # execute tested op
li x30, 0x12 # load expected result
li x3, 20 # load test_id
bne x30, x10, fail # test op

op_lb2:
lla x11, dat2 # load dmem offset
lb x10, 1(x11)
li x30, 0x03 # load expected result
li x3, 21 # load test_id
bne x30, x10, fail # test op

done: j done # loop

fail: 
add x25, x0, x3 # store failed test id in x25

failed: j failed

.data

dat1: .word 0x5555ff12
dat2: .word 0xfff10357

# add x10, x11, x12
# sub x10, x11, x12
# sll x10, x11, x12
# srl x10, x11, x12
# sra x10, x11, x12
# slt x10, x11, x12
# sltu x10, x11, x12
# xor x10, x11, x12
# or x10, x11, x12
# and x10, x11, x12
# 
# loop:
# addi x10, x11, 37 
# slti x10, x11, 37
# sltiu x10, x11, 37
# xori x10, x11, 37
# ori x10, x11, 37
# andi x10, x11, 37
# slli x10, x11, 3
# srli x10, x11, 3
# srai x10, x11, 3
# 
# lb x10, 4(x11)
# lh x10, 4(x11)
# lw x10, 4(x11)
# lbu x10, 4(x11)
# lhu x10, 4(x11)
# 
# sb x10, 4(x11)
# sh x10, 4(x11)
# sw x10, 4(x11)
# 
# beq x10, x11, loop
# bne x10, x11, loop
# blt x10, x11, loop
# bge x10, x11, loop
# bltu x10, x11, loop
# bgeu x10, x11, loop
# 
# jalr x10, x11, 4
# j loop
# 
# test:
# lui x10, 0x1000
# auipc x10, 0x1000
# 
# add x8, x5, x6
# addi x11, x8, 15
# addi x12, x8, 99


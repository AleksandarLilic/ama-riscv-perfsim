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

op_add:
li x11, 35 # load rs1
li x12, 65 # load rs2
add x10, x11, x12 # execute tested op
li x20, 100 # load expected result
li x3, 1 # load test_id
bne x20, x10, fail # test op

j done # if no error

fail: 
add x25, x0, x3 # store failed test id in x25

done: j done # loop

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

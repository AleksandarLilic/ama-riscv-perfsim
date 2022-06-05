//
// Project:         AMA-RISCV-SIM
// File:            logic_t.h
// Date created:    2022-05-16
// Author:          Aleksandar Lilic
// Description:     Custom data type, designed to model sequential logic
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

#pragma once

#include "defines.h"
#include <vector>

// forward declaration, seq_queue.h included in the logic_t.cpp
class seq_queue;

class logic_t
{
private:
    //uint32_t logic_in;
    //uint32_t* uint_in;
    uint32_t hold;
    uint32_t logic_reg;
    uint32_t rst_value;
    bool enable;
    std::string id;
    uint32_t *connected_input_uint;
    std::vector<uint32_t*> connected_outputs_uint;
    std::vector<logic_t*> connected_outputs_logic;

public:

    //uint32_t len;
    //uint32_t mask;
    //void init_mask(uint32_t len_in) { for (uint32_t i = 0; i < len; i++) mask = (mask << 1) | 1; }

private:
    void enqueue(seq_queue *q);
    void init(uint32_t init_val, std::string init_id);

public:
    // Constructors
    logic_t() = delete;     // always provide queue
    logic_t(seq_queue *q, uint32_t init_val, std::string init_id);

    // Methods
    void connect_in(uint32_t *connection);
    void connect(uint32_t *connection);
    void connect(logic_t *connection);
    void rst();  // sync rst, needs active edge to take reset value
    void set_enable(bool enable);
    void clk_update_hold();
    void clk_update();

    // Getters
    std::string get_id() const;
    bool get_en() const;
    uint32_t out() const;

    //    // Operator overloads - logic_t with an integer
    //    void operator= (const uint32_t operand);
    //    uint32_t operator+ (const uint32_t operand) const;
    //    uint32_t operator- (const uint32_t operand) const;
    //    void operator+= (const uint32_t operand);
    //    void operator-= (const uint32_t operand);
    //    void operator++ (int);
    //    void operator-- (int);
    //    // Operator overloads - logic_t with a logic_t
    //    void operator= (const logic_t operand);
    //    uint32_t operator+ (const logic_t operand) const;
    //    uint32_t operator- (const logic_t operand) const;

};

// Non-member operator overloads
std::ostream &operator<<(std::ostream &stream, const logic_t &operand);

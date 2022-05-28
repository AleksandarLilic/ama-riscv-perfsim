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

typedef class logic_t
{
private:
    uint32_t logic_in;
    uint32_t hold;
    uint32_t logic_reg;
    std::vector<uint32_t*> connected_outputs_uint;
    std::vector<logic_t*> connected_outputs_logic;
    uint32_t rst_value;
    bool enable;
    std::string name;

public:

    //uint32_t len;
    //uint32_t mask;
    //void init_mask(uint32_t len_in) { for (uint32_t i = 0; i < len; i++) mask = (mask << 1) | 1; }

public:
    // Constructors
    logic_t();
    logic_t(uint32_t init_val, std::string init_name);

    // Methods
    void init(uint32_t init_val, std::string init_name);
    void connect(uint32_t *connection);
    void connect(logic_t *connection);
    void rst();  // sync rst, needs active edge to take reset value
    void set_enable(bool enable);
    void clk_update_hold();
    void clk_update();

    // Getters
    std::string get_name() const;
    bool get_en() const;
    uint32_t out() const;

    // Operator overloads - logic_t with an integer
    void operator= (const uint32_t operand);
    uint32_t operator+ (const uint32_t operand) const;
    uint32_t operator- (const uint32_t operand) const;
    void operator+= (const uint32_t operand);
    void operator-= (const uint32_t operand);
    void operator++ (int);
    void operator-- (int);
    // Operator overloads - logic_t with a logic_t
    void operator= (const logic_t operand);
    uint32_t operator+ (const logic_t operand) const;
    uint32_t operator- (const logic_t operand) const;

} logic_t;

// Non-member operator overloads
std::ostream &operator<<(std::ostream &stream, const logic_t &operand);

//
// Project:         AMA-RISCV-SIM
// File:            logic_t.cpp
// Date created:    2022-05-16
// Author:          Aleksandar Lilic
// Description:     Custom data type, designed to model sequential logic
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

#include "../include/logic_t.h"

// Constructors
logic_t::logic_t() : logic_reg(0), logic_in(0), rst_value(0), enable(1), name("no-name") {};
logic_t::logic_t(uint32_t init_val, std::string init_name)
{
    init(init_val, init_name);
}

// Methods
void logic_t::init(uint32_t init_val, std::string init_name)
{
    logic_reg = init_val;
    hold = init_val;
    logic_in = init_val;
    rst_value = init_val;
    enable = 1;
    name = init_name;
}
void logic_t::connect(uint32_t *connection) { connected_outputs_uint.push_back(connection); } 
void logic_t::connect(logic_t *connection) { connected_outputs_logic.push_back(connection); }
void logic_t::rst() { logic_in = rst_value; }  // sync rst, needs active edge to take rst val
void logic_t::set_enable(bool enable) { this->enable = enable; }
void logic_t::clk_update_hold() { hold = logic_in; }
void logic_t::clk_update()
{
    uint32_t prev = logic_reg;
    if (enable) {   // only update if ff is enabled
        logic_reg = hold;
        for (uint32_t* i : connected_outputs_uint)
            *i = hold;
        for (logic_t* i : connected_outputs_logic)
            *i = hold;
    }
    LOG("Called clk_update() on: '" << name << "'; Input: " << hold << "; Enable: " << enable
        << "; Old value: " << prev << "; New Value: " << logic_reg << "; ");
}

// Getters
std::string logic_t::get_name() const { return name; }
bool logic_t::get_en() const { return enable; }
uint32_t logic_t::out() const { return logic_reg; }

// Operator overloads - logic_t with an integer
void logic_t::operator= (const uint32_t operand) { logic_in = operand; }
uint32_t logic_t::operator+ (const uint32_t operand) const { return (out() + operand); }
uint32_t logic_t::operator- (const uint32_t operand) const { return (out() - operand); }
void logic_t::operator+= (const uint32_t operand) { logic_in = out() + operand; }
void logic_t::operator-= (const uint32_t operand) { logic_in = out() - operand; }
void logic_t::operator++ (int) { logic_in++; }
void logic_t::operator-- (int) { logic_in--; }

// Operator overloads - logic_t with a logic_t
void logic_t::operator= (const logic_t operand) { logic_in = operand.out(); }
uint32_t logic_t::operator+ (const logic_t operand) const
{
    uint32_t res;
    res = out() + operand.logic_reg;
    return res;
}
uint32_t logic_t::operator- (const logic_t operand) const
{
    uint32_t res;
    res = out() - operand.logic_reg;
    return res;
}

// Non-member operator overloads
std::ostream& operator<<(std::ostream& stream, const logic_t& operand)
{
    stream << "Logic: " << operand.get_name() << 
        "; Enable: " << operand.get_en() << 
        "; Value: " << operand.out() << "; ";
    return stream;
}

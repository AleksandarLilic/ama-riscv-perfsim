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
#include "../include/seq_queue.h"

// Constructor
logic_t::logic_t(seq_queue *q, uint32_t init_val, std::string init_id)
{
    init(init_val, init_id);
    enqueue(q);
}

// Methods
void logic_t::init(uint32_t init_val, std::string init_id)
{
    logic_reg = init_val;
    hold = init_val;
    rst_value = init_val;
    id = init_id;
}
void logic_t::enqueue(seq_queue *q)
{
    q->add(this);
    //LOG("Enqueue called");
}

//void logic_t::connect_in(uint32_t *connection) { uint_in=connection; }

void logic_t::connect_in(uint32_t *connection) { connected_input = connection; } 
void logic_t::connect_out(uint32_t *connection) { connected_output = connection; } 
void logic_t::connect_rst(uint32_t *connection) { connected_reset = connection; } 
void logic_t::connect_en(uint32_t *connection) { connected_enable = connection; } 
// void logic_t::connect(logic_t *connection) { connected_outputs_logic.push_back(connection); }
void logic_t::update_hold() { hold = *connected_input; }
void logic_t::update()
{
    uint32_t prev = logic_reg;
    if (*connected_enable) {   // only update if ff is enabled
        logic_reg = hold;
        *connected_output = hold;
    }
    if (*connected_reset) {   // overwrite if rst
        logic_reg = rst_value;
        *connected_output = rst_value;
    }
    // write to .txt here
    LOG("Update: '" << id << "'; Input: " << hold << "', HEX: " << FHEX(hold)
        << "; Enable: " << *connected_enable << "; Reset: " << *connected_reset 
        << "; Old Output: " << prev << ", HEX: " << FHEX(prev)
        << "; New Output: " << logic_reg << ", HEX: " << FHEX(logic_reg) << "; ");
}

// Getters
std::string logic_t::get_id() const { return id; }
uint32_t logic_t::out() const { return logic_reg; }

// Operator overloads - logic_t with an integer
//  void logic_t::operator= (const uint32_t operand) { logic_in = operand; }
//  uint32_t logic_t::operator+ (const uint32_t operand) const { return (out() + operand); }
//  uint32_t logic_t::operator- (const uint32_t operand) const { return (out() - operand); }
//  void logic_t::operator+= (const uint32_t operand) { logic_in = out() + operand; }
//  void logic_t::operator-= (const uint32_t operand) { logic_in = out() - operand; }
//  void logic_t::operator++ (int) { logic_in++; }
//  void logic_t::operator-- (int) { logic_in--; }
//  
//  // Operator overloads - logic_t with a logic_t
//  void logic_t::operator= (const logic_t operand) { logic_in = operand.out(); }
//  uint32_t logic_t::operator+ (const logic_t operand) const
//  {
//      uint32_t res;
//      res = out() + operand.logic_reg;
//      return res;
//  }
//  uint32_t logic_t::operator- (const logic_t operand) const
//  {
//      uint32_t res;
//      res = out() - operand.logic_reg;
//      return res;
//  }
//  
//  // Non-member operator overloads
//  std::ostream& operator<<(std::ostream& stream, const logic_t& operand)
//  {
//      stream << "Logic: " << operand.get_id() << 
//          "; Enable: " << operand.get_en() << 
//          "; Value: " << operand.out() << "; ";
//      return stream;
//  }

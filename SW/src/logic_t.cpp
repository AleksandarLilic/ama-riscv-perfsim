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

#ifndef MULTI_LOGIC

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
void logic_t::connect_clr(uint32_t *connection) { connected_clear = connection; } 
// void logic_t::connect(logic_t *connection) { connected_outputs_logic.push_back(connection); }
void logic_t::update_hold() { hold = *connected_input; }
void logic_t::update()
{
    uint32_t prev = logic_reg;
    if (*connected_enable) {   // only update if ff is enabled
        logic_reg = hold;
        *connected_output = hold;
    }
    if (*connected_reset /*| *connected_clear*/) {   // overwrite if rst
        logic_reg = rst_value;
        *connected_output = rst_value;
    }
    // write to .txt here
    LOG("Update: '" << id 
        << "'; Input: " << hold << ", HEX: " << FHEX(hold)
        << "; Reset: " << *connected_reset/* << "; Clear: " << *connected_clear*/
        << "; Enable: " << *connected_enable  
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

#else // !MULTI_LOGIC

logic_port_t::logic_port_t(std::string init_id, uint32_t init_val, uint32_t *din, uint32_t *dout)
{
    rst_value = init_val;
    hold = init_val;
    current = init_val;
    connected_input = din;
    connected_output = dout;
    id = init_id;
}

void logic_port_t::status_log(uint32_t prev)
{
    LOG("    Update: '" << id
        << "'; Input: " << hold << ", HEX: " << FHEX(hold)
        << "; Old Output: " << prev << ", HEX: " << FHEX(prev)
        << "; New Output: " << current << ", HEX: " << FHEX(current) << "; ");
}

void logic_port_t::update_hold()
{
    hold = *connected_input;
}

void logic_port_t::update(uint32_t update_value)
{
    uint32_t prev = current;
    current = update_value;
    *connected_output = update_value;
    status_log(prev);
}

logic_t::logic_t(seq_queue *q, std::string init_id)
{
    id = init_id;
    q->add(this);
}

void logic_t::connect_port(std::string init_id, uint32_t init_val, uint32_t *din, uint32_t *dout)
{
    logic_port_t port_data(init_id, init_val, din, dout);
    connected_ports.push_back(port_data);
}
void logic_t::connect_rst(uint32_t *connection) { connected_reset = connection; }
void logic_t::connect_en(uint32_t *connection) { connected_enable = connection; }
void logic_t::connect_clr(uint32_t *connection) { connected_clear = connection; }

void logic_t::update_hold()
{
    if (*connected_enable & !(*connected_reset | *connected_clear)) {
        for (logic_port_t i : connected_ports)
            i.update_hold();
    }
}

void logic_t::update()
{
    if (*connected_reset) {
        LOG("Resetting: " << id);
        for (logic_port_t i : connected_ports) {
            i.update(i.get_rst_value());
        }
        return;
    }

    if (*connected_clear) {
        LOG("Clearing: " << id);
        for (logic_port_t i : connected_ports) {
        i.update(i.get_rst_value());    // clear and reset value are the same for now
        }
        return;
    }

    if (*connected_enable) {
        LOG("Updating: " << id);
        for (logic_port_t i : connected_ports) {
            i.update(i.get_input_value());
        }
        return;
    }

    LOG("Logic " << id << " not updated as none of the reset, clear or enable signals are active");
    return;
}

#endif
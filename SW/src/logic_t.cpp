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

logic_port_t::logic_port_t(std::string init_id, uint32_t init_val, uint32_t *din, uint32_t *dout) :
    v_exp(init_id)
{
    v_exp.log_table();
    v_exp.log_vector_txt(current);

    id = init_id;
    rst_value = init_val;
    hold = init_val;
    current = init_val;
    connected_input = din;
    connected_output = dout;    
}

logic_port_t::~logic_port_t() {}

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
    if (id == "alu_mem")
        LOG_M("alu_mem current value: " << current << "; update_value: " << update_value);
    *connected_output = update_value;
    vector_log();
#if LOG_LOGIC_T
    status_log(prev);
#endif
}

void logic_port_t::vector_log()
{
    v_exp.log_vector_txt(current);
}

logic_t::logic_t(seq_queue *q, std::string init_id)
{
    id = init_id;
    q->add(this);
}

logic_t::~logic_t()
{
    for (logic_port_t *i : connected_ports)
        delete i;
}

void logic_t::connect_port(std::string init_id, uint32_t init_val, uint32_t *din, uint32_t *dout)
{
    logic_port_t *port;
    port = new logic_port_t(init_id, init_val, din, dout);
    connected_ports.push_back(port);
#if LOG_LOGIC_T
    LOG("Connected Port: " << init_id);
#endif
}
void logic_t::connect_rst(uint32_t *connection) { connected_reset = connection; }
void logic_t::connect_en(uint32_t *connection) { connected_enable = connection; }
void logic_t::connect_clr(uint32_t *connection) { connected_clear = connection; }

void logic_t::update_hold()
{
    if (*connected_enable & !(*connected_reset | *connected_clear)) {
        for (logic_port_t *i : connected_ports)
            i->update_hold();
    }
}

void logic_t::update()
{
    if (*connected_reset) {
        LOG("Resetting: " << id);
        for (logic_port_t *i : connected_ports) {
            i->update(i->get_rst_value());
        }
        return;
    }

    if (*connected_clear) {
        LOG("Clearing: " << id);
        for (logic_port_t *i : connected_ports) {
        i->update(i->get_rst_value());    // clear and reset value are the same for now
        }
        return;
    }

    if (*connected_enable) {
        LOG("Updating: " << id);
        for (logic_port_t *i : connected_ports) {
            i->update(i->get_input_value());
        }
        return;
    }

    // fall through, just log to file
    LOG("Logic " << id << " not updated as none of the reset, clear or enable signals are active");
    for (logic_port_t *i : connected_ports) {
        i->vector_log();
    }
    return;
}

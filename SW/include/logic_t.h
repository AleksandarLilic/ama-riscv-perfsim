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
#include "vector_export.h"

#include <vector>
#include <fstream>

// Forward declaration, seq_queue.h included in the logic_t.cpp
class seq_queue;

class logic_port_t {
private:
    uint32_t rst_value;
    uint32_t *connected_input;
    uint32_t *connected_output;
    uint32_t hold = NOT_RESET;
    uint32_t current = NOT_RESET;
    std::string id;
    vector_export v_exp;
public:
    logic_port_t() = delete;
    logic_port_t(std::string init_id, uint32_t init_val, uint32_t *din, uint32_t *dout);
    ~logic_port_t();
    void update_hold();
    void update(uint32_t update_value);
    void vector_log();
    void status_log(uint32_t prev);
    uint32_t get_rst_value() const { return rst_value; };
    uint32_t get_input_value() const { return hold; };
    std::string get_id() const { return id; };
};

class logic_t
{
private:
    uint32_t *connected_enable;
    uint32_t *connected_reset;
    uint32_t *connected_clear;
    std::string id;
    std::vector<logic_port_t *> connected_ports;

public:
    //uint32_t len;
    //uint32_t mask;
    //void init_mask(uint32_t len_in) { for (uint32_t i = 0; i < len; i++) mask = (mask << 1) | 1; }

public:
    logic_t() = delete;
    logic_t(seq_queue *q, std::string init_id);
    ~logic_t();
    
public:
    void connect_port(std::string init_id, uint32_t init_val, uint32_t *din, uint32_t *dout);
    void connect_rst(uint32_t *connection);
    void connect_en(uint32_t *connection);
    void connect_clr(uint32_t *connection);
    void update_hold();
    void update();

    std::string get_id() const { return id; };
    uint32_t out() const;
};

// Non-member operator overloads
std::ostream &operator<<(std::ostream &stream, const logic_t &operand);

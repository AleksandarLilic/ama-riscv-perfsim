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

// TODO: create virtual logic_t class then extend to get various FF configurations
// 1a. sync rst or 1b. async rst or 1c. no rst
// then add other inputs, extend again
// 1. FF w/ enable
// 2. FF w/ clear

// TODO: every input has to have a connect pointer

#define MULTI_LOGIC

#ifndef MULTI_LOGIC

class logic_t
{
private:
    uint32_t hold;
    uint32_t logic_reg;
    uint32_t rst_value;
    uint32_t *connected_enable;
    uint32_t *connected_reset;
    uint32_t *connected_clear;
    uint32_t *connected_input;
    uint32_t *connected_output;
    std::string id;

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
    void connect_out(uint32_t *connection);
    void connect_rst(uint32_t *connection);
    void connect_en(uint32_t *connection);
    void connect_clr(uint32_t *connection);
    void update_hold();
    void update();

    // Getters
    std::string get_id() const;
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

#else // !MULTI_LOGIC

class logic_port_t {
private:
    uint32_t rst_value;
    uint32_t *connected_input;
    uint32_t *connected_output;
    uint32_t hold;
    uint32_t current;
    std::string id;
public:
    logic_port_t() = delete;
    logic_port_t(std::string init_id, uint32_t init_val, uint32_t *din, uint32_t *dout);
    void update_hold();
    void update(uint32_t update_value);
    void status_log(uint32_t prev);
    uint32_t get_rst_value() { return rst_value; };
    uint32_t get_input_value() { return *connected_input; };
};



class logic_t
{
private:
    uint32_t *connected_enable;
    uint32_t *connected_reset;
    uint32_t *connected_clear;
    std::string id;
    std::vector<logic_port_t> connected_ports;
public:
    //uint32_t len;
    //uint32_t mask;
    //void init_mask(uint32_t len_in) { for (uint32_t i = 0; i < len; i++) mask = (mask << 1) | 1; }
private:
    // void enqueue(seq_queue *q);
    // void init(uint32_t init_val, std::string init_id);

public:
    // Constructors
    logic_t() = delete;     // always provide queue
    logic_t(seq_queue *q, std::string init_id);
    
    // Methods
    void connect_port(std::string init_id, uint32_t init_val, uint32_t *din, uint32_t *dout);
    void connect_rst(uint32_t *connection);
    void connect_en(uint32_t *connection);
    void connect_clr(uint32_t *connection);
    void update_hold();
    void update();

    // Getters
    std::string get_id() const;
    uint32_t out() const;
};

#endif

// Non-member operator overloads
std::ostream &operator<<(std::ostream &stream, const logic_t &operand);

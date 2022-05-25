#pragma once

#include "defines.h"
#include <vector>

typedef class logic_t
{
private:
    uint32_t logic_in;
    std::vector<uint32_t*> connected_outputs_uint;
    uint32_t logic_reg;
    uint32_t rst_value;
    bool enable;
public:
    std::string name;

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
    uint32_t out();
    void rst();  // sync rst, needs active edge to take rst val
    void set_enable(bool enable);
    // void set_name(std::string init_name);
    std::string get_name();
    void clk_update();

    // Operator overloads
    void operator= (const uint32_t operand);
    uint32_t operator+ (const uint32_t operand);
    uint32_t operator- (const uint32_t operand);
    void operator+= (const uint32_t operand);
    void operator-= (const uint32_t operand);
    void operator++ (int);
    void operator-- (int);
    uint32_t operator+ (const logic_t operand);
    uint32_t operator- (const logic_t operand);

} logic_t;

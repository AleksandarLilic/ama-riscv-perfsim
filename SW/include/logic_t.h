#pragma once

#include "defines.h"

typedef class logic_t
{
private:
    // when assigning, always to temp
    // when reading, always from val
    // transfer temp to val only on clk update
    // use uint for 'wire' equivalent

    uint32_t m_logic_in;
    uint32_t m_logic_reg;
    uint32_t m_rst_value;
    bool m_enable;
public:
    std::string m_name;


    //uint32_t len;
    //uint32_t mask;
    //void init_mask(uint32_t len_in) { for (uint32_t i = 0; i < len; i++) mask = (mask << 1) | 1; }

public:
    // Constructors
    logic_t();
    logic_t(uint32_t init);
    logic_t(std::string initn);
    logic_t(uint32_t init, std::string init_n);
    
    void init(std::string initn) {
        m_name = initn;
    };

    // Methods
    uint32_t m_get();
    void m_rst();  // sync rst, needs active edge to take rst val
    void m_set_enable(bool enable_in);
    void m_set_name(std::string init_n);
    void m_clk_update();

    // Operator overloads
    void operator= (const uint32_t val_in);
    uint32_t operator+ (const uint32_t val_in);
    uint32_t operator- (const uint32_t val_in);
    void operator+= (const uint32_t val_in);
    void operator-= (const uint32_t val_in);
    void operator++ (int);
    void operator-- (int);
    uint32_t operator+ (const logic_t logic_in);
    uint32_t operator- (const logic_t logic_in);

} logic_t;

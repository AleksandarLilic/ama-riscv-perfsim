#include "../include/logic_t.h"

// Constructors
logic_t::logic_t() : m_value(0), m_temp_value(0), m_rst_value(0), m_name("N/A") {};
logic_t::logic_t(uint32_t init) : m_value(init), m_temp_value(init), m_rst_value(init), m_name("N/A") {};
logic_t::logic_t(std::string initn) : m_value(0), m_temp_value(0), m_rst_value(0), m_name(initn) {};
logic_t::logic_t(uint32_t init, std::string init_n) : m_value(init), m_temp_value(init), m_rst_value(init), m_name(init_n) {};

// Methods
uint32_t logic_t::m_get() { return m_value; }
void logic_t::m_rst() { m_temp_value = m_rst_value; }  // sync rst, needs active edge to take rst val
void logic_t::m_set_name(std::string init_n) { m_name = init_n; }
void logic_t::m_clk_update()
{
    m_value = m_temp_value;
    LOG("clk_update on: " << m_name);
}

// Operator overloads
void logic_t::operator= (const uint32_t val_in) { m_temp_value = val_in; }
uint32_t logic_t::operator+ (const uint32_t val_in) { return (m_get() + val_in); }
uint32_t logic_t::operator- (const uint32_t val_in) { return (m_get() - val_in); }
void logic_t::operator+= (const uint32_t val_in) { m_temp_value = m_get() + val_in; }
void logic_t::operator-= (const uint32_t val_in) { m_temp_value = m_get() - val_in; }
void logic_t::operator++ (int) { m_temp_value++; }
void logic_t::operator-- (int) { m_temp_value--; }
uint32_t logic_t::operator+ (const logic_t logic_in)
{
    uint32_t res;
    res = m_get() + logic_in.m_value;
    return res;
}
logic_t logic_t::operator- (const logic_t logic_in)
{
    logic_t res(0);
    res.m_temp_value = m_get() - logic_in.m_value;
    return res;
}

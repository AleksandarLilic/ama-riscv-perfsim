#include "../include/logic_t.h"

// Constructors
logic_t::logic_t() : m_value(0), m_temp_value(0), m_rst_value(0), m_enable(1), m_name("no-name") {};
logic_t::logic_t(uint32_t init) : m_value(init), m_temp_value(init), m_rst_value(init), m_enable(1), m_name("no-name") {};
logic_t::logic_t(std::string initn) : m_value(0), m_temp_value(0), m_rst_value(0), m_enable(1), m_name(initn) {};
logic_t::logic_t(uint32_t init, std::string init_n) : m_value(init), m_temp_value(init), m_rst_value(init), m_enable(1), m_name(init_n) {};

// Methods
uint32_t logic_t::m_get() { return m_value; }
void logic_t::m_rst() { m_temp_value = m_rst_value; }  // sync rst, needs active edge to take rst val
void logic_t::m_set_enable(bool enable_in) { m_enable = enable_in; }
void logic_t::m_set_name(std::string init_n) { m_name = init_n; }
void logic_t::m_clk_update()
{
    uint32_t prev = m_value;
    if(m_enable) m_value = m_temp_value;    // only update if ff is enabled
    LOG("Called clk_update() on: '" << m_name << "'; Input: " << m_temp_value << "; Enable: " << m_enable << "; Old value: " << prev << "; New Value: " << m_value << "; ");
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

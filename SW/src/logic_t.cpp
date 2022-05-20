#include "../include/logic_t.h"

// Constructors
logic_t::logic_t() : m_logic_reg(0), m_logic_in(0), m_rst_value(0), m_enable(1), m_name("no-name") {};
logic_t::logic_t(uint32_t init) : m_logic_reg(init), m_logic_in(init), m_rst_value(init), m_enable(1), m_name("no-name") {};
logic_t::logic_t(std::string initn) : m_logic_reg(0), m_logic_in(0), m_rst_value(0), m_enable(1), m_name(initn) {};
logic_t::logic_t(uint32_t init, std::string init_n) : m_logic_reg(init), m_logic_in(init), m_rst_value(init), m_enable(1), m_name(init_n) {};

// Methods
uint32_t logic_t::m_get() { return m_logic_reg; }
void logic_t::m_rst() { m_logic_in = m_rst_value; }  // sync rst, needs active edge to take rst val
void logic_t::m_set_enable(bool enable_in) { m_enable = enable_in; }
void logic_t::m_set_name(std::string init_n) { m_name = init_n; }
void logic_t::m_clk_update()
{
    uint32_t prev = m_logic_reg;
    if(m_enable) m_logic_reg = m_logic_in;    // only update if ff is enabled
    LOG("Called clk_update() on: '" << m_name << "'; Input: " << m_logic_in << "; Enable: " << m_enable << "; Old value: " << prev << "; New Value: " << m_logic_reg << "; ");
}

// Operator overloads
void logic_t::operator= (const uint32_t val_in) { m_logic_in = val_in; }
uint32_t logic_t::operator+ (const uint32_t val_in) { return (m_get() + val_in); }
uint32_t logic_t::operator- (const uint32_t val_in) { return (m_get() - val_in); }
void logic_t::operator+= (const uint32_t val_in) { m_logic_in = m_get() + val_in; }
void logic_t::operator-= (const uint32_t val_in) { m_logic_in = m_get() - val_in; }
void logic_t::operator++ (int) { m_logic_in++; }
void logic_t::operator-- (int) { m_logic_in--; }
uint32_t logic_t::operator+ (const logic_t logic_in)
{
    uint32_t res;
    res = m_get() + logic_in.m_logic_reg;
    return res;
}
logic_t logic_t::operator- (const logic_t logic_in)
{
    logic_t res(0);
    res.m_logic_in = m_get() - logic_in.m_logic_reg;
    return res;
}

#include "../include/intf_cfg.h"

void intf_cfg::init()
{
    uint32_t i = 0u;
    logic_t *reg;
    for (i = 0u; i < 2; i++)
    {
        reg = new logic_t(qp, logic_init_cfg[i].init, logic_init_cfg[i].id);
        reg->connect_rst(logic_init_cfg[i].reset);
        reg->connect_en(logic_init_cfg[i].enable);
        reg->connect_in(logic_init_cfg[i].input);
        reg->connect_out(logic_init_cfg[i].output);
        LOG("Logic created; ID: " << logic_init_cfg[i].id);
    }
}

intf_cfg::intf_cfg()
{
    init();
}
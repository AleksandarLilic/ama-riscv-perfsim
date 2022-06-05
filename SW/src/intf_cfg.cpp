#include "../include/intf_cfg.h"

void intf_cfg::init()
{
    uint32_t i = 0u;
    logic_t *reg;
    for (i = 0u; i < 2; i++)
    {
        reg = new logic_t(qp, logic_init_cfg[i].init, logic_init_cfg[i].id);
        reg->connect(logic_init_cfg[i].output);
        reg->connect_in(logic_init_cfg[i].input);
    }

}

intf_cfg::intf_cfg()
{
    init();
}
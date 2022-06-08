#include "../include/intf_cfg.h"

void intf_cfg::init(seq_queue *q, logic_init_cfg_t *logic_init_cfg, uint32_t loop)
{
    uint32_t i = 0u;
    logic_t *reg;
    for (i = 0u; i < loop; i++)
    {
        reg = new logic_t(q, logic_init_cfg[i].init, logic_init_cfg[i].id);
        reg->connect_rst(logic_init_cfg[i].reset);
        reg->connect_en(logic_init_cfg[i].enable);
        reg->connect_in(logic_init_cfg[i].input);
        reg->connect_out(logic_init_cfg[i].output);
        LOG("Logic created; ID: " << logic_init_cfg[i].id);
        LOG("connected input addr: " << logic_init_cfg[i].input);
    }
}

void intf_cfg::init_if_id(seq_queue *q, sys_intf_t *sys_intf, if_intf_t *if_intf, id_intf_t *id_intf, uint32_t *imem_dout)
{
    logic_init_cfg_t if_id[IF_ID_SIZE] = {
        {"pc", 0, &sys_intf->rst, &if_intf->pc_we_if, &if_intf->pc, &id_intf->nx_pc},
        {"imem", 0, &in_false, &in_true, imem_dout, &id_intf->inst_id}
    };
    init(q, if_id, IF_ID_SIZE);
}


// void intf_cfg::intf_if_id(seq_queue *q, sys_intf_t *sys_intf, if_intf_t *if_intf, id_intf_t *id_intf)
// {
//     logic_init_cfg_t if_id[2] = {
//         {"branch_inst_id_ex", 0, &sys_intf->rst, &en_true, &ctrl_intf->dec_branch_inst_id, &ctrl_intf.dec_branch_inst_ex},
//         {"jump_inst_id_ex", 0, &sys_intf->rst, &en_true, &ctrl_intf.dec_jump_inst_id, &ctrl_intf.dec_jump_inst_ex}
//     };
//     init(q, if_id);
// }
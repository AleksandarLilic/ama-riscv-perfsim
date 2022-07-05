#pragma once

#include "defines.h"
#include "intf.h"

#include "decoder.h"
#include "op_fwd.h"

extern uint32_t global_inst_to_ctrl;

class control
{
private:
    decoder decoder;
    op_fwd op_fwd;

private:
    sys_intf_t *sys_intf;
    if_intf_t *if_intf;
    id_intf_t *id_intf;
    ex_intf_t *ex_intf;
    mem_intf_t *mem_intf;
    wb_intf_t *wb_intf;

public:
    control() = delete;
    control(sys_intf_t *sys_intf, if_intf_t *if_intf, id_intf_t *id_intf,
        ex_intf_t *ex_intf, mem_intf_t *mem_intf, wb_intf_t *wb_intf);
    void update();

private:
    void update(sys_intf_t *sys_intf, id_intf_t *id_intf, ex_intf_t *ex_intf,
        mem_intf_t *mem_intf);
    void pipeline_ctrl(sys_intf_t *sys_intf, id_intf_t *id_intf);
    void store_mask(id_intf_t *id_intf, ex_intf_t *ex_intf);
    void branch_resolution(sys_intf_t *sys_intf, id_intf_t *id_intf, ex_intf_t *ex_intf);
};

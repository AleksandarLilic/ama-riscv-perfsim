#pragma once

#include "defines.h"
#include "inst_field.h"
#include "intf.h"
#include "seq_queue.h"
#include "logic_t.h"
#include "cl.h"
#include <array>

#include "decoder.h"
#include "op_fwd.h"

class control
{
private:
    // modules
    decoder decoder;
    op_fwd op_fwd;
    // interfaces
    sys_intf_t *sys_intf;
    if_intf_t *if_intf;
    id_intf_t *id_intf;
    ex_intf_t *ex_intf;
    mem_intf_t *mem_intf;

public:
    control(sys_intf_t *sys_intf, if_intf_t *if_intf, id_intf_t *id_intf,
        ex_intf_t *ex_intf, mem_intf_t *mem_intf);
    void update();
private:
    void update(sys_intf_t *sys_intf, id_intf_t *id_intf, ex_intf_t *ex_intf,
        mem_intf_t *mem_intf);
    void pipeline_ctrl(sys_intf_t *sys_intf, id_intf_t *id_intf);
    void store_mask(id_intf_t *id_intf, ex_intf_t *ex_intf);
    void branch_resolution(sys_intf_t *sys_intf, id_intf_t *id_intf, ex_intf_t *ex_intf);
};

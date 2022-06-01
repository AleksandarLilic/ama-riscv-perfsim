#pragma once

#include "defines.h"
#include "inst_field.h"
#include "intf.h"
#include "seq_queue.h"
#include "logic_t.h"
#include "cl.h"
#include <array>

class decoder
{
public:
    void update(ctrl_intf_t *ctrl_intf, sys_intf_t *sys_intf);

private:
    void r_type(ctrl_intf_t *ctrl_intf);
    void i_type(ctrl_intf_t *ctrl_intf);
    void load(ctrl_intf_t *ctrl_intf);
    void store(ctrl_intf_t *ctrl_intf);
    void branch(ctrl_intf_t *ctrl_intf);
    void jalr(ctrl_intf_t *ctrl_intf);
    void jal(ctrl_intf_t *ctrl_intf);
    void lui(ctrl_intf_t *ctrl_intf);
    void auipc(ctrl_intf_t *ctrl_intf);
    void system(ctrl_intf_t *ctrl_intf);
    void unsupported(ctrl_intf_t *ctrl_intf);
    void reset(ctrl_intf_t *ctrl_intf);
};

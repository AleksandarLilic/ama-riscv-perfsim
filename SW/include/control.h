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
    decoder decoder;
    op_fwd op_fwd;

public:
    void update(ctrl_intf_t *ctrl_intf, sys_intf_t *sys_intf);

private:
    void pipeline_ctrl(ctrl_intf_t *ctrl_intf, sys_intf_t *sys_intf);
    void store_mask(ctrl_intf_t *ctrl_intf);
    void branch_resolution(ctrl_intf_t *ctrl_intf, sys_intf_t *sys_intf);
};

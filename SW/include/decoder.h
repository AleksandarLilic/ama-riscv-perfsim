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
    void update(sys_intf_t *sys_intf, id_intf_t *id_intf);

private:
    void r_type(id_intf_t *id_intf);
    void i_type(id_intf_t *id_intf);
    void load(id_intf_t *id_intf);
    void store(id_intf_t *id_intf);
    void branch(id_intf_t *id_intf);
    void jalr(id_intf_t *id_intf);
    void jal(id_intf_t *id_intf);
    void lui(id_intf_t *id_intf);
    void auipc(id_intf_t *id_intf);
    void system(id_intf_t *id_intf);
    void unsupported(id_intf_t *id_intf);
    void reset(id_intf_t *id_intf);

    uint32_t store_mask(id_intf_t *id_intf);
};


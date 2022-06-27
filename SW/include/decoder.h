#pragma once

#include "defines.h"
#include "intf.h"
#include "inst_field.h"

class decoder
{
private:
    sys_intf_t *sys_intf;
    id_intf_t *id_intf;

public:
    void update();
    decoder() = delete;
    decoder(sys_intf_t *sys_intf, id_intf_t *id_intf);

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


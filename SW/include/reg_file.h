#pragma once

#include "defines.h"
#include "intf.h"

#define NO_REG_UPDATE 32

class reg_file
{
private:
    uint32_t updated_register = NO_REG_UPDATE;
private:
    id_intf_t *id_intf;
    mem_intf_t *mem_intf;
    wb_intf_t *wb_intf;
    reg_file_intf_t *reg_file_intf;

public:
    reg_file() = delete;
    reg_file(reg_file_intf_t *reg_file_intf, id_intf_t *id_intf, mem_intf_t *mem_intf_t, wb_intf_t *wb_intf);
    void write();
    void read();
    void status_log();
private:
    void status_updated_register();
};
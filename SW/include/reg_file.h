#pragma once

#include "defines.h"
#include "intf.h"

class reg_file
{
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
};
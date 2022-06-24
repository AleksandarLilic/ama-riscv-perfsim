#include "../include/reg_file.h"

reg_file::reg_file(reg_file_intf_t *reg_file_intf, id_intf_t *id_intf, mem_intf_t *mem_intf, wb_intf_t *wb_intf)
{
    this->reg_file_intf = reg_file_intf;
    this->id_intf = id_intf;
    this->mem_intf = mem_intf;
    this->wb_intf = wb_intf;
}

void reg_file::write()
{
    LOG("--- reg_file::write");
    if ((mem_intf->rd_we_mem) && (rf_t(mem_intf->rd_addr_mem) != rf_t::x0_zero)) {
        if (mem_intf->rd_addr_mem < 32)
            reg_file_intf->in[mem_intf->rd_addr_mem] = wb_intf->data_d;
        else
            LOGE("Invalid Reg File address");
    }

    LOG("    Reg File - addr d: " << mem_intf->rd_addr_mem << "; data d : " << wb_intf->data_d);
    status_log();
}

void reg_file::read()
{
    LOG("--- reg_file::read");

    if ((rf_t(id_intf->rs1_addr_id) == rf_t::x0_zero)) {
        id_intf->rf_data_a = 0;
    }
    else {
        if (mem_intf->rd_addr_mem < 32)
            id_intf->rf_data_a = reg_file_intf->out[id_intf->rs1_addr_id];
        else
            LOGE("Invalid Reg File address for A port");
    }

    if ((rf_t(id_intf->rs2_addr_id) == rf_t::x0_zero)) {
        id_intf->rf_data_b = 0;
    }
    else {
        if (mem_intf->rd_addr_mem < 32)
            id_intf->rf_data_b = reg_file_intf->out[id_intf->rs2_addr_id];
        else
            LOGE("Invalid Reg File address for B port");
    }

    LOG("    Reg File - addr a: " << id_intf->rs1_addr_id << "; addr b : " << id_intf->rs2_addr_id );
    LOG("    Reg File - data a: " << id_intf->rf_data_a << "; data b : " << id_intf->rf_data_b );
}

void reg_file::status_log()
{
    //LOG("Arch State - Register File");
    //for (uint32_t i = 0; i < 32; i++) {
    //    std::string item = "out_r1";
    //    LOG("    X" << i << ": " << reg_file_intf->item);
    //}
    LOG("reg file x8: " << reg_file_intf->out[8]);
    LOG("reg file x10: " << reg_file_intf->out[10]);
    LOG("reg file x11: " << reg_file_intf->out[11]);
    LOG("reg file x12: " << reg_file_intf->out[12]);
}
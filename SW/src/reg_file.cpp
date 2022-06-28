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
    // Default write, keep previous outputs
    for (uint32_t i = 1; i < reg_file_intf->out.size(); i++) {
        reg_file_intf->in[i] = reg_file_intf->out[i];
    }

    // Actual write, if asserted from control
    if ((mem_intf->rd_we_mem) && (rf_t(mem_intf->rd_addr_mem) != rf_t::x0_zero)) {
        if (mem_intf->rd_addr_mem < 32)
            reg_file_intf->in[mem_intf->rd_addr_mem] = wb_intf->data_d;
        else
            LOGE("Invalid Reg File address");

    LOG("    Reg File write - Addres D: " << mem_intf->rd_addr_mem << "; Data D : " << wb_intf->data_d);
    }
    status_updated_register();
}

void reg_file::read()
{
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
    LOG("    Reg File read - Addres A: " << id_intf->rs1_addr_id << "; Data A : " << id_intf->rf_data_a);
    LOG("    Reg File read - Addres B: " << id_intf->rs2_addr_id << "; Data B : " << id_intf->rf_data_b);
}

void reg_file::status_log()
{
    std::cout << std::endl;
    LOG("> Arch State - Register File");
    for (uint32_t i = 0; i < reg_file_intf->out.size(); i+=4) {
        for (uint32_t j = i; j < i + 4; j++) {
            if (j == updated_register) // mark changed register
                LOG_L(FRF_M(j, reg_file_intf->out[j]));
            else
                LOG_L(FRF(j, reg_file_intf->out[j]));
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    updated_register = NO_REG_UPDATE; // reset register address
}

void reg_file::status_updated_register()
{
    if ((mem_intf->rd_we_mem) && 
        (rf_t(mem_intf->rd_addr_mem) != rf_t::x0_zero) && 
        (mem_intf->rd_addr_mem < 32))
            updated_register = mem_intf->rd_addr_mem;
}
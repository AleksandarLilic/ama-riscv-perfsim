#include "../include/core.h"

void core::init(seq_queue *q)
{
    LOG("core init called");    
}

void core::mem_init(uint32_t *imem_ptr, uint32_t *dmem_ptr) 
{ 
    LOG("mem_init called");
    this->imem_ptr = imem_ptr;  
    this->dmem_ptr = dmem_ptr;
    LOG("imem_ptr = " << this->imem_ptr);
    LOG("dmem_ptr = " << this->dmem_ptr);
};

void core::reset(bool rst_in)
{
    sys_intf.rst = rst_in;
}

void core::update_fe()
{
    front_end(&if_intf, &id_intf);
}

void core::update()
{
    LOG("---------- inst in ID stage: " << std::hex << id_intf.inst_id << std::dec);
    control.update();
}

core::core(seq_queue *q, uint32_t *imem_ptr, uint32_t *dmem_ptr) :
    control(&sys_intf, &if_intf, &id_intf, &ex_intf, &mem_intf)
{
    mem_init(imem_ptr, dmem_ptr);
    intf_cfg.init_if_id(q, &sys_intf, &if_intf, &id_intf, imem_ptr);
    init(q);
    LOG("core queue constructor called");
}

void core::front_end(if_intf_t *if_intf, id_intf_t *id_intf)
{
    //LOG("Before update - PC: " << if_intf->pc << "; NX PC: " << id_intf->nx_pc);
    if_intf->imem_addr = id_intf->nx_pc;
    if_intf->pc = id_intf->nx_pc + 1;
    //LOG("After update - PC: " << if_intf->pc << "; NX PC: " << id_intf->nx_pc);
}

// Core ID stage


//void core::ex_stage()
//{
//   uint32_t mux_out = cl::mux4(ctrl_if_ex.test_logic_type.get(), seq_dp_id_ex_if.rf_read_op_a, fwd_mem, pc_ex, CL_UNUSED);
//   
//   alu_in_a = mux_out;
//   core::alu(alu_in_a, alu_in_b, alu_sel, &alu_out);
//
//   dp_ex_mem.mux_out_ex = mux_out;
//}
//
//uint32_t core::alu()
//{
//    uint32_t local_alu = 0;
//    switch(alu_sel)
//        case 0x00: *alu_out = alu_in_a + alu_in_b; break;
//        case 0x08: *alu_out = alu_in_a - alu_in_b; break;
//
//}

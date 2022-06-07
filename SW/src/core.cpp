#include "../include/core.h"

void core::init(seq_queue *q)
{
    LOG("core init called");    
}

void core::reset(bool rst_in)
{
    sys_intf.rst = rst_in;
}

void core::update(std::array<uint32_t, IMEM_SIZE> &imem_ptr, std::array<uint32_t, DMEM_SIZE> &dmem_ptr)
{
    ctrl_intf.in_inst_id = imem_ptr[pc_mock];
    LOG("---------- inst in decode stage: " << std::hex << ctrl_intf.in_inst_id << std::dec);
    control.update(&ctrl_intf, &sys_intf);
    if (!sys_intf.rst)
        pc_mock++;
}


core::core(seq_queue *q) /*:
    seq_id_ex_intf(q)*/
{
    intf_cfg.init_if_id(q, &sys_intf, &if_intf, &id_intf);
    init(q);
    LOG("core queue constructor called");
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

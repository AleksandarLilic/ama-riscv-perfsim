#include "../include/core.h"
#include "../include/cl.h"

void core::reset(bool rst_in)
{
    sys_intf.rst = rst_in;
    if (sys_intf.rst)
        queue.reset();
}

core::core()
{
    uint32_t value;
    ctrl_intf_id.alu_a_sel_id.init(0, "alu_a_sel_id");

}

void core::decode(ctrl_intf_t *ctrl_intf_id)
{
    uint32_t opc7_id = inst_field::opc7(ctrl_intf_id->inst_id);

    ctrl_intf_id->alu_a_sel_id = 3;

}

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
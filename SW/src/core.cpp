#include "../include/core.h"
#include "../include/cl.h"

void core::m_reset(bool rst_in)
{
    sys_intfc.rst = rst_in;
    if (sys_intfc.rst)
        queue.reset();
}

core::core()
{
    ctrl_intfc_id.test_logic_type.init("alu_a_sel_id");
//    LOG("waiting for dummy string:" << ctrl_if.test_logic_type.m_name);
    //static logic_t alu_ex(0, "alu_ex");

}

void core::m_decode()
{
    uint32_t opc7_id = inst_field::opc7(ctrl_intfc_id.inst_id);

    ctrl_intfc_id.test_logic_type = 3;

}

void core::ex_stage()
{
   uint32_t mux_out = cl::mux4(ctrl_if_ex.test_logic_type.m_get(), seq_dp_id_ex_if.rf_read_op_a, fwd_mem, pc_ex, CL_UNUSED);
   
   alu_in_a = mux_out;
   core::m_alu(alu_in_a, alu_in_b, alu_sel, &alu_out);

   dp_ex_mem.mux_out_ex = mux_out;
}

uint32_t core::m_alu()
{
    uint32_t local_alu = 0;
    switch(alu_sel)
        case 0x00: *alu_out = alu_in_a + alu_in_b; break;
        case 0x08: *alu_out = alu_in_a - alu_in_b; break;

}
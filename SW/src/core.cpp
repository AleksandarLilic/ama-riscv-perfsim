#include "../include/core.h"

core::core(seq_queue *q, uint32_t *imem_ptr, uint32_t *dmem_ptr) :
    control(&sys_intf, &if_intf, &id_intf, &ex_intf, &mem_intf, &wb_intf),
    reg_file(&reg_file_intf, &id_intf, &mem_intf, &wb_intf),
    imm_gen(&id_intf),
    branch_compare(&ex_intf),
    alu(&ex_intf),
    store_shift(&ex_intf),
    load_shift_mask(&mem_intf)
{
    this->imem_ptr = imem_ptr;
    this->dmem_ptr = dmem_ptr;
    intf_cfg.init_regs(q, &sys_intf, &reg_file_intf, &if_intf, &id_intf, &ex_intf, &mem_intf, imem_ptr);
}

void core::reset(bool rst_in)
{
    sys_intf.rst = rst_in;
}

void core::update_system()
{
    if (sys_intf.rst)
        sys_intf.rst_seq = 0b0111;
    else
        sys_intf.rst_seq = sys_intf.rst_seq >> 1;

    sys_intf.rst_seq_d1 = (sys_intf.rst_seq & 0b100) >> 2;
    sys_intf.rst_seq_d2 = (sys_intf.rst_seq & 0b10) >> 1;
    sys_intf.rst_seq_d3 = sys_intf.rst_seq & 0b1;

#if LOG_DBG
    LOG("sys_intf.rst_seq: " << sys_intf.rst_seq);
    LOG("sys_intf.rst_seq_d1: " << sys_intf.rst_seq_d1);
    LOG("sys_intf.rst_seq_d2: " << sys_intf.rst_seq_d2);
    LOG("sys_intf.rst_seq_d3: " << sys_intf.rst_seq_d3);
#endif
}

void core::update_if()
{
    const uint32_t ALU_OUT_MOCK = 12;
    LOG(">> UPDATE_IF");
    LOG("Current PC: " << if_intf.pc << "; NX PC: " << id_intf.nx_pc);

    if_intf.imem_addr = id_intf.nx_pc;
    //if_intf.imem_addr = cl::mux2(uint32_t(id_intf.dec_pc_sel_if), id_intf.nx_pc, ALU_OUT_MOCK);
    if_intf.pc = id_intf.nx_pc + 1;
    //LOG("After update - PC: " << if_intf.pc << "; NX PC: " << id_intf.nx_pc);
}

void core::update_id()
{
    LOG(">> UPDATE_ID");
    LOG("--- inst_parsing");
    id_intf.opc7_id = inst_field::opc7(id_intf.inst_id);
    id_intf.funct3_id = inst_field::funct3(id_intf.inst_id);
    id_intf.funct7_id = inst_field::funct7(id_intf.inst_id);
    id_intf.rs1_addr_id = inst_field::rs1_addr(id_intf.inst_id);
    id_intf.rs2_addr_id = inst_field::rs2_addr(id_intf.inst_id);
    id_intf.rd_addr_id = inst_field::rd_addr(id_intf.inst_id);

    control.update();
    reg_file.read();
    imm_gen.update();
    LOG("---------- inst in ID stage: " << std::hex << id_intf.inst_id << std::dec);
}

void core::update_ex()
{
    LOG(">> UPDATE_EX");
    branch_compare.update();
    alu.update();
    store_shift.update();
//   uint32_t mux_out = cl::mux4(ctrl_if_ex.test_logic_type.get(), seq_dp_id_ex_if.rf_read_op_a, fwd_mem, pc_ex, CL_UNUSED);
//   
//   alu_in_a = mux_out;
//   core::alu(alu_in_a, alu_in_b, alu_sel, &alu_out);
//
//   dp_ex_mem.mux_out_ex = mux_out;
}

void core::update_mem()
{
    LOG(">> UPDATE_MEM");
    load_shift_mask.update();
}

void core::update_wb()
{
    LOG(">> UPDATE_WB");
    uint32_t pc_mem_inc4 = mem_intf.alu_mem + 1;
    uint32_t csr_placeholder = 0;
    wb_intf.data_d = cl::mux4(mem_intf.wb_sel_mem, 
        wb_intf.load_sm_out, 
        mem_intf.alu_mem, 
        pc_mem_inc4, 
        csr_placeholder);
    reg_file.write();
}

void core::status_log()
{
    reg_file.status_log();
}
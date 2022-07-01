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
    LOG("> UPDATE_SYSTEM");
    if (sys_intf.rst)
        sys_intf.rst_seq = 0b0111;
    else
        sys_intf.rst_seq = sys_intf.rst_seq >> 1;

    sys_intf.rst_seq_d1 = (sys_intf.rst_seq & 0b100) >> 2;
    sys_intf.rst_seq_d2 = (sys_intf.rst_seq & 0b10) >> 1;
    sys_intf.rst_seq_d3 = sys_intf.rst_seq & 0b1;

#if LOG_DBG
    LOG("    RST Seq: " << sys_intf.rst_seq << ", bin:" << FBIN(sys_intf.rst_seq, 3));
#endif
}

void core::update_if()
{
    LOG("> UPDATE_IF");

    //if_intf.imem_addr = cl::mux2(uint32_t(id_intf.dec_pc_sel_if), id_intf.nx_pc, ex_intf.alu_out);
    if_intf.imem_addr = cl::mux2(0u, id_intf.nx_pc, ex_intf.alu_out);
    if_intf.pc_inc4 = if_intf.imem_addr + 1;

    LOG("    Current PC: " << id_intf.nx_pc << 
        "; Current IMEM Addr: " << if_intf.imem_addr << 
        "; NX PC: " << if_intf.pc_inc4);
}

void core::update_id()
{
    LOG("> UPDATE_ID");
    LOG("    Instruction in ID stage: " << FHEX(id_intf.inst_id));
    id_intf.opc7_id = inst_field::opc7(id_intf.inst_id);
    id_intf.funct3_id = inst_field::funct3(id_intf.inst_id);
    id_intf.funct7_id = inst_field::funct7(id_intf.inst_id);
    id_intf.rs1_addr_id = inst_field::rs1_addr(id_intf.inst_id);
    id_intf.rs2_addr_id = inst_field::rs2_addr(id_intf.inst_id);
    id_intf.rd_addr_id = inst_field::rd_addr(id_intf.inst_id);

    control.update();
    reg_file.read();
    imm_gen.update();
    
    id_intf.rf_data_a_fwd = cl::mux2(id_intf.of_rf_a_sel_fwd_id, id_intf.rf_data_a, wb_intf.data_d);
    id_intf.rf_data_b_fwd = cl::mux2(id_intf.of_rf_b_sel_fwd_id, id_intf.rf_data_b, wb_intf.data_d);
}

void core::update_ex()
{
    LOG("> UPDATE_EX");
    ex_intf.bc_in_a = cl::mux2(ex_intf.bc_a_sel_ex, ex_intf.rf_data_a_ex, wb_intf.data_d);
    ex_intf.bcs_in_b = cl::mux2(ex_intf.bcs_b_sel_ex, ex_intf.rf_data_b_ex, wb_intf.data_d);

    ex_intf.alu_in_a = cl::mux4(ex_intf.alu_a_sel_ex,
        ex_intf.rf_data_a_ex,
        ex_intf.pc_ex,
        wb_intf.data_d,
        CL_UNUSED);
    ex_intf.alu_in_b = cl::mux4(ex_intf.alu_b_sel_ex, 
        ex_intf.rf_data_b_ex, 
        ex_intf.imm_gen_out_ex,
        wb_intf.data_d,
        CL_UNUSED);

#if LOG_DBG
    LOG("    BC A sel: " << ex_intf.bc_a_sel_ex);
    LOG("    BC in A: " << ex_intf.bc_in_a);
    LOG("    BCS B sel: " << ex_intf.bcs_b_sel_ex);
    LOG("    BCS in B: " << ex_intf.bcs_in_b);
    LOG("    ALU A sel: " << ex_intf.alu_a_sel_ex);
    LOG("    ALU in A: " << ex_intf.alu_in_a);
    LOG("    ALU B sel: " << ex_intf.alu_b_sel_ex);
    LOG("    ALU in B: " << ex_intf.alu_in_b);
#endif

    branch_compare.update();
    alu.update();
    store_shift.update();
}

void core::update_mem()
{
    LOG("> UPDATE_MEM");
    mem_intf.dmem_out++;
    if(mem_intf.load_sm_en_mem)
        load_shift_mask.update();
    LOG("    Load SM output: " << mem_intf.load_sm_out);
    uint32_t pc_mem_inc4 = mem_intf.alu_mem + 1;
    uint32_t csr_placeholder = 0;

    wb_intf.data_d = cl::mux4(mem_intf.wb_sel_mem, 
        mem_intf.load_sm_out, 
        mem_intf.alu_mem, 
        pc_mem_inc4, 
        csr_placeholder);

    LOG("    WB mux select: " << mem_intf.wb_sel_mem);
    LOG("    WB mux output: " << wb_intf.data_d);

    reg_file.write();
}

void core::update_wb()
{
    LOG("> UPDATE_WB");
}

void core::status_log()
{
    reg_file.status_log();
}

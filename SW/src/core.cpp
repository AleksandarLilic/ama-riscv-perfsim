#include "../include/core.h"

//core::core(seq_queue *q, uint32_t *imem_ptr, uint32_t *dmem_ptr) :
core::core(seq_queue *q, core_intf_t *core_intf):
    control(&sys_intf, &if_intf, &id_intf, &ex_intf, &mem_intf, &wb_intf),
    reg_file(&reg_file_intf, &id_intf, &mem_intf, &wb_intf),
    imm_gen(&id_intf),
    branch_compare(&ex_intf),
    alu(&ex_intf),
    store_shift(&ex_intf),
    load_shift_mask(&mem_intf),
    csr_file(&csr_file_intf, &id_intf, &mem_intf, &wb_intf)
{
    core_intf->imem_addr = &if_intf.imem_addr;
    imem_dout_ptr = &core_intf->imem_dout;
    core_intf->dmem_addr = &ex_intf.dmem_addr;
    core_intf->dmem_din = &ex_intf.dmem_din;
    core_intf->dmem_en = &ex_intf.dmem_en_ex;
    core_intf->dmem_we = &id_intf.dec_store_mask_ex;
    dmem_dout_ptr = &core_intf->dmem_dout;

    intf_cfg.init_regs(q, &sys_intf, &reg_file_intf, &if_intf, &id_intf, &ex_intf, &mem_intf, &wb_intf,
        imem_dout_ptr, dmem_dout_ptr, &csr_file_intf);

    global_pc_id_ptr = &id_intf.pc;
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

    sys_intf.rst_seq_id_ex = (sys_intf.rst_seq_d & 0b100) >> 2;
    sys_intf.rst_seq_ex_mem = (sys_intf.rst_seq_d & 0b10) >> 1;
    sys_intf.rst_seq_mem_wb = sys_intf.rst_seq_d & 0b1;

#if LOG_DBG
    LOG("    RST Seq: " << sys_intf.rst_seq_d << ", bin:" << FBIN(sys_intf.rst_seq_d, 3));
#endif
}

void core::update_if()
{
    LOG("> UPDATE_IF");

    if_intf.pc_prepared = cl::mux2(sys_intf.rst_seq_id_ex, id_intf.pc + 4, id_intf.pc);
    if_intf.imem_addr = cl::mux2(uint32_t(id_intf.dec_pc_sel_if), if_intf.pc_prepared, ex_intf.alu_out);

    LOG("    PC write enable: " << id_intf.dec_pc_we_if);
    LOG("    PC sel: " << id_intf.dec_pc_sel_if);
    LOG("    IF/ID stall: " << id_intf.stall_if_id);

    LOG("    Current PC: " << if_intf.pc_prepared <<
        ", (" << (if_intf.pc_prepared >> 2) << "), " << FHEXI(if_intf.pc_prepared) <<
        "; Current IMEM Addr (mux out): " << if_intf.imem_addr << 
        ", (" << (if_intf.imem_addr >> 2) << "), " << FHEXI(if_intf.imem_addr));
}

void core::update_id()
{
    LOG("> UPDATE_ID");
    LOG_M("    Instruction in ID stage: " << FHEXI(id_intf.inst_id));
    LOG("    PC ID: " << FHEXI(id_intf.pc));

    id_intf.stall_if_id = sys_intf.rst;
    if (id_intf.stall_if_id_d)     // Convert to NOP on stall
        id_intf.inst_id = NOP;
    LOG("    Instruction going to control: " << FHEXI(id_intf.inst_id));

    id_intf.opc7_id = inst_field::opc7(id_intf.inst_id);
    id_intf.funct3_id = inst_field::funct3(id_intf.inst_id);
    id_intf.funct7_id = inst_field::funct7(id_intf.inst_id);
    id_intf.rs1_addr_id = inst_field::rs1_addr(id_intf.inst_id);
    id_intf.rs2_addr_id = inst_field::rs2_addr(id_intf.inst_id);
    id_intf.rd_addr_id = inst_field::rd_addr(id_intf.inst_id);
    id_intf.csr_uimm = inst_field::uimm_zero_ext(id_intf.inst_id);

    control.update();
    reg_file.read();
    csr_file.read();
    imm_gen.update();
    
    id_intf.rf_data_a_fwd = cl::mux2(id_intf.of_rf_a_sel_fwd_id, id_intf.rf_data_a, wb_intf.data_d);
    id_intf.rf_data_b_fwd = cl::mux2(id_intf.of_rf_b_sel_fwd_id, id_intf.rf_data_b, wb_intf.data_d);
}

void core::update_ex()
{
    LOG("> UPDATE_EX");
    LOG_M("    Instruction in EX stage: " << FHEXI(ex_intf.inst_ex));
    LOG("    PC EX: " << FHEX(ex_intf.pc_ex));
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
    ex_intf.dmem_addr = ex_intf.alu_out >> 2;
    store_shift.update();

#if LOG_DBG
    LOG("    DMEM en: " << ex_intf.dmem_en_ex);
    LOG("    DMEM we: " << id_intf.dec_store_mask_ex);
    LOG("    DMEM addr: " << ex_intf.dmem_addr);
    LOG("    DMEM din: " << ex_intf.dmem_din);
#endif
}

void core::update_mem()
{
    LOG("> UPDATE_MEM");
    LOG_M("    Instruction in MEM stage: " << FHEXI(mem_intf.inst_mem));
    LOG("    PC MEM: " << FHEX(mem_intf.pc_mem));
    LOG("    DMEM out: " << mem_intf.dmem_dout << ", " << FHEX(mem_intf.dmem_dout));
    if(mem_intf.load_sm_en_mem)
        load_shift_mask.update();
    LOG("    Load SM output: " << mem_intf.load_sm_out << ", " << FHEX(mem_intf.load_sm_out));
    uint32_t pc_mem_inc4 = mem_intf.pc_mem + 4;

    wb_intf.data_d = cl::mux4(mem_intf.wb_sel_mem, 
        mem_intf.load_sm_out, 
        mem_intf.alu_mem, 
        pc_mem_inc4, 
        mem_intf.csr_data_mem);

    LOG("    WB mux select: " << mem_intf.wb_sel_mem);
    LOG("    WB mux output: " << wb_intf.data_d << ", " << FHEX(wb_intf.data_d));

    reg_file.write();
    csr_file.write();
}

void core::update_wb()
{
    LOG_M("> UPDATE_WB");
    LOG_M("    Instruction in WB stage: " << FHEXI(wb_intf.inst_wb));
    //global_wb_inst = wb_intf.inst_wb;
#if RISCV_SANITY_TESTS
    if(wb_intf.inst_wb != 0x13 && (!sys_intf.rst)) // if instruction is not NOP, record as committed
        global_committed_instructions.push_back(wb_intf.inst_wb);
#endif
    perf_cpu::update(&sys_intf, &if_intf, &id_intf, &wb_intf);
}

void core::status_log()
{
    reg_file.status_log();
    csr_file.status_log();

#if RISCV_SANITY_TESTS
    if(!sys_intf.rst)
        reg_file.check_reg_25();
#endif
}

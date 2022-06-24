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
    if (mem_intf->rd_we_mem) {
        switch (rf_t(mem_intf->rd_addr_mem)) {
        case rf_t::x0_zero: break;
        case rf_t::x1_ra: reg_file_intf->in_r1 = wb_intf->data_d; break;
        case rf_t::x2_sp: reg_file_intf->in_r2 = wb_intf->data_d; break;
        case rf_t::x3_gp: reg_file_intf->in_r3 = wb_intf->data_d; break;
        case rf_t::x4_tp: reg_file_intf->in_r4 = wb_intf->data_d; break;
        case rf_t::x5_t0: reg_file_intf->in_r5 = wb_intf->data_d; break;
        case rf_t::x6_t1: reg_file_intf->in_r6 = wb_intf->data_d; break;
        case rf_t::x7_t2: reg_file_intf->in_r7 = wb_intf->data_d; break;
        case rf_t::x8_s0: reg_file_intf->in_r8 = wb_intf->data_d; break;
        case rf_t::x9_s1: reg_file_intf->in_r9 = wb_intf->data_d; break;
        case rf_t::x10_a0: reg_file_intf->in_r10 = wb_intf->data_d; break;
        case rf_t::x11_a1: reg_file_intf->in_r11 = wb_intf->data_d; break;
        case rf_t::x12_a2: reg_file_intf->in_r12 = wb_intf->data_d; break;
        case rf_t::x13_a3: reg_file_intf->in_r13 = wb_intf->data_d; break;
        case rf_t::x14_a4: reg_file_intf->in_r14 = wb_intf->data_d; break;
        case rf_t::x15_a5: reg_file_intf->in_r15 = wb_intf->data_d; break;
        case rf_t::x16_a6: reg_file_intf->in_r16 = wb_intf->data_d; break;
        case rf_t::x17_a7: reg_file_intf->in_r17 = wb_intf->data_d; break;
        case rf_t::x18_s2: reg_file_intf->in_r18 = wb_intf->data_d; break;
        case rf_t::x19_s3: reg_file_intf->in_r19 = wb_intf->data_d; break;
        case rf_t::x20_s4: reg_file_intf->in_r20 = wb_intf->data_d; break;
        case rf_t::x21_s5: reg_file_intf->in_r21 = wb_intf->data_d; break;
        case rf_t::x22_s6: reg_file_intf->in_r22 = wb_intf->data_d; break;
        case rf_t::x23_s7: reg_file_intf->in_r23 = wb_intf->data_d; break;
        case rf_t::x24_s8: reg_file_intf->in_r24 = wb_intf->data_d; break;
        case rf_t::x25_s9: reg_file_intf->in_r25 = wb_intf->data_d; break;
        case rf_t::x26_s10: reg_file_intf->in_r26 = wb_intf->data_d; break;
        case rf_t::x27_s11: reg_file_intf->in_r27 = wb_intf->data_d; break;
        case rf_t::x28_t3: reg_file_intf->in_r28 = wb_intf->data_d; break;
        case rf_t::x29_t4: reg_file_intf->in_r29 = wb_intf->data_d; break;
        case rf_t::x30_t5: reg_file_intf->in_r30 = wb_intf->data_d; break;
        case rf_t::x31_t6: reg_file_intf->in_r31 = wb_intf->data_d; break;
        default: LOGE("Invalid Reg File address");
        }
    }
    LOG("    Reg File - addr d: " << mem_intf->rd_addr_mem << "; data d : " << wb_intf->data_d);
    status_log();
}

void reg_file::read()
{
    LOG("--- reg_file::read");

    switch (rf_t(id_intf->rs1_addr_id)) {
    case rf_t::x0_zero: id_intf->rf_data_a = 0u; break;
    case rf_t::x1_ra: id_intf->rf_data_a = reg_file_intf->out_r1; break;
    case rf_t::x2_sp: id_intf->rf_data_a = reg_file_intf->out_r2; break;
    case rf_t::x3_gp: id_intf->rf_data_a = reg_file_intf->out_r3; break;
    case rf_t::x4_tp: id_intf->rf_data_a = reg_file_intf->out_r4; break;
    case rf_t::x5_t0: id_intf->rf_data_a = reg_file_intf->out_r5; break;
    case rf_t::x6_t1: id_intf->rf_data_a = reg_file_intf->out_r6; break;
    case rf_t::x7_t2: id_intf->rf_data_a = reg_file_intf->out_r7; break;
    case rf_t::x8_s0: id_intf->rf_data_a = reg_file_intf->out_r8; break;
    case rf_t::x9_s1: id_intf->rf_data_a = reg_file_intf->out_r9; break;
    case rf_t::x10_a0: id_intf->rf_data_a = reg_file_intf->out_r10; break;
    case rf_t::x11_a1: id_intf->rf_data_a = reg_file_intf->out_r11; break;
    case rf_t::x12_a2: id_intf->rf_data_a = reg_file_intf->out_r12; break;
    case rf_t::x13_a3: id_intf->rf_data_a = reg_file_intf->out_r13; break;
    case rf_t::x14_a4: id_intf->rf_data_a = reg_file_intf->out_r14; break;
    case rf_t::x15_a5: id_intf->rf_data_a = reg_file_intf->out_r15; break;
    case rf_t::x16_a6: id_intf->rf_data_a = reg_file_intf->out_r16; break;
    case rf_t::x17_a7: id_intf->rf_data_a = reg_file_intf->out_r17; break;
    case rf_t::x18_s2: id_intf->rf_data_a = reg_file_intf->out_r18; break;
    case rf_t::x19_s3: id_intf->rf_data_a = reg_file_intf->out_r19; break;
    case rf_t::x20_s4: id_intf->rf_data_a = reg_file_intf->out_r20; break;
    case rf_t::x21_s5: id_intf->rf_data_a = reg_file_intf->out_r21; break;
    case rf_t::x22_s6: id_intf->rf_data_a = reg_file_intf->out_r22; break;
    case rf_t::x23_s7: id_intf->rf_data_a = reg_file_intf->out_r23; break;
    case rf_t::x24_s8: id_intf->rf_data_a = reg_file_intf->out_r24; break;
    case rf_t::x25_s9: id_intf->rf_data_a = reg_file_intf->out_r25; break;
    case rf_t::x26_s10: id_intf->rf_data_a = reg_file_intf->out_r26; break;
    case rf_t::x27_s11: id_intf->rf_data_a = reg_file_intf->out_r27; break;
    case rf_t::x28_t3: id_intf->rf_data_a = reg_file_intf->out_r28; break;
    case rf_t::x29_t4: id_intf->rf_data_a = reg_file_intf->out_r29; break;
    case rf_t::x30_t5: id_intf->rf_data_a = reg_file_intf->out_r30; break;
    case rf_t::x31_t6: id_intf->rf_data_a = reg_file_intf->out_r31; break;
    default: LOGE("Invalid Reg File address for A port");
    }

    switch (rf_t(id_intf->rs2_addr_id)) {
    case rf_t::x0_zero: id_intf->rf_data_b = 0u; break;
    case rf_t::x1_ra: id_intf->rf_data_b = reg_file_intf->out_r1; break;
    case rf_t::x2_sp: id_intf->rf_data_b = reg_file_intf->out_r2; break;
    case rf_t::x3_gp: id_intf->rf_data_b = reg_file_intf->out_r3; break;
    case rf_t::x4_tp: id_intf->rf_data_b = reg_file_intf->out_r4; break;
    case rf_t::x5_t0: id_intf->rf_data_b = reg_file_intf->out_r5; break;
    case rf_t::x6_t1: id_intf->rf_data_b = reg_file_intf->out_r6; break;
    case rf_t::x7_t2: id_intf->rf_data_b = reg_file_intf->out_r7; break;
    case rf_t::x8_s0: id_intf->rf_data_b = reg_file_intf->out_r8; break;
    case rf_t::x9_s1: id_intf->rf_data_b = reg_file_intf->out_r9; break;
    case rf_t::x10_a0: id_intf->rf_data_b = reg_file_intf->out_r10; break;
    case rf_t::x11_a1: id_intf->rf_data_b = reg_file_intf->out_r11; break;
    case rf_t::x12_a2: id_intf->rf_data_b = reg_file_intf->out_r12; break;
    case rf_t::x13_a3: id_intf->rf_data_b = reg_file_intf->out_r13; break;
    case rf_t::x14_a4: id_intf->rf_data_b = reg_file_intf->out_r14; break;
    case rf_t::x15_a5: id_intf->rf_data_b = reg_file_intf->out_r15; break;
    case rf_t::x16_a6: id_intf->rf_data_b = reg_file_intf->out_r16; break;
    case rf_t::x17_a7: id_intf->rf_data_b = reg_file_intf->out_r17; break;
    case rf_t::x18_s2: id_intf->rf_data_b = reg_file_intf->out_r18; break;
    case rf_t::x19_s3: id_intf->rf_data_b = reg_file_intf->out_r19; break;
    case rf_t::x20_s4: id_intf->rf_data_b = reg_file_intf->out_r20; break;
    case rf_t::x21_s5: id_intf->rf_data_b = reg_file_intf->out_r21; break;
    case rf_t::x22_s6: id_intf->rf_data_b = reg_file_intf->out_r22; break;
    case rf_t::x23_s7: id_intf->rf_data_b = reg_file_intf->out_r23; break;
    case rf_t::x24_s8: id_intf->rf_data_b = reg_file_intf->out_r24; break;
    case rf_t::x25_s9: id_intf->rf_data_b = reg_file_intf->out_r25; break;
    case rf_t::x26_s10: id_intf->rf_data_b = reg_file_intf->out_r26; break;
    case rf_t::x27_s11: id_intf->rf_data_b = reg_file_intf->out_r27; break;
    case rf_t::x28_t3: id_intf->rf_data_b = reg_file_intf->out_r28; break;
    case rf_t::x29_t4: id_intf->rf_data_b = reg_file_intf->out_r29; break;
    case rf_t::x30_t5: id_intf->rf_data_b = reg_file_intf->out_r30; break;
    case rf_t::x31_t6: id_intf->rf_data_b = reg_file_intf->out_r31; break;
    default: LOGE("Invalid Reg File address for B port");
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
    LOG("reg file x8: " << reg_file_intf->out_r8);
    LOG("reg file x10: " << reg_file_intf->out_r10);
    LOG("reg file x11: " << reg_file_intf->out_r11);
    LOG("reg file x12: " << reg_file_intf->out_r12);
}
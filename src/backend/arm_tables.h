
#define REGISTERS \
    X( "D0",  0x00, REGTdouble ) \
    X( "D1",  0x01, REGTdouble ) \
    X( "D10", 0x0A, REGTdouble ) \
    X( "D11", 0x0B, REGTdouble ) \
    X( "D12", 0x0C, REGTdouble ) \
    X( "D13", 0x0D, REGTdouble ) \
    X( "D14", 0x0E, REGTdouble ) \
    X( "D15", 0x0F, REGTdouble ) \
    X( "D16", 0x10, REGTdouble ) \
    X( "D17", 0x11, REGTdouble ) \
    X( "D18", 0x12, REGTdouble ) \
    X( "D19", 0x13, REGTdouble ) \
    X( "D2",  0x02, REGTdouble ) \
    X( "D20", 0x14, REGTdouble ) \
    X( "D21", 0x15, REGTdouble ) \
    X( "D22", 0x16, REGTdouble ) \
    X( "D23", 0x17, REGTdouble ) \
    X( "D24", 0x18, REGTdouble ) \
    X( "D25", 0x19, REGTdouble ) \
    X( "D26", 0x1A, REGTdouble ) \
    X( "D27", 0x1B, REGTdouble ) \
    X( "D28", 0x1C, REGTdouble ) \
    X( "D29", 0x1D, REGTdouble ) \
    X( "D30", 0x1E, REGTdouble ) \
    X( "D3",  0x03, REGTdouble ) \
    X( "D31", 0x1F, REGTdouble ) \
    X( "D4",  0x04, REGTdouble ) \
    X( "D5",  0x05, REGTdouble ) \
    X( "D6",  0x06, REGTdouble ) \
    X( "D7",  0x07, REGTdouble ) \
    X( "D8",  0x08, REGTdouble ) \
    X( "D9",  0x09, REGTdouble ) \
\
    X( "FP",  0x0B, REGTcore ) \
    X( "IP",  0x0C, REGTcore ) \
    X( "LR",  0x0E, REGTcore ) \
    X( "PC",  0x0F, REGTcore ) \
\
    X( "Q0",  0x00, REGTquad ) \
    X( "Q1",  0x01, REGTquad ) \
    X( "Q10", 0x0A, REGTquad ) \
    X( "Q11", 0x0B, REGTquad ) \
    X( "Q12", 0x0C, REGTquad ) \
    X( "Q13", 0x0D, REGTquad ) \
    X( "Q14", 0x0E, REGTquad ) \
    X( "Q15", 0x0F, REGTquad ) \
    X( "Q16", 0x10, REGTquad ) \
    X( "Q2",  0x02, REGTquad ) \
    X( "Q3",  0x03, REGTquad ) \
    X( "Q4",  0x04, REGTquad ) \
    X( "Q5",  0x05, REGTquad ) \
    X( "Q6",  0x06, REGTquad ) \
    X( "Q7",  0x07, REGTquad ) \
    X( "Q8",  0x08, REGTquad ) \
    X( "Q9",  0x09, REGTquad ) \
\
    X( "R0",  0x00, REGTcore ) \
    X( "R1",  0x01, REGTcore ) \
    X( "R10", 0x0A, REGTcore ) \
    X( "R11", 0x0B, REGTcore ) \
    X( "R12", 0x0C, REGTcore ) \
    X( "R13", 0x0D, REGTcore ) \
    X( "R14", 0x0E, REGTcore ) \
    X( "R15", 0x0F, REGTcore ) \
    X( "R2",  0x02, REGTcore ) \
    X( "R3",  0x03, REGTcore ) \
    X( "R4",  0x04, REGTcore ) \
    X( "R5",  0x05, REGTcore ) \
    X( "R6",  0x06, REGTcore ) \
    X( "R7",  0x07, REGTcore ) \
    X( "R8",  0x08, REGTcore ) \
    X( "R9",  0x09, REGTcore ) \
\
    X( "S0",  0x00, REGTsingle ) \
    X( "S1",  0x01, REGTsingle ) \
    X( "S10", 0x0A, REGTsingle ) \
    X( "S11", 0x0B, REGTsingle ) \
    X( "S12", 0x0C, REGTsingle ) \
    X( "S13", 0x0D, REGTsingle ) \
    X( "S14", 0x0E, REGTsingle ) \
    X( "S15", 0x0F, REGTsingle ) \
    X( "S16", 0x10, REGTsingle ) \
    X( "S17", 0x11, REGTsingle ) \
    X( "S18", 0x12, REGTsingle ) \
    X( "S19", 0x13, REGTsingle ) \
    X( "S20", 0x14, REGTsingle ) \
    X( "S2",  0x02, REGTsingle ) \
    X( "S21", 0x15, REGTsingle ) \
    X( "S22", 0x16, REGTsingle ) \
    X( "S23", 0x17, REGTsingle ) \
    X( "S24", 0x18, REGTsingle ) \
    X( "S25", 0x19, REGTsingle ) \
    X( "S26", 0x1A, REGTsingle ) \
    X( "S27", 0x1B, REGTsingle ) \
    X( "S28", 0x1C, REGTsingle ) \
    X( "S29", 0x1D, REGTsingle ) \
    X( "S30", 0x1E, REGTsingle ) \
    X( "S3",  0x03, REGTsingle ) \
    X( "S31", 0x1F, REGTsingle ) \
    X( "S4",  0x04, REGTsingle ) \
    X( "S5",  0x05, REGTsingle ) \
    X( "S6",  0x06, REGTsingle ) \
    X( "S7",  0x07, REGTsingle ) \
    X( "S8",  0x08, REGTsingle ) \
    X( "S9",  0x09, REGTsingle ) \
\
    X( "SP",  0x0D, REGTcore )

#define CONDCODES \
    X( AL, 0xE ) \
    X( CC, 0x3 ) \
    X( CS, 0x2 ) \
    X( EQ, 0x0 ) \
    X( GE, 0xA ) \
    X( GT, 0xC ) \
    X( HI, 0x8 ) \
    X( HS, 0x2 ) \
    X( LE, 0xD ) \
    X( LO, 0x3 ) \
    X( LS, 0x9 ) \
    X( LT, 0xB ) \
    X( MI, 0x4 ) \
    X( NE, 0x1 ) \
    X( NV, 0xF ) \
    X( PL, 0x5 ) \
    X( VC, 0x7 ) \
    X( VS, 0x6 )

#define SIZESUFFIXES \
    X( "16" )\
    X( "32" )\
    X( "64" )\
    X( "8" )\
    X( "D" )\
    X( "F" )\
    X( "F16" )\
    X( "F16.F32" )\
    X( "F32" )\
    X( "F32.F16" )\
    X( "F32.F64" )\
    X( "F32.S16" )\
    X( "F32.S32" )\
    X( "F32.U16" )\
    X( "F32.U32" )\
    X( "F64" )\
    X( "F64.F32" )\
    X( "F64.S16" )\
    X( "F64.S32" )\
    X( "F64.U16" )\
    X( "F64.U32" )\
    X( "I16" )\
    X( "I32" )\
    X( "I64" )\
    X( "I8" )\
    X( "P16" )\
    X( "P8" )\
    X( "S16" )\
    X( "S16.F32" )\
    X( "S16.F64" )\
    X( "S32" )\
    X( "S32.F32" )\
    X( "S32.F64" )\
    X( "S64" )\
    X( "S8" )\
    X( "U16" )\
    X( "U16.F32" )\
    X( "U16.F64" )\
    X( "U32" )\
    X( "U32.F32" )\
    X( "U32.F64" )\
    X( "U64" )\
    X( "U8" )

#define SHIFTCODES \
    X( ASR ) \
    X( LSL ) \
    X( LSR ) \
    X( ROR ) \
    X( RRX )

// start at 0x10, leaving 0x00 through 0x0F as special ops
// dataproc operands must be on order from 0x10 through Ox1F due to opcode encoding logic
//   for the rest, no ordering matters
// list must be sorted in alpha order
#define OPCODES \
    X( ADC,      0x0015, 2, 4, ITdataproc, asm_emit_dataproc        ) \
    X( ADD,      0x0014, 2, 4, ITdataproc, asm_emit_dataproc        ) \
    X( ADR,      0x0020, 2, 3, 0,          asm_emit_dataproc        ) \
    X( AND,      0x0010, 2, 4, ITdataproc, asm_emit_dataproc        ) \
    X( ASR,      0x0021, 2, 3, ITdataproc, asm_emit_dataproc2       ) \
\
    X( B,        0x0022, 1, 1, ITjump,     asm_emit_branch          ) \
    X( BFC,      0x0023, 3, 3, 0,          asm_emit_media_misc      ) \
    X( BFI,      0x0024, 4, 4, 0,          asm_emit_media_misc      ) \
    X( BIC,      0x001E, 2, 4, ITdataproc, asm_emit_dataproc        ) \
    X( BKPT,     0x0025, 1, 1, 0,          asm_emit_misc            ) \
    X( BL,       0x0026, 1, 1, ITjump,     asm_emit_branch          ) \
    X( BLX,      0x0027, 1, 1, ITjump,     asm_emit_branch          ) \
    X( BX,       0x0028, 1, 1, ITjump,     asm_emit_misc            ) \
    X( BXJ,      0x0029, 0, 0, 0,          asm_emit_unsupported     ) \
\
    X( CBNZ,     0x002A, 0, 0, 0,          asm_emit_unsupported     ) \
    X( CBZ,      0x002B, 0, 0, 0,          asm_emit_unsupported     ) \
    X( CDP,      0x002C, 0, 0, IToddinstr, asm_emit_cdp             ) \
    X( CDP2,     0x002D, 0, 0, IToddinstr, asm_emit_cdp             ) \
    X( CHKA,     0x002E, 0, 0, 0,          asm_emit_unsupported     ) \
    X( CLREX,    0x002F, 0, 0, 0,          asm_emit_unimplemented   ) \
    X( CLZ,      0x0030, 2, 2, 0,          asm_emit_misc            ) \
    X( CMN,      0x001B, 2, 4, ITdataproc, asm_emit_dataproc        ) \
    X( CMP,      0x001A, 2, 4, ITdataproc, asm_emit_dataproc        ) \
    X( CPS,      0x0031, 0, 0, IToddinstr, asm_emit_odd             ) \
    X( CPSID,    0x0032, 0, 0, IToddinstr, asm_emit_odd             ) \
    X( CPSIE,    0x0033, 0, 0, IToddinstr, asm_emit_odd             ) \
    X( CPY,      0x0034, 2, 2, 0,          asm_emit_unimplemented   ) \
\
    X( DBG,      0x0035, 1, 1, 0,          asm_emit_hints           ) \
    X( DMB,      0x0036, 0, 1, 0,          asm_emit_unimplemented   ) \
    X( DSB,      0x0037, 0, 1, 0,          asm_emit_unimplemented   ) \
\
    X( ENTERX,   0x0038, 0, 0, 0,          asm_emit_unsupported     ) \
    X( EOR,      0x0011, 2, 4, ITdataproc, asm_emit_dataproc        ) \
    X( ERET,     0x018E, 0, 0, 0,          asm_emit_misc            ) \
\
    X( HB,       0x0039, 0, 0, 0,          asm_emit_unsupported     ) \
    X( HBL,      0x003A, 0, 0, 0,          asm_emit_unsupported     ) \
    X( HBLP,     0x003B, 0, 0, 0,          asm_emit_unsupported     ) \
    X( HBP,      0x003C, 0, 0, 0,          asm_emit_unsupported     ) \
    X( HVC,      0x018F, 1, 1, 0,          asm_emit_misc            ) \
\
    X( ISB,      0x003D, 0, 0, IToddinstr, asm_emit_odd             ) \
    X( IT,       0x003E, 0, 0, 0,          asm_emit_unimplemented   ) \
\
    X( LDC,      0x003F, 0, 0, IToddinstr, asm_emit_odd             ) \
    X( LDC2,     0x0040, 0, 0, IToddinstr, asm_emit_odd             ) \
    X( LDC2L,    0x0041, 0, 0, IToddinstr, asm_emit_odd             ) \
    X( LDCL,     0x0042, 0, 0, IToddinstr, asm_emit_odd             ) \
    X( LDM,      0x0043, 2, 2, 0,          asm_emit_blockdata       ) \
    X( LDMDA,    0x0044, 2, 2, 0,          asm_emit_blockdata       ) \
    X( LDMDB,    0x0045, 2, 2, 0,          asm_emit_blockdata       ) \
    X( LDMEA,    0x0046, 2, 2, 0,          asm_emit_blockdata       ) \
    X( LDMED,    0x0047, 2, 2, 0,          asm_emit_blockdata       ) \
    X( LDMFA,    0x0048, 2, 2, 0,          asm_emit_blockdata       ) \
    X( LDMFD,    0x0049, 2, 2, 0,          asm_emit_blockdata       ) \
    X( LDMIA,    0x004A, 2, 2, 0,          asm_emit_blockdata       ) \
    X( LDMIB,    0x004B, 2, 2, 0,          asm_emit_blockdata       ) \
    X( LDR,      0x004C, 2, 2, 0,          asm_emit_wordbyte_ldst   ) \
    X( LDRB,     0x004D, 2, 2, 0,          asm_emit_wordbyte_ldst   ) \
    X( LDRBT,    0x004E, 2, 2, 0,          asm_emit_wordbyte_ldst   ) \
    X( LDRD,     0x004F, 3, 3, 0,          asm_emit_extra_ldst      ) \
    X( LDREX,    0x0050, 2, 2, 0,          asm_emit_sync            ) \
    X( LDREXB,   0x0051, 2, 2, 0,          asm_emit_sync            ) \
    X( LDREXD,   0x0052, 3, 3, 0,          asm_emit_sync            ) \
    X( LDREXH,   0x0053, 2, 2, 0,          asm_emit_sync            ) \
    X( LDRH,     0x0054, 2, 2, 0,          asm_emit_extra_ldst      ) \
    X( LDRHT,    0x0055, 2, 2, 0,          asm_emit_extra_ldst      ) \
    X( LDRSB,    0x0056, 2, 2, 0,          asm_emit_extra_ldst      ) \
    X( LDRSBT,   0x0057, 2, 2, 0,          asm_emit_extra_ldst      ) \
    X( LDRSH,    0x0058, 2, 2, 0,          asm_emit_extra_ldst      ) \
    X( LDRSHT,   0x0059, 2, 2, 0,          asm_emit_extra_ldst      ) \
    X( LDRT,     0x005A, 2, 2, 0,          asm_emit_wordbyte_ldst   ) \
    X( LEAVEX,   0x005B, 0, 0, 0,          asm_emit_unsupported     ) \
    X( LSL,      0x005C, 2, 3, ITdataproc, asm_emit_dataproc2       ) \
    X( LSR,      0x005D, 2, 3, ITdataproc, asm_emit_dataproc2       ) \
\
    X( MCR,      0x005E, 0, 0, IToddinstr, asm_emit_odd             ) \
    X( MCR2,     0x005F, 0, 0, IToddinstr, asm_emit_odd             ) \
    X( MCRR,     0x0060, 0, 0, IToddinstr, asm_emit_odd             ) \
    X( MCRR2,    0x0061, 0, 0, IToddinstr, asm_emit_odd             ) \
    X( MLA,      0x0062, 4, 4, 0,          asm_emit_mult_accum      ) \
    X( MLS,      0x0063, 4, 4, 0,          asm_emit_mult_accum      ) \
    X( MOV,      0x001D, 2, 4, ITdataproc, asm_emit_dataproc        ) \
    X( MOVT,     0x0064, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( MOVW,     0x0065, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( MRC,      0x0066, 0, 0, IToddinstr, asm_emit_odd             ) \
    X( MRC2,     0x0067, 0, 0, IToddinstr, asm_emit_odd             ) \
    X( MRRC,     0x0068, 0, 0, IToddinstr, asm_emit_odd             ) \
    X( MRRC2,    0x0069, 0, 0, IToddinstr, asm_emit_odd             ) \
    X( MRS,      0x006A, 0, 0, IToddinstr, asm_emit_MRS             ) \
    X( MSR,      0x006B, 0, 0, IToddinstr, asm_emit_MSR             ) \
    X( MUL,      0x006C, 2, 3, 0,          asm_emit_mult_accum      ) \
    X( MVN,      0x001F, 2, 4, ITdataproc, asm_emit_dataproc        ) \
\
    X( NEG,      0x006D, 2, 2, 0,          asm_emit_dataproc        ) \
    X( NOP,      0x006E, 0, 0, 0,          asm_emit_hints           ) \
\
    X( ORN,      0x006F, 0, 0, 0,          asm_emit_unsupported     ) \
    X( ORR,      0x001C, 2, 4, ITdataproc, asm_emit_dataproc        ) \
\
    X( PKHBT,    0x0070, 2, 4, 0,          asm_emit_packing         ) \
    X( PKHTB,    0x0071, 2, 4, 0,          asm_emit_packing         ) \
    X( PLD,      0x0072, 1, 1, 0,          asm_emit_unimplemented   ) \
    X( PLDW,     0x0073, 1, 1, 0,          asm_emit_unimplemented   ) \
    X( PLI,      0x0074, 1, 1, 0,          asm_emit_unimplemented   ) \
    X( POP,      0x0075, 1, 1, 0,          asm_emit_blockdata       ) \
    X( PUSH,     0x0076, 1, 1, 0,          asm_emit_blockdata       ) \
\
    X( QADD,     0x0077, 2, 3, 0,          asm_emit_sat_add_sub     ) \
    X( QADD16,   0x0078, 2, 3, 0,          asm_emit_p_addsub        ) \
    X( QADD8,    0x0079, 2, 3, 0,          asm_emit_p_addsub        ) \
    X( QASX,     0x007A, 2, 3, 0,          asm_emit_p_addsub        ) \
    X( QDADD,    0x007B, 2, 3, 0,          asm_emit_sat_add_sub     ) \
    X( QDSUB,    0x007C, 2, 3, 0,          asm_emit_sat_add_sub     ) \
    X( QSAX,     0x007D, 2, 3, 0,          asm_emit_p_addsub        ) \
    X( QSUB,     0x007E, 2, 3, 0,          asm_emit_sat_add_sub     ) \
    X( QSUB16,   0x007F, 2, 3, 0,          asm_emit_p_addsub        ) \
    X( QSUB8,    0x0080, 2, 3, 0,          asm_emit_p_addsub        ) \
\
    X( RBIT,     0x0081, 2, 2, 0,          asm_emit_packing         ) \
    X( REV,      0x0082, 2, 2, 0,          asm_emit_packing         ) \
    X( REV16,    0x0083, 2, 2, 0,          asm_emit_packing         ) \
    X( REVSH,    0x0084, 2, 2, 0,          asm_emit_packing         ) \
    X( RFEDA,    0x0085, 1, 1, 0,          asm_emit_unimplemented   ) \
    X( RFEDB,    0x0086, 1, 1, 0,          asm_emit_unimplemented   ) \
    X( RFEIA,    0x0087, 1, 1, 0,          asm_emit_unimplemented   ) \
    X( RFEIB,    0x0088, 1, 1, 0,          asm_emit_unimplemented   ) \
    X( ROR,      0x0089, 2, 3, ITdataproc, asm_emit_dataproc2       ) \
    X( RRX,      0x008A, 1, 2, ITdataproc, asm_emit_dataproc2       ) \
    X( RSB,      0x0013, 2, 4, ITdataproc, asm_emit_dataproc        ) \
    X( RSC,      0x0017, 2, 4, ITdataproc, asm_emit_dataproc        ) \
\
    X( SADD16,   0x008B, 2, 3, 0,          asm_emit_p_addsub        ) \
    X( SADD8,    0x008C, 2, 3, 0,          asm_emit_p_addsub        ) \
    X( SASX,     0x008D, 2, 3, 0,          asm_emit_p_addsub        ) \
    X( SBC,      0x0016, 2, 4, ITdataproc, asm_emit_dataproc        ) \
    X( SBFX,     0x008E, 4, 4, 0,          asm_emit_media_misc      ) \
    X( SDIV,     0x008F, 2, 3, 0,          asm_emit_smult           ) \
    X( SEL,      0x0090, 2, 3, 0,          asm_emit_packing         ) \
    X( SETEND,   0x0091, 1, 1, 0,          asm_emit_unimplemented   ) \
    X( SEV,      0x0092, 0, 0, 0,          asm_emit_hints           ) \
    X( SHADD16,  0x0093, 2, 3, 0,          asm_emit_p_addsub        ) \
    X( SHADD8,   0x0094, 2, 3, 0,          asm_emit_p_addsub        ) \
    X( SHASX,    0x0095, 2, 3, 0,          asm_emit_p_addsub        ) \
    X( SHSAX,    0x0096, 2, 3, 0,          asm_emit_p_addsub        ) \
    X( SHSUB16,  0x0097, 2, 3, 0,          asm_emit_p_addsub        ) \
    X( SHSUB8,   0x0098, 2, 3, 0,          asm_emit_p_addsub        ) \
    X( SMC,      0x0099, 1, 1, 0,          asm_emit_misc            ) \
    X( SMLABB,   0x009A, 4, 4, 0,          asm_emit_half_mult_accum ) \
    X( SMLABT,   0x009B, 4, 4, 0,          asm_emit_half_mult_accum ) \
    X( SMLAD,    0x009C, 4, 4, 0,          asm_emit_smult           ) \
    X( SMLADX,   0x009D, 4, 4, 0,          asm_emit_smult           ) \
    X( SMLAL,    0x009E, 4, 4, 0,          asm_emit_mult_accum      ) \
    X( SMLALBB,  0x009F, 4, 4, 0,          asm_emit_half_mult_accum ) \
    X( SMLALBT,  0x00A0, 4, 4, 0,          asm_emit_half_mult_accum ) \
    X( SMLALD,   0x00A1, 4, 4, 0,          asm_emit_smult           ) \
    X( SMLALDX,  0x00A2, 4, 4, 0,          asm_emit_smult           ) \
    X( SMLALTB,  0x00A3, 4, 4, 0,          asm_emit_half_mult_accum ) \
    X( SMLALTT,  0x00A4, 4, 4, 0,          asm_emit_half_mult_accum ) \
    X( SMLATB,   0x00A5, 4, 4, 0,          asm_emit_half_mult_accum ) \
    X( SMLATT,   0x00A6, 4, 4, 0,          asm_emit_half_mult_accum ) \
    X( SMLAWB,   0x00A7, 4, 4, 0,          asm_emit_half_mult_accum ) \
    X( SMLAWT,   0x00A8, 4, 4, 0,          asm_emit_half_mult_accum ) \
    X( SMLSD,    0x00A9, 4, 4, 0,          asm_emit_smult           ) \
    X( SMLSDX,   0x00AA, 4, 4, 0,          asm_emit_smult           ) \
    X( SMLSLD,   0x00AB, 4, 4, 0,          asm_emit_smult           ) \
    X( SMLSLDX,  0x00AC, 4, 4, 0,          asm_emit_smult           ) \
    X( SMMLA,    0x00AD, 4, 4, 0,          asm_emit_smult           ) \
    X( SMMLAR,   0x00AE, 4, 4, 0,          asm_emit_smult           ) \
    X( SMMLS,    0x00AF, 4, 4, 0,          asm_emit_smult           ) \
    X( SMMLSR,   0x00B0, 4, 4, 0,          asm_emit_smult           ) \
    X( SMMUL,    0x00B1, 2, 3, 0,          asm_emit_smult           ) \
    X( SMMULR,   0x00B2, 2, 3, 0,          asm_emit_smult           ) \
    X( SMUAD,    0x00B3, 2, 3, 0,          asm_emit_smult           ) \
    X( SMUADX,   0x00B4, 2, 3, 0,          asm_emit_smult           ) \
    X( SMULBB,   0x00B5, 2, 3, 0,          asm_emit_half_mult_accum ) \
    X( SMULBT,   0x00B6, 2, 3, 0,          asm_emit_half_mult_accum ) \
    X( SMULL,    0x00B7, 4, 4, 0,          asm_emit_mult_accum      ) \
    X( SMULTB,   0x00B8, 2, 3, 0,          asm_emit_half_mult_accum ) \
    X( SMULTT,   0x00B9, 2, 3, 0,          asm_emit_half_mult_accum ) \
    X( SMULWB,   0x00BA, 2, 3, 0,          asm_emit_half_mult_accum ) \
    X( SMULWT,   0x00BB, 2, 3, 0,          asm_emit_half_mult_accum ) \
    X( SMUSD,    0x00BC, 2, 3, 0,          asm_emit_smult           ) \
    X( SMUSDX,   0x00BD, 2, 3, 0,          asm_emit_smult           ) \
    X( SRSDA,    0x00BE, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( SRSDB,    0x00BF, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( SRSIA,    0x00C0, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( SRSIB,    0x00C1, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( SSAT,     0x00C2, 3, 4, 0,          asm_emit_packing         ) \
    X( SSAT16,   0x00C3, 3, 3, 0,          asm_emit_packing         ) \
    X( SSAX,     0x00C4, 2, 3, 0,          asm_emit_p_addsub        ) \
    X( SSUB16,   0x00C5, 2, 3, 0,          asm_emit_p_addsub        ) \
    X( SSUB8,    0x00C6, 2, 3, 0,          asm_emit_p_addsub        ) \
    X( STC,      0x00C7, 0, 0, IToddinstr, asm_emit_odd             ) \
    X( STC2,     0x00C8, 0, 0, IToddinstr, asm_emit_odd             ) \
    X( STC2L,    0x00C9, 0, 0, IToddinstr, asm_emit_odd             ) \
    X( STCL,     0x00CA, 0, 0, IToddinstr, asm_emit_odd             ) \
    X( STM,      0x00CB, 2, 2, 0,          asm_emit_blockdata       ) \
    X( STMDA,    0x00CC, 2, 2, 0,          asm_emit_blockdata       ) \
    X( STMDB,    0x00CD, 2, 2, 0,          asm_emit_blockdata       ) \
    X( STMEA,    0x00CE, 2, 2, 0,          asm_emit_blockdata       ) \
    X( STMED,    0x00CF, 2, 2, 0,          asm_emit_blockdata       ) \
    X( STMFA,    0x00D0, 2, 2, 0,          asm_emit_blockdata       ) \
    X( STMFD,    0x00D1, 2, 2, 0,          asm_emit_blockdata       ) \
    X( STMIA,    0x00D2, 2, 2, 0,          asm_emit_blockdata       ) \
    X( STMIB,    0x00D3, 2, 2, 0,          asm_emit_blockdata       ) \
    X( STR,      0x00D4, 2, 2, 0,          asm_emit_wordbyte_ldst   ) \
    X( STRB,     0x00D5, 2, 2, 0,          asm_emit_wordbyte_ldst   ) \
    X( STRBT,    0x00D6, 2, 2, 0,          asm_emit_wordbyte_ldst   ) \
    X( STRD,     0x00D7, 3, 3, 0,          asm_emit_extra_ldst      ) \
    X( STREX,    0x00D8, 3, 3, 0,          asm_emit_sync            ) \
    X( STREXB,   0x00D9, 3, 3, 0,          asm_emit_sync            ) \
    X( STREXD,   0x00DA, 4, 4, 0,          asm_emit_sync            ) \
    X( STREXH,   0x00DB, 3, 3, 0,          asm_emit_sync            ) \
    X( STRH,     0x00DC, 2, 2, 0,          asm_emit_extra_ldst      ) \
    X( STRHT,    0x00DD, 2, 2, 0,          asm_emit_extra_ldst      ) \
    X( STRT,     0x00DE, 2, 2, 0,          asm_emit_wordbyte_ldst   ) \
    X( SUB,      0x0012, 2, 4, ITdataproc, asm_emit_dataproc        ) \
    X( SVC,      0x00DF, 1, 1, 0,          asm_emit_unimplemented   ) \
    X( SWP,      0x00E0, 3, 3, 0,          asm_emit_sync            ) \
    X( SWPB,     0x00E1, 3, 3, 0,          asm_emit_sync            ) \
    X( SXTAB,    0x00E2, 2, 4, 0,          asm_emit_packing         ) \
    X( SXTAB16,  0x00E3, 2, 4, 0,          asm_emit_packing         ) \
    X( SXTAH,    0x00E4, 2, 4, 0,          asm_emit_packing         ) \
    X( SXTB,     0x00E5, 1, 3, 0,          asm_emit_packing         ) \
    X( SXTB16,   0x00E6, 1, 3, 0,          asm_emit_packing         ) \
    X( SXTH,     0x00E7, 1, 3, 0,          asm_emit_packing         ) \
\
    X( TEQ,      0x0019, 2, 4, ITdataproc, asm_emit_dataproc        ) \
    X( TST,      0x0018, 2, 4, ITdataproc, asm_emit_dataproc        ) \
\
    X( UADD16,   0x00E8, 2, 3, 0,          asm_emit_p_addsub        ) \
    X( UADD8,    0x00E9, 2, 3, 0,          asm_emit_p_addsub        ) \
    X( UASX,     0x00EA, 2, 3, 0,          asm_emit_p_addsub        ) \
    X( UBFX,     0x00EB, 4, 4, 0,          asm_emit_media_misc      ) \
    X( UDF,      0x0190, 1, 1, 0,          asm_emit_media_misc      ) \
    X( UDIV,     0x00EC, 2, 3, 0,          asm_emit_smult           ) \
    X( UHADD16,  0x00ED, 2, 3, 0,          asm_emit_p_addsub        ) \
    X( UHADD8,   0x00EE, 2, 3, 0,          asm_emit_p_addsub        ) \
    X( UHASX,    0x00EF, 2, 3, 0,          asm_emit_p_addsub        ) \
    X( UHSAX,    0x00F0, 2, 3, 0,          asm_emit_p_addsub        ) \
    X( UHSUB16,  0x00F1, 2, 3, 0,          asm_emit_p_addsub        ) \
    X( UHSUB8,   0x00F2, 2, 3, 0,          asm_emit_p_addsub        ) \
    X( UMAAL,    0x00F3, 4, 4, 0,          asm_emit_mult_accum      ) \
    X( UMLAL,    0x00F4, 4, 4, 0,          asm_emit_mult_accum      ) \
    X( UMULL,    0x00F5, 4, 4, 0,          asm_emit_mult_accum      ) \
    X( UQADD16,  0x00F6, 2, 3, 0,          asm_emit_p_addsub        ) \
    X( UQADD8,   0x00F7, 2, 3, 0,          asm_emit_p_addsub        ) \
    X( UQASX,    0x00F8, 2, 3, 0,          asm_emit_p_addsub        ) \
    X( UQSAX,    0x00F9, 2, 3, 0,          asm_emit_p_addsub        ) \
    X( UQSUB16,  0x00FA, 2, 3, 0,          asm_emit_p_addsub        ) \
    X( UQSUB8,   0x00FB, 2, 3, 0,          asm_emit_p_addsub        ) \
    X( USAD8,    0x00FC, 2, 3, 0,          asm_emit_media_misc      ) \
    X( USADA8,   0x00FD, 4, 4, 0,          asm_emit_media_misc      ) \
    X( USAT,     0x00FE, 3, 4, 0,          asm_emit_packing         ) \
    X( USAT16,   0x00FF, 3, 3, 0,          asm_emit_packing         ) \
    X( USAX,     0x0100, 2, 3, 0,          asm_emit_p_addsub        ) \
    X( USUB16,   0x0101, 2, 3, 0,          asm_emit_p_addsub        ) \
    X( USUB8,    0x0102, 2, 3, 0,          asm_emit_p_addsub        ) \
    X( UXTAB,    0x0103, 2, 4, 0,          asm_emit_packing         ) \
    X( UXTAB16,  0x0104, 2, 4, 0,          asm_emit_packing         ) \
    X( UXTAH,    0x0105, 2, 4, 0,          asm_emit_packing         ) \
    X( UXTB,     0x0106, 1, 3, 0,          asm_emit_packing         ) \
    X( UXTB16,   0x0107, 1, 3, 0,          asm_emit_packing         ) \
    X( UXTH,     0x0108, 1, 3, 0,          asm_emit_packing         ) \
\
    X( VABA,     0x0109, 3, 3, 0,          asm_emit_unimplemented   ) \
    X( VABAL,    0x010A, 3, 3, 0,          asm_emit_unimplemented   ) \
    X( VABD,     0x010B, 3, 3, 0,          asm_emit_unimplemented   ) \
    X( VABDL,    0x010C, 3, 3, 0,          asm_emit_unimplemented   ) \
    X( VABS,     0x010D, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VACGE,    0x010E, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VACGT,    0x010F, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VACLE,    0x0110, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VACLT,    0x0111, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VADD,     0x0112, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VADDHN,   0x0113, 3, 3, 0,          asm_emit_unimplemented   ) \
    X( VADDL,    0x0114, 3, 3, 0,          asm_emit_unimplemented   ) \
    X( VADDW,    0x0115, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VAND,     0x0116, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VBIC,     0x0117, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VBIF,     0x0118, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VBIT,     0x0119, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VBSL,     0x011A, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VCEQ,     0x011B, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VCGE,     0x011C, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VCGT,     0x011D, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VCLE,     0x011E, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VCLS,     0x011F, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VCLT,     0x0120, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VCLZ,     0x0121, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VCMP,     0x0122, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VCMPE,    0x0123, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VCNT,     0x0124, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VCVT,     0x0125, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VCVTB,    0x0126, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VCVTR,    0x0127, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VCVTT,    0x0128, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VDIV,     0x0129, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VDUP,     0x012A, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VEOR,     0x012B, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VEXT,     0x012C, 3, 4, 0,          asm_emit_unimplemented   ) \
    X( VHADD,    0x012D, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VHSUB,    0x012E, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VLD1,     0x012F, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VLD2,     0x0130, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VLD3,     0x0131, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VLD4,     0x0132, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VLDMDB,   0x0133, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VLDMIA,   0x0134, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VLDR,     0x0135, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VMAX,     0x0136, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VMIN,     0x0137, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VMLA,     0x0138, 3, 3, 0,          asm_emit_unimplemented   ) \
    X( VMLAL,    0x0139, 3, 3, 0,          asm_emit_unimplemented   ) \
    X( VMLS,     0x013A, 3, 3, 0,          asm_emit_unimplemented   ) \
    X( VMLSL,    0x013B, 3, 3, 0,          asm_emit_unimplemented   ) \
    X( VMOV,     0x013C, 2, 4, 0,          asm_emit_unimplemented   ) \
    X( VMOVL,    0x013D, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VMOVN,    0x013E, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VMRS,     0x013F, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VMSR,     0x0140, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VMUL,     0x0141, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VMULL,    0x0142, 3, 3, 0,          asm_emit_unimplemented   ) \
    X( VMVN,     0x0143, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VNEG,     0x0144, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VNMLA,    0x0145, 3, 3, 0,          asm_emit_unimplemented   ) \
    X( VNMLS,    0x0146, 3, 3, 0,          asm_emit_unimplemented   ) \
    X( VNMUL,    0x0147, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VORN,     0x0148, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VORR,     0x0149, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VPADAL,   0x014A, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VPADD,    0x014B, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VPADDL,   0x014C, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VPMAX,    0x014D, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VPMIN,    0x014E, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VPOP,     0x014F, 1, 1, 0,          asm_emit_unimplemented   ) \
    X( VPUSH,    0x0150, 1, 1, 0,          asm_emit_unimplemented   ) \
    X( VQABS,    0x0151, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VQADD,    0x0152, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VQDMLAL,  0x0153, 3, 3, 0,          asm_emit_unimplemented   ) \
    X( VQDMULH,  0x0154, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VQDMULL,  0x0155, 3, 3, 0,          asm_emit_unimplemented   ) \
    X( VQMOVN,   0x0156, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VQMOVUN,  0x0157, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VQNEG,    0x0158, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VQRDMULH, 0x0159, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VQRSHL,   0x015A, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VQRSHRN,  0x015B, 3, 3, 0,          asm_emit_unimplemented   ) \
    X( VQRSHRUN, 0x015C, 3, 3, 0,          asm_emit_unimplemented   ) \
    X( VQSHL,    0x015D, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VQSHLU,   0x015E, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VQSHRN,   0x015F, 3, 3, 0,          asm_emit_unimplemented   ) \
    X( VQSHRUN,  0x0160, 3, 3, 0,          asm_emit_unimplemented   ) \
    X( VQSUB,    0x0161, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VRADDHN,  0x0162, 3, 3, 0,          asm_emit_unimplemented   ) \
    X( VRECPE,   0x0163, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VRECPS,   0x0164, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VRECRE,   0x0165, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VREV16,   0x0166, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VREV32,   0x0167, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VREV64,   0x0168, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VRHADD,   0x0169, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VRSHL,    0x016A, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VRSHR,    0x016B, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VRSHRN,   0x016C, 3, 3, 0,          asm_emit_unimplemented   ) \
    X( VRSQRTE,  0x016D, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VRSQRTS,  0x016E, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VRSRA,    0x016F, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VRSUBHN,  0x0170, 3, 3, 0,          asm_emit_unimplemented   ) \
    X( VSHI,     0x0171, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VSHL,     0x0172, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VSHLL,    0x0173, 3, 3, 0,          asm_emit_unimplemented   ) \
    X( VSHR,     0x0174, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VSHRN,    0x0175, 3, 3, 0,          asm_emit_unimplemented   ) \
    X( VSQRT,    0x0176, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VSRA,     0x0177, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VSRI,     0x0178, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VST1,     0x0179, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VST2,     0x017A, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VST3,     0x017B, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VST4,     0x017C, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VSTMDB,   0x017D, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VSTMIA,   0x017E, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VSTR,     0x017F, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VSUB,     0x0180, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VSUBHN,   0x0181, 3, 3, 0,          asm_emit_unimplemented   ) \
    X( VSUBL,    0x0182, 3, 3, 0,          asm_emit_unimplemented   ) \
    X( VSUBW,    0x0183, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VSWP,     0x0184, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VTBL,     0x0185, 3, 3, 0,          asm_emit_unimplemented   ) \
    X( VTBX,     0x0186, 3, 3, 0,          asm_emit_unimplemented   ) \
    X( VTRN,     0x0187, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VTST,     0x0188, 2, 3, 0,          asm_emit_unimplemented   ) \
    X( VUZP,     0x0189, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( VZIP,     0x018A, 2, 2, 0,          asm_emit_unimplemented   ) \
    X( WFE,      0x018B, 0, 0, 0,          asm_emit_hints           ) \
    X( WFI,      0x018C, 0, 0, 0,          asm_emit_hints           ) \
    X( YIELD,    0x018D, 0, 0, 0,          asm_emit_hints           )

// NOTE: highest numbered: UDF 0x0190


#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_helper(nop) {
	print_asm("nop");
	return 1;
}

make_helper(int3) {
	void do_int3();
	do_int3();
	print_asm("int3");

	return 1;
}

make_helper(lea) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = load_addr(eip + 1, &m, op_src);
	reg_l(m.reg) = op_src->addr;

	print_asm("leal %s,%%%s", op_src->str, regsl[m.reg]);
	return 1 + len;
}

make_helper(cld){
	cpu.DF = 0;
	print_asm("cld");
	return 1;
}

make_helper(cli){
	cpu.IF = 0;
	print_asm("cli");
	return 1;
}

make_helper(sti){
	cpu.IF = 1;
	print_asm("sti");
	return 1;
}


make_helper(clc){
	cpu.CF = 0;
	print_asm("clc");
	return 1;
}

make_helper(stc){
	cpu.CF = 1;
	print_asm("stc");
	return 1;
}
make_helper(lgdt){
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = load_addr(eip + 1, &m, op_src);
	cpu.GDTR.limit = lnaddr_read(op_src->addr, 2);
	cpu.GDTR.base = lnaddr_read(op_src->addr+2, 4);
	print_asm("lgdt seg_limit:%2x, base_addr:%x", cpu.GDTR.limit, cpu.GDTR.base);
	return 1 + len;
}

make_helper(lidt){
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = load_addr(eip + 1, &m, op_src);
	cpu.IDTR.limit = lnaddr_read(op_src->addr, 2);
	cpu.IDTR.base = lnaddr_read(op_src->addr+2, 4);
	print_asm("lidt seg_limit:%2x, base_addr:%x", cpu.IDTR.limit, cpu.IDTR.base);
	return 1 + len;
}

make_helper(std){
	cpu.DF = 1;
	print_asm("std");
	return 1;
}

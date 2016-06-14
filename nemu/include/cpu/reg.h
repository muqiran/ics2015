#ifndef __REG_H__
#define __REG_H__

#include "common.h"
#include "../../lib-common/x86-inc/cpu.h"

enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };
enum { R_ES, R_CS, R_SS, R_DS };

/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */

typedef struct {
	union {
		union {
			uint32_t _32;
			uint16_t _16;
			uint8_t _8[2];
		} gpr[8];

	/* Do NOT change the order of the GPRs' definitions. */

		struct {
			uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
		};
	};
	struct {
		uint32_t base;
		uint16_t limit;
	} GDTR;
	struct {
		uint32_t base;
		uint16_t limit;
	} IDTR;
/*	union {
		struct {
			unsigned PE :  1;
			unsigned a  : 31;
		};
		uint32_t _32;
	} CR0;*/
	CR0 cr0;
	CR3 cr3;
	union {
		union {
			struct {
				unsigned RPL   : 2;
				unsigned TI    : 1;
				unsigned INDEX : 13;
			};
			uint16_t _16;
		} SR[4];
		struct {
			uint16_t ES, CS, SS, DS;
		};
	};
	struct {
		bool vaild;
		uint32_t base;
		uint32_t limit;
	} SR_cache[4];
	swaddr_t eip;
	volatile bool INTR;

	union {
		struct {
			uint32_t CF: 1;
			uint32_t a : 1;
			uint32_t PF: 1;
			uint32_t b : 1;
			uint32_t AF: 1;
			uint32_t c : 1;
			uint32_t ZF: 1;
			uint32_t SF: 1;
			uint32_t TF: 1;
			uint32_t IF: 1;
			uint32_t DF: 1;
			uint32_t OF: 1;
		};
		uint32_t eflags;
	};
} CPU_state;

extern CPU_state cpu;
static inline int check_reg_index(int index) {
	assert(index >= 0 && index < 8);
	return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define reg_b(index) (cpu.gpr[check_reg_index(index) & 0x3]._8[index >> 2])
#define sreg(index) (cpu.SR[check_reg_index(index)]._16)

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];
extern const char* sr[];

#endif

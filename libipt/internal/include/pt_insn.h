/*
 * Copyright (c) 2016, Intel Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  * Neither the name of Intel Corporation nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef PT_INSN_H
#define PT_INSN_H

#include <inttypes.h>

struct pt_insn;


/* A finer-grain classification of instructions used internally. */
typedef enum {
	PTI_INST_INVALID,

	PTI_INST_CALL_9A,
	PTI_INST_CALL_FFr3,
	PTI_INST_CALL_FFr2,
	PTI_INST_CALL_E8,
	PTI_INST_INT,

	PTI_INST_INT3,
	PTI_INST_INT1,
	PTI_INST_INTO,
	PTI_INST_IRET,	/* includes IRETD and IRETQ (EOSZ determines) */

	PTI_INST_JMP_E9,
	PTI_INST_JMP_EB,
	PTI_INST_JMP_EA,
	PTI_INST_JMP_FFr5,	/* REXW? */
	PTI_INST_JMP_FFr4,
	PTI_INST_JCC,
	PTI_INST_JrCXZ,
	PTI_INST_LOOP,
	PTI_INST_LOOPE,	/* aka Z */
	PTI_INST_LOOPNE,	/* aka NE */

	PTI_INST_MOV_CR3,

	PTI_INST_RET_C3,
	PTI_INST_RET_C2,
	PTI_INST_RET_CB,
	PTI_INST_RET_CA,

	PTI_INST_SYSCALL,
	PTI_INST_SYSENTER,
	PTI_INST_SYSEXIT,
	PTI_INST_SYSRET,

	PTI_INST_VMLAUNCH,
	PTI_INST_VMRESUME,
	PTI_INST_VMCALL,
	PTI_INST_VMPTRLD,

	PTI_INST_LAST
} pti_inst_enum_t;

/* Information about an instruction we need internally in addition to the
 * information provided in struct pt_insn.
 */
struct pt_insn_ext {
	/* A more detailed instruction class. */
	pti_inst_enum_t iclass;

	/* Instruction-specific information. */
	union {
		/* For branch instructions. */
		struct {
			/* The target address.
			 *
			 * This is only valid for direct calls/jumps.
			 */
			uint64_t target;

			/* A flag saying whether the branch is direct.
			 *
			 *   non-zero: direct
			 *   zero:     indirect
			 *
			 * This is expected to go away someday when we extend
			 * enum pt_insn_class to distinguish direct and indirect
			 * branches.
			 */
			uint8_t is_direct;
		} branch;
	} variant;
};


/* Check if the instruction @insn/@iext changes the current privilege level.
 *
 * Returns non-zero if it does, zero if it doesn't (or @insn/@iext is NULL).
 */
extern int pt_insn_changes_cpl(const struct pt_insn *insn,
			       const struct pt_insn_ext *iext);

/* Check if the instruction @insn/@iext changes CR3.
 *
 * Returns non-zero if it does, zero if it doesn't (or @insn/@iext is NULL).
 */
extern int pt_insn_changes_cr3(const struct pt_insn *insn,
			       const struct pt_insn_ext *iext);

/* Check if the instruction @insn/@iext is a far branch.
 *
 * Returns non-zero if it is, zero if it isn't (or @insn/@iext is NULL).
 */
extern int pt_insn_is_far_branch(const struct pt_insn *insn,
				 const struct pt_insn_ext *iext);

/* Check if the instruction @insn/@iext binds to a PIP packet.
 *
 * Returns non-zero if it does, zero if it doesn't (or @insn/@iext is NULL).
 */
extern int pt_insn_binds_to_pip(const struct pt_insn *insn,
				const struct pt_insn_ext *iext);

/* Check if the instruction @insn/@iext binds to a VMCS packet.
 *
 * Returns non-zero if it does, zero if it doesn't (or @insn/@iext is NULL).
 */
extern int pt_insn_binds_to_vmcs(const struct pt_insn *insn,
				 const struct pt_insn_ext *iext);

#endif /* PT_INSN_H */

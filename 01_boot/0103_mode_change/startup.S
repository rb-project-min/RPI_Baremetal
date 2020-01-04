	.section ".text"
	.global _start
	.balign 0x4000
	.arm
_start:
// ================================================================
//  IO_Base_Addr, LocalPeripheralsAddrの設定
// ================================================================
	mrc		p15, 0, r4, c0, c0, 0			// MIDRの読み込み
	bic		r4, r4, #0x0F					// RevisionIDをクリア

	mov		r1, #0x20000000					// IO_Base_Addr(Raspberry pi 1/Zeroの場合)
	mov		r2, #0x00000000					// LocalPeripheralsAddr(Raspberry pi 1/Zeroの場合)

	ldr		r3, =0x410FB760					// ARM11(Raspberry pi 1/Zero)
	cmp		r4, r3

	movne	r1, #0x3F000000					// IO_Base_Addr(Raspberry pi 1/Zeroの場合)
	movne	r2, #0x40000000					// LocalPeripheralsAddr(Raspberry pi 1/Zeroの場合)

	ldrne	r3, =0x410FC070					// Cortex-A7(Raspberry pi 2)
	cmpne	r4, r3
	ldrne	r3, =0x410FD030					// Cortex-A53(Raspberry pi 3)
	cmpne	r4, r3

	movne	r1, #0xFE000000					// IO_Base_Addr(Raspberry pi 4の場合)
	orrne	r2, r1, #0x01800000				// LocalPeripheralsAddr(Raspberry pi 4の場合)

	str		r1, RPi_IO_Base_Addr			// IO_Base_Addrの保存
	str		r2, RPi_LocalPeripheralsAddr	// LocalPeripheralsAddrの保存

// ================================================================
//  Raspberry pi 1/Zeroでは直接cpsrに書き込む
//  Raspberry pi 2以降でCPUモードがHYP_MODEの場合、
//  eretを使用してシステムモードに移行
// ================================================================
    mrs		r0, cpsr				// CPUモードビットを含むCPSRレジスタをコピー
 	and		r1, r0, #0x1F			// CPUモードビットの抽出
 	cmp		r1, #0x1A				// HYP_MODEの確認
	orr		r0, r0, #0xDF			// システムモードをセット(IRQ/FIQは無効)
    msreq	spsr_cxsf, r0			// (HYP_MODEの場合)SPSRレジスタへコピー
	addeq	lr, pc, #4				// (HYP_MODEの場合)戻りアドレスを算出

	// コンパイルエラーが出るので .word で直接指示
	.word	0x012ef30e				// msreq ELR_hyp, lr	(HYP_MODEの場合)戻りアドレスをコピー
	.word	0x0160006e				// ereteq				(HYP_MODEの場合)システムモードでリターン

    mrs		r6, cpsr				// CPUモードビットを含むCPSRレジスタをコピー
	orr		r6, r6, #0xDF			// システムモードをセット(IRQ/FIQは無効)
    msr		CPSR_c, r6				// システムモードへ切り替え

// ================================================================
//  main関数にジャンプする
// ================================================================
	mov		sp, #0x800000					// スタックポインタを適当に設定
	bl		main

// ================================================================
//  main関数から戻った場合は無限ループ
// ================================================================
hang:
	b		.

	.globl RPi_IO_Base_Addr
RPi_IO_Base_Addr:				// Peripheral Base Addr
	.word	0x20000000

	.globl RPi_LocalPeripheralsAddr
RPi_LocalPeripheralsAddr:		// Local Peripherals Addr
	.word	0x40000000


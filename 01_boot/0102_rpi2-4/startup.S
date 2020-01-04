	.section ".text"
	.global _start
	.balign 0x4000
	.arm
_start:
// ================================================================
//  IO_Base_Addr, LocalPeripheralsAddrの設定
// ================================================================
	mrc		p15, 0, r4, c0, c0, 0			// MIDRの読み込み
	bic		r4, r4, #0x0F					// RevisionIDをクリア(念のため)

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
//  スタックポインタを設定してmain関数へジャンプ
// ================================================================
	mov		sp, #0x800000					// スタックポインタを適当に設定
	b		main							// main関数へジャンプ
hang:
	b		.								// 無限ループ

	.globl RPi_IO_Base_Addr
RPi_IO_Base_Addr:				// Peripheral Base Addr
	.word	0x20000000

	.globl RPi_LocalPeripheralsAddr
RPi_LocalPeripheralsAddr:		// Local Peripherals Addr
	.word	0x40000000


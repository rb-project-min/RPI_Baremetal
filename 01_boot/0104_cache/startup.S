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

	// eretによる戻り先
    mrs		r0, cpsr				// CPUモードビットを含むCPSRレジスタをコピー
	orr		r0, r0, #0xDF			// システムモードをセット(IRQ/FIQは無効)
    msr		CPSR_c, r0				// システムモードへ切り替え

// ================================================================
//  NSACR(非セキュアアクセス制御レジスタ)の設定
//  mcrneを使うと何故かフリーズするのでジャンプを使用
// ================================================================
	mrc		p15, 0, r0, c1, c1, 2	// NSACR(非セキュアアクセス制御レジスタ)
	and		r1, r0, #0x0C00
	cmp		r1, #0x0C00				// 浮動小数点ユニットが有効かチェック
	beq		1f						// 既に有効ならジャンプ
	orr		r0, r0, #0x0C00			// セキュアモード,非セキュアモードのアクセス許可
	mcr		p15, 0, r0, c1, c1, 2	// NSACR(非セキュアアクセス制御レジスタ)

// ================================================================
//  FPUをオンラインにする
//  Raspberry pi 4ではisbを使わないと何故かフリーズする
// ================================================================
1:	mrc		p15, 0, r0, c1, c0, 2	// CPACR(アーキテクチャ機能アクセス制御レジスタ)
	orr		r0, #0xF00000			// 単精度/倍精度浮動小数点演算の有効化
	mcr		p15, 0, r0, c1, c0, 2	// CPACR(アーキテクチャ機能アクセス制御レジスタ)

	ldr		r11, RPi_LocalPeripheralsAddr	// RaspberryPi1/Zeroでは0x00000000となる
	cmp		r11, #0					// RaspberryPi1/Zeroチェック(isb命令用)
	beq		1f						// isb命令をスキップ

	// コンパイルエラーが出るので .word で直接指示
	.word	0xF57FF06F				// isb

1:	mov		r0, #0x40000000			// VFP有効化
	vmsr	fpexc, r0				// FPEXC書き込み

// ================================================================
//  キャッシュを有効にする
// ================================================================
    mrc		p15, 0, r0, c1, c0, 0	// SCTLR(システムコントロールレジスタ)
	orr		r0, r0, #4
	orr		r0, r0, #6144			// C bit, Z bit, I bitをセット(データキャッシュ,フロー予測,命令キャッシュの有効化) 
	mcr		p15, 0, r1, c1, c0, 0	// SCTLR

	cmp		r11, #0					// RaspberryPi1/Zeroチェック(dsb/isb命令用)
	beq		1f						// dsb/isb命令をスキップ

	// コンパイルエラーが出るので .word で直接指示
	.word	0xF57FF04F				// dsb
	.word	0xF57FF06F				// isb

// ================================================================
//  スタックポインタを設定してmain関数へジャンプ
// ================================================================
1:	mov		sp, #0x800000					// スタックポインタを適当に設定
	b		main							// main関数へジャンプ
hang:
	b		.								// 無限ループ

	.globl RPi_IO_Base_Addr
RPi_IO_Base_Addr:				// Peripheral Base Addr
	.word	0x20000000

	.globl RPi_LocalPeripheralsAddr
RPi_LocalPeripheralsAddr:		// Local Peripherals Addr
	.word	0x40000000


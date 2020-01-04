	.section ".text"
	.global _start
	.balign 0x4000
	.arm
_start:
// ================================================================
//  0x0000にロードした場合の割り込みエリア 0x0000にコピーする
//  ジャンプ先アドレスは後に生成する
// ================================================================
	b		_reset
	ldr		pc, _undefined_instruction_vector_h
	ldr		pc, _software_interrupt_vector_h
	ldr		pc, _prefetch_abort_vector_h
	ldr		pc, _data_abort_vector_h
	b		.
	ldr		pc, _interrupt_vector_h
	ldr		pc, _fast_interrupt_vector_h

_reset_h:							.word   _start
_undefined_instruction_vector_h:	.word   hang
_software_interrupt_vector_h:		.word   hang
_prefetch_abort_vector_h:			.word   hang
_data_abort_vector_h:				.word   hang
_interrupt_vector_h:				.word   hang
_fast_interrupt_vector_h:			.word   hang

_reset:
// ================================================================
//  初期CPUモードとブートアドレスを保存する
// ================================================================
	adr		r11, _start				// ブートアドレス
	sub		r10, r11, #0x8000		// ブートアドレスと実アドレスのオフセット
	str		r10, RPi_BootOffset		// ブートオフセットの保存

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
//  割り込みテーブルを0x0000にコピー
// ================================================================
	mov     r1, #0x0000						// コピー先アドレス
	mov		r0, r11							// コピー元アドレス
    ldmia   r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	mov		r2, r3							// リセット割り込み命令を書き換え
    stmia   r1!, {r2, r3, r4, r5, r6, r7, r8, r9}

	adr		r2, _start						// リセット割り込みアドレス
	adr		r3, hang						// 未定義割り込みアドレス
	mov		r4, r3							// ソフトウェア割り込みアドレス
	mov		r5, r3							// プリフェッチアボート割り込みアドレス
	mov		r6, r3							// データアボート割り込みアドレス
	mov		r7, r3							// IRQ割り込みアドレス
	mov		r8, r3							// FIQ割り込みアドレス
    stmia   r1, {r2, r3, r4, r5, r6, r7, r8}	// 割り込みジャンプ先を書き込み
    stmia   r0, {r2, r3, r4, r5, r6, r7, r8}	// 割り込みジャンプ先を書き込み

// ================================================================
//  CPUモードがHYP_MODEの場合、SVC_MODEに移行
//  Raspberry pi 1/Zeroでは起動時SVC_MODEのため不要
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

// ================================================================
//  SVCモード以外のスタックポインタのセットアップ
// ================================================================
	ldr		r0, =__FIQ_stack_core0	// FIQモード用スタックポインタ(Core 0)
	ldr		r1, =__IRQ_stack_core0	// IRQモード用スタックポインタ(Core 0)
	ldr		r2, =__SVC_stack_core0	// SVCモード用スタックポインタ(Core 0)
	ldr		r3, =__ABT_stack_core0	// アボートモード用スタックポインタ(Core 0)
	ldr		r4, =__UND_stack_core0	// 未定義モード用スタックポインタ(Core 0)

	ldr		r5, RPi_LocalPeripheralsAddr	// シングルコア判別用にLocalPeripheralsAddrを読み込む
	cmp		r5, #0					// アドレスがない場合はシングルコア

	mrcne	p15, 0, r5, c0, c0, 5	// Core IDの読み込み(ARM7/8)
	andnes	r5, r5, #0x03			// Core IDのみ抽出(ついでに0比較)

	ldrne	r0, =__FIQ_stack_core1	// FIQモード用スタックポインタ(Core 1)
	ldrne	r1, =__IRQ_stack_core1	// IRQモード用スタックポインタ(Core 1)
	ldrne	r2, =__SVC_stack_core1	// SVCモード用スタックポインタ(Core 1)
	ldrne	r3, =__ABT_stack_core1	// アボートモード用スタックポインタ(Core 1)
	ldrne	r4, =__UND_stack_core1	// 未定義モード用スタックポインタ(Core 1)
	cmpne	r5, #0x01

	ldrne	r0, =__FIQ_stack_core2	// FIQモード用スタックポインタ(Core 2)
	ldrne	r1, =__IRQ_stack_core2	// IRQモード用スタックポインタ(Core 2)
	ldrne	r2, =__SVC_stack_core2	// SVCモード用スタックポインタ(Core 2)
	ldrne	r3, =__ABT_stack_core2	// アボートモード用スタックポインタ(Core 2)
	ldrne	r4, =__UND_stack_core2	// 未定義モード用スタックポインタ(Core 2)
	cmpne	r5, #0x02

	ldrne	r0, =__FIQ_stack_core3	// FIQモード用スタックポインタ(Core 3)
	ldrne	r1, =__IRQ_stack_core3	// IRQモード用スタックポインタ(Core 3)
	ldrne	r2, =__SVC_stack_core3	// SVCモード用スタックポインタ(Core 3)
	ldrne	r3, =__ABT_stack_core3	// アボートモード用スタックポインタ(Core 3)
	ldrne	r4, =__UND_stack_core3	// 未定義モード用スタックポインタ(Core 3)

	ldr		r7, RPi_BootOffset		// ブートオフセット
									// (Core1以降は_startを通らないため,ここで読み込む)
	add		r0, r0, r7				// FIQモード用スタックポインタのアドレス補正
	add		r1, r1, r7				// IRQモード用スタックポインタのアドレス補正
	add		r2, r2, r7				// SVCモード用スタックポインタのアドレス補正
	add		r3, r3, r7				// アボートモード用スタックポインタのアドレス補正
	add		r4, r4, r7				// 未定義モード用スタックポインタのアドレス補正

    mrs		r6, cpsr				// CPUモードビットを含むCPSRレジスタをコピー
	bic		r6, r6, #0x1F
	orr		r6, r6, #0xD1			// FIQモードをセット(IRQ/FIQは無効)
    msr		CPSR_c, r6				// FIQモードへ切り替え
	mov		sp, r0

	bic		r6, r6, #0x1F
	orr		r6, r6, #0xD2			// IRQモードをセット(IRQ/FIQは無効)
    msr		CPSR_c, r6				// IRQモードへ切り替え
	mov		sp, r1

	bic		r6, r6, #0x1F
	orr		r6, r6, #0xD3			// SVCモードをセット(IRQ/FIQは無効)
    msr		CPSR_c, r6				// SVCモードへ切り替え
	mov		sp, r2

	bic		r6, r6, #0x1F
	orr		r6, r6, #0xD7			// アボートモードをセット(IRQ/FIQは無効)
    msr		CPSR_c, r6				// アボートモードへ切り替え
	mov		sp, r3

	bic		r6, r6, #0x1F
	orr		r6, r6, #0xDB			// 未定義モードをセット(IRQ/FIQは無効)
    msr		CPSR_c, r6				// 未定義モードへ切り替え
	mov		sp, r4

	orr		r6, r6, #0xDF			// システムモードをセット(IRQ/FIQは無効)
    msr		CPSR_c, r6				// システムモードへ切り替え
// ================================================================
//  システムモードのスタックポインタのセットアップ
//  STACK_SIZEを定義している場合はその値を優先
// ================================================================
#ifdef STACK_SIZE
	ldr		r2, =STACK_SIZE
#else
	mov		r2, #32768
#endif
	ldr		r4, =__stack_end		// スタック領域下端
	add		r4, r4, r7				// スタック領域下端のアドレス補正

	add		sp, r4, r2				// スタック領域上端(Core 0)
	cmp		r5, #0x00

	addne	sp, sp, r2				// スタック領域上端(Core 1)
	cmpne	r5, #0x01

	addne	sp, sp, r2				// スタック領域上端(Core 2)
	cmpne	r5, #0x02

	addne	sp, sp, r2				// スタック領域上端(Core 3)

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

	ldr		r11, RPi_LocalPeripheralsAddr
	cmp		r11, #0					// ARM11チェック(isb命令用)
	beq		1f						// isb命令をスキップ

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
//  main関数にジャンプする
// ================================================================
1:	bl		main

// ================================================================
//  main関数から戻った場合は無限ループ
// ================================================================
hang:
	b		.

RPi_BootOffset:
	.word	0x00000000			// ブートアドレスと実アドレスのオフセット

	.globl RPi_IO_Base_Addr
RPi_IO_Base_Addr:				// Peripheral Base Addr
	.word	0x20000000

	.globl RPi_LocalPeripheralsAddr
RPi_LocalPeripheralsAddr:		// Local Peripherals Addr
	.word	0x40000000


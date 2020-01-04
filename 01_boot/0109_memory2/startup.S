	.section ".text"
	.global _start
	.balign 0x4000
	.arm
_start:
// ================================================================
//  0x0000にロードした場合の割り込みエリア 0x0000にコピーする
//  コピー後は絶対アドレスでアクセスするため
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
	sub		r11, r11, #0x8000		// ブートアドレスと実アドレスのオフセット
	str		r11, RPi_BootOffset		// ブートオフセットの保存

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
//  rpi.hにてSTACK_SIZEを定義している場合はその値を優先
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

	cmp		r5, #0x00
	bne		memory_setup_finish		// Core 1以降の場合はメモリ初期化をスキップ

// ================================================================
//  メモリ配置の計算
// ================================================================
	adr		r0, mbox1				// mailboxデータアドレス
	mov		r1, #8					// mailboxチャンネル
	bl		mailbox					// mailboxルーチン(r0, r1は非破壊)

	ldr		r6, VC_Offset			// GPUメモリ下端
	ldr		r4, VC_Size				// GPUメモリサイズ

	add		r7, r4, r6				// GPUメモリ上端

	ldr		r8, RPi_IO_Base_Addr

// ================================================================
//  テーブルの作成
//  1GB以上の領域はGPUと入れ替えて連続領域として使用する
// ================================================================
	mov		r2, #0x4000

// メインメモリ ～r6
	ldr		r10, =0x00011C0E
	mov		r0, r10					// キャッシュあり メインメモリ下端
	add		r1, r6, r10				// キャッシュあり メインメモリ上端
1:	cmp		r0, r1					// 上端に到達した場合にループ抜ける
	strne	r0, [r2], #4			// テーブル書き込み
	addne	r0, r0, #0x00100000		// メモリ加算
	bne		1b						// ループ

// 1GB以上 r7～r8(存在しない場合もあり)
	add		r0, r7, r10				// キャッシュあり ハイメモリ下端
	add		r1, r8, r10				// キャッシュあり ハイメモリ上端
1:	cmp		r0, r1					// 上端に到達した場合にループ抜ける
	strne	r0, [r2], #4			// テーブル書き込み
	addne	r0, r0, #0x00100000		// メモリ加算
	bne		1b						// ループ

// GPU r6～r7
	ldr		r10, =0x00010C02
	add		r0, r6, r10				// キャッシュなし GPUメモリ下端
	add		r1, r7, r10				// キャッシュなし GPUメモリ上端
1:	cmp		r0, r1					// 上端に到達した場合にループ抜ける
	strne	r0, [r2], #4			// テーブル書き込み
	addne	r0, r0, #0x00100000		// メモリ加算
	bne		1b						// ループ

// I/Oエリア r8～
	add		r0, r8, r10				// キャッシュなし I/Oメモリ下端
	mov		r1, r10					// キャッシュなし I/Oメモリ上端(オーバーフロー)
1:	cmp		r0, r1					// 上端に到達した場合にループ抜ける
	strne	r0, [r2], #4			// テーブル書き込み
	addne	r0, r0, #0x00100000		// メモリ加算
	bne		1b						// ループ

memory_setup_finish:
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
//  ドメインアクセス制御レジスタの全クライアント設定
// ================================================================
	ldr		r0,=0x55555555			// クライアント設定
	mcr		p15, 0, r0, c3, c0, 0	// DACRに書き込み

// ================================================================
//  TLB,分岐予測器アレイ,命令キャッシュを無効にする
// ================================================================
	mov		r0, #0
	mcr		p15, 0, r0, c8, c7, 0	// TLBの無効化
	mcr		p15, 0, r0, c7, c5, 6	// 分岐予測器アレイの無効化
	mcr		p15, 0, r0, c7, c5, 0	// 命令キャッシュの無効化

// ================================================================
//  データキャッシュを無効にする
// ================================================================
1:	mcr		p15, 0, r0, c7, c6, 2	// セット/ウェイ設定でキャッシュラインを無効化
									// (初期値 r0=0x00000000)
	add		r0, r0, #0x20			// セット番号 +1
	tst		r0, #0x2000				// セット番号の確認
	beq		1b						// セット番号がオーバーフローするまでループ

	bic		r0, r0, #0x2000			// セット番号のオーバーフローをクリア
	add		r0, r0, #0x40000000		// ウェイ番号 +1
	tst		r0, #0xC0000000			// ウェイ番号の確認
	bne		1b						// ウェイ番号がオーバーフローするまでループ

// ================================================================
//  TTBCR, TTBR0の書き換え
// ================================================================
	mcr		p15, 0, r0, c2, c0, 2	// TTBCRの初期化(r0=0x00000000)
	mov		r0, #0x4000
	orr		r0, r0, #0x48
	mcr		p15, 0, r0, c2, c0, 0	// TTBR0の書き込み

// ================================================================
//  MMUを有効にする
// ================================================================
    mrc		p15, 0, r0, c1, c0, 0	// SCTLR(システムコントロールレジスタ)
    orr		r0, #0x1				// MMUの有効化
    mcr		p15, 0, r0, c1, c0, 0	// SCTLR(システムコントロールレジスタ)

	cmp		r11, #0					// RaspberryPi1/Zeroチェック(dsb/isb命令用)
	beq		1f						// dsb/isb命令をスキップ

	// コンパイルエラーが出るので .word で直接指示
	.word	0xF57FF04F				// dsb
	.word	0xF57FF06F				// isb

// ================================================================
//  ARM7/8でCore 1～3はコアの準備完了としてジャンプ
//  Core 0は継続
// ================================================================
1:	mov		r0, #1
	adr		r1, RPi_CoresReady
	strb	r0, [r1, r5]			// CoreReadyカウントの保存
	cmp		r5, #0
	bne		SecondarySpin			// Core 1～3のループへ

// ================================================================
//  Core 1～3の初期化プロセスを起動
// ================================================================
	cmp		r11, #0					// ARM11チェック(マルチコア処理スキップ用)
	beq		.setup_finish			// シングルコアの場合はスキップ

    adr		r2, _reset				// Core 1～3のセットアップアドレス
	str		r2, [r11, #0x9C]		// Mailbox3(Core 1)にアドレスを転送
	str		r2, [r11, #0xAC]		// Mailbox3(Core 2)にアドレスを転送
	str		r2, [r11, #0xBC]		// Mailbox3(Core 3)にアドレスを転送
	sev
	ldr		r2, =0x01010101
1:	ldr		r1, RPi_CoresReady
	cmp		r1, r2
	bne		1b						// 全コアの準備が完了するまでループ

// ================================================================
//  main関数にジャンプする
// ================================================================
.setup_finish:
	bl		main

// ================================================================
//  main関数から戻った場合は無限ループ
// ================================================================
hang:
	b		.								// 無限ループ

// ================================================================
//  Core 1～3のループ
// ================================================================
SecondarySpin:
	mrc		p15, 0, r0, c0, c0, 5
	and		r0, r0, #0x03			// Make core 2 bit bitmask in R0
	ldr		r5, RPi_LocalPeripheralsAddr
	add		r5, r5, #0xCC			// Mailbox3 (Core 0) Read
	mov		r3, #0					// magic
1:	wfe								// イベント待ちスリープ
	ldr		r4, [r5, r0, lsl #4]	// Mailbox3 (Core x) Read
	str		r4, [r5, r0, lsl #4]	// Mailbox3 (Core x) をクリア
	cmp		r4, r3					// Mailboxに何か書き込まれているかチェック
	beq		1b						// 何もない場合はループ

	mov		r0, #0					// r0 = 0(詳細不明)
	ldr		r1, =3138				// r1 = マシンID(詳細不明)
	ldr		r2, =0x100				// r2 = ATAGS(詳細不明)
	adr		lr, SecondarySpin		// 戻りアドレスをSecondarySpinにする
	bx		r4						// 受け取ったアドレスへジャンプ

// ================================================================
//  Coreの起動(第1引数:コアID, 第2引数:実行する関数のアドレス)
// ================================================================
	.balign	4
	.globl CoreExecute
CoreExecute:
	cmp		r0, #0x04
	bcs		CoreExecuteFail			// 存在しないコアの場合は失敗扱い

	ands	r0, r0, #0x03
	beq		CoreExecuteFail			// Core 0の場合は失敗扱い

	ldr		r3, RPi_LocalPeripheralsAddr
	cmp		r3, #0
	beq		CoreExecuteFail			// Raspberry Pi 1/Zeroの場合は失敗扱い

	add		r3, r3, #0xCC			// Mailbox3 (Core 0) Read
	ldr		r2, [r3, r0, lsl #4]	// Mailboxアドレスを読み込み
	cmp		r2, #0
	bne		CoreExecuteFail			// アドレスが残ったままの場合は失敗

	sub		r3, r3, #0x40			// Mailbox3 (Core 0) Write
	str		r1, [r3, r0, lsl #4]	// Mailboxへアドレスを転送
	sev								// すべてのコアを復帰する
	mov		r0, #1					// 戻り値 1
	bx		lr						// 戻る
CoreExecuteFail:
	mov		r0, #0					// 戻り値 0
	bx		lr						// 戻る

// ================================================================
//  mailboxアクセス
//  void mailbox(void *AlinedBuffer, uint32_t Channel);
// ================================================================
	.balign	4
	.globl mailbox
mailbox:
	ldr		r12, RPi_IO_Base_Addr
	add		r12, r12, #0x0000B800

	bic		r2, r0, #0x0F
	and		r3, r1, #0x0F
	orr		r2, r2, r3

1:	ldr		r3, [r12, #0x98]
	ands	r3, #0x80000000
	bne		1b

	str		r2, [r12, #0xA0]

2:	ldr		r3, [r12, #0x98]
	ands	r3, #0x40000000
	bne		2b

	ldr		r3, [r12, #0x80]
	cmp		r3, r2
	bne		1b

	bx		lr

RPi_CoresReady:
	.word	0x00000000			// 準備完了したコア数

RPi_BootOffset:
	.word	0x00000000			// ブートアドレスと実アドレスのオフセット

	.globl RPi_IO_Base_Addr
RPi_IO_Base_Addr:				// Peripheral Base Addr
	.word	0x20000000

	.globl RPi_LocalPeripheralsAddr
RPi_LocalPeripheralsAddr:		// Local Peripherals Addr
	.word	0x40000000

	.balign	16
mbox1:
	.word	0x00000020			// Buffer Length
	.word	0x00000000			// Request
	.word	0x00010006			// RPI_FIRMWARE_GET_VC_MEMORY
	.word	0x00000008			// Data Length
	.word	0x00000000			// Request
	.globl VC_Offset
VC_Offset:
	.word	0x00000000			// Memory Offset
	.globl VC_Size
VC_Size:
	.word	0x00000000			// Memory Size
	.word	0x00000000			// Data End

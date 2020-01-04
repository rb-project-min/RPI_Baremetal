	.section ".text"
	.global _start
	.balign 0x4000
	.arm
_start:
	mov		sp, #0x800000			// スタックポインタを適当に設定
	b		main


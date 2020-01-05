OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
	. = 0x8000;

	.text : {
		. = ALIGN(8);
		__text_start = .;
		*(.text*)
		__text_end = .;
	}

	.init : {
		. = ALIGN(8);
		__init_start = .;
		*(.init*)
		__init_end = .;
	}

	.fini : {
		. = ALIGN(8);
		__fini_start = .;
		*(.fini*)
		__fini_end = .;
	}

	.rodata : {
		. = ALIGN(8);
		__rodata_start = .;
		*(.rodata*)
		__rodata_end = .;
	}

	.data : {
		. = ALIGN(8);
		__data_start = .;
		*(.data*)
		__data_end = .;
	}

	.bss : {
		. = ALIGN(8);
		__bss_start = .;
		*(.bss*)
		__bss_end = .;
	}

	.stack : {
		. = ALIGN(8);
		__stack_start = .;
		. = . + 512;
		__FIQ_stack_core0 = .;
		. = . + 512;
		__IRQ_stack_core0 = .;
		. = . + 512;
		__SVC_stack_core0 = .;
		. = . + 512;
		__ABT_stack_core0 = .;
		. = . + 512;
		__UND_stack_core0 = .;
		. = . + 512;
		__FIQ_stack_core1 = .;
		. = . + 512;
		__IRQ_stack_core1 = .;
		. = . + 512;
		__SVC_stack_core1 = .;
		. = . + 512;
		__ABT_stack_core1 = .;
		. = . + 512;
		__UND_stack_core1 = .;
		. = . + 512;
		__FIQ_stack_core2 = .;
		. = . + 512;
		__IRQ_stack_core2 = .;
		. = . + 512;
		__SVC_stack_core2 = .;
		. = . + 512;
		__ABT_stack_core2 = .;
		. = . + 512;
		__UND_stack_core2 = .;
		. = . + 512;
		__FIQ_stack_core3 = .;
		. = . + 512;
		__IRQ_stack_core3 = .;
		. = . + 512;
		__SVC_stack_core3 = .;
		. = . + 512;
		__ABT_stack_core3 = .;
		. = . + 512;
		__UND_stack_core3 = .;
		__stack_end = .;
	}
}


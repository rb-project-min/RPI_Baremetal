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
}


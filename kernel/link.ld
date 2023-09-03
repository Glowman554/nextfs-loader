ENTRY(_start)

SECTIONS
{
	. = 0x500;
	
	.text : {
		*(.text.init)
		*(.text)
	}
	.data  : {
        start_ctors = .;
		KEEP(*( .init_array ));
		KEEP(*(SORT_BY_INIT_PRIORITY( .init_array.* )));
		end_ctors = .;
		*(.data)
	}
	.rodata  : {
		*(.rodata)
	}
    .bss : {
		*(.bss)
	}
}
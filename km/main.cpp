#include "utils.hpp"
#define nt_qword_sig "\x48\x8B\x05\x41\xEF\x05\x00"
#define nt_qword_mask "xxxxxxx"

__int64 request_handler(uintptr_t request)
{
	PDRIVER_REQUEST in = reinterpret_cast<PDRIVER_REQUEST>(request);
	switch (in->type)
	{
		case BASE:
		{
			in->base = utils::get_base_address(in);
			break;
		}
		case WRITE:
		{
			utils::write_process_memory(in);
			break;
		}
		case READ:
		{
			utils::read_process_memory(in);
			break;
		}
	}
	return 0;
}

NTSTATUS driver_entry()
{
	const uintptr_t win32k = utils::get_kernel_module("win32k.sys");


	if (!win32k) {
		DbgPrintEx(0, 0, "!win32k\n");
		return STATUS_FAILED_DRIVER_ENTRY;
	} 
	uintptr_t nt_qword = utils::pattern_scan(win32k, nt_qword_sig, nt_qword_mask);
	if (!nt_qword) {
		DbgPrintEx(0, 0, "!nt_qword\n");
		return STATUS_FAILED_DRIVER_ENTRY;
	} 
	uintptr_t nt_qword_deref = nt_qword + *(int*)((PUCHAR)nt_qword + 3) + 7;
	*reinterpret_cast<uintptr_t*>(nt_qword_deref) = reinterpret_cast<uintptr_t>(&request_handler);
	if (nt_qword_deref) {
		DbgPrintEx(0, 0, "nt_qword_deref\n");
	}
	
	return STATUS_SUCCESS;
}
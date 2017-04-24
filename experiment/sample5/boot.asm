; void cc(unsigned int eax,char* addr)
extern cc

[section .text]

global _start

_start:
	mov ecx, eax
	push ebx
	push ecx
	call cc
	add esp, 8

	mov eax, 0x00550077

	hlt
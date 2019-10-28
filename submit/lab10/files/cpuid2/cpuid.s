#Currently get_cpuid(int *eax, int *ebx, int *ecx, int *edx).
#Modify to get_cpuid(int cpuid_op, int *eax, int *ebx, int *ecx, int *edx).	
	.text
	.globl get_cpuid
get_cpuid:
	pushq   %rdx
	pushq   %rcx
	pushq   %r8
	cpuid
	cpuid
	#largest param in %eax
	#12-char manufacturer string in ebx, edx, ecx.
	movl	%eax, (%rsi)	#store eax cpuid result
	popq    %rax
	movl	%edx, (%rax)	#store ebx cpuid result
	popq	%rax		#pop address for edxP
	movl    %ecx, (%rax)    #store edx cpuid result
	popq    %rax		#pop address for ecxP
	movl	%ebx, (%rax)	#store ecx cpuid result
	ret
	

	.section .text
	.globl _start;
	len = 8 #Размер массива
	enroll = 4 #Количество обрабатываемых элементов за одну итерацию
	elem_sz = 4 #Размер одного элемента массива
_start:
	addi x20, x0, len/enroll
	la x1, _x
loop:	
	lw x2, 0(x1)
	add x31, x31, x2
	lw x2, 4(x1)
	add x31, x31, x2
	lw x2, 8(x1)
	add x31, x31, x2
	lw x2, 12(x1)
	add x31, x31, x2
	addi x1, x1, elem_sz*enroll
	addi x20, x20, -1
	bne x20, x0, loop
	addi x31, x31, 1
forever: j forever

	.section .data
_x:	.4byte 0x1
	.4byte 0x2
	.4byte 0x3
	.4byte 0x4
	.4byte 0x5
	.4byte 0x6
	.4byte 0x7
	.4byte 0x8

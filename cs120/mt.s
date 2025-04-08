raphael
	.data
space:	.byte ' ' 
lf:	.byte '\n'

	.text
main:
	addi $sp, $sp -4
	sw $ra, 0($sp)

	li $a0, 7
	li $a1,	10
	jal mt
	
	li $v0, 11
	lb $a0, lf
	syscall
	
	li $a0, 4
	li $a1, 5
	jal mt
	
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	jr $ra

mt:
	addi $sp, $sp, -20
	sw $ra, 0($sp)
	sw $s0, 4($sp)
	sw $s1, 8($sp)
	sw $s2, 12($sp)
	sw $s3, 16($sp)
	
	move $s1, $a0
	move $s3, $a1
	
	li $s0, 1
oloop:
	bgt $s0, $s1, odone
	
	li $s2, 1
iloop:
	bgt $s2, $s3, idone
	
	mul $t0, $s0, $s2
	move $a0, $t0
	li $v0, 1
	syscall
	
	lb $a0, space
	li $v0, 11
	syscall
	
	addi, $s2, $s2, 1
	j iloop
	
idone:
	lb $a0, lf
	li $v0, 11
	syscall
	
	addi $s0, $s0, 1
	j oloop
	
odone:
	lw $s3, 16($sp)
	lw $s2, 12($sp)
	lw $s1, 8($sp)
	lw $s0, 4($sp)
	lw $ra, 0($sp)
	
	addi $sp, $sp, 20
	jr $ra

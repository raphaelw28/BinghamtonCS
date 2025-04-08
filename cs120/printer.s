.data
name: .asciiz "Raphael Wong"
.text
.globl main

main:
	#Print name
	li $v0 4
	la $a0, name
	syscall
	li $t0, 18
	li $t1, 27
	li $t2, -11
	add $t3, $t0, $t1
	add $t3, $t3, $t2
	
	
	li $v0, 1
	move $a0, $t3
	syscall
	
	
	li $v0, 10
	syscall

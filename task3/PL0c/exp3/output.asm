.data
stack: .space 16384
.text
main:
la $fp, stack
addi $fp, $fp, 16384
add $sp, $zero, $fp
la $t0, end
sw $sp, 0($sp)
sw $sp, -4($sp)
sw $t0, -8($sp)
addi $sp, $sp, -12
_1:
addi $sp, $sp, -24

_2:
li $v0, 11
la $a0, '?'
syscall
li $v0, 5
syscall
sw $v0, 0($sp)
addi $sp, $sp, -4

_3:
addi $a0, $zero, 0
add $a1, $zero, $fp
jal base
addi $sp, $sp, 4
addi $t0, $v0, -20
lw $t1, 0($sp)
sw $t1, 0($t0)

_4:
addi $a0, $zero, 0
add $a1, $zero, $fp
jal base
addi $t0, $v0, -20
lw $t0, 0($t0)
sw $t0, 0($sp)
addi $sp, $sp, -4

_5:
addi $a0, $zero, 0
add $a1, $zero, $fp
jal base
addi $sp, $sp, 4
addi $t0, $v0, -12
lw $t1, 0($sp)
sw $t1, 0($t0)

_6:
li $t0, 1
sw $t0, 0($sp)
addi $sp, $sp, -4

_7:
addi $a0, $zero, 0
add $a1, $zero, $fp
jal base
addi $sp, $sp, 4
addi $t0, $v0, -16
lw $t1, 0($sp)
sw $t1, 0($t0)

_8:
addi $a0, $zero, 0
add $a1, $zero, $fp
jal base
addi $t0, $v0, -16
lw $t0, 0($t0)
sw $t0, 0($sp)
addi $sp, $sp, -4

_9:
li $t0, 5
sw $t0, 0($sp)
addi $sp, $sp, -4

_10:
addi $sp, $sp, 4
lw $t0, 0($sp)
lw $t1, 4($sp)
slt $t0, $t1, $t0
sw $t0, 4($sp)

_11:
addi $sp, $sp, 4
lw $t0, 0($sp)
beq $t0, $zero, _25

_12:
li $v0, 11
la $a0, '?'
syscall
li $v0, 5
syscall
sw $v0, 0($sp)
addi $sp, $sp, -4

_13:
addi $a0, $zero, 0
add $a1, $zero, $fp
jal base
addi $sp, $sp, 4
addi $t0, $v0, -20
lw $t1, 0($sp)
sw $t1, 0($t0)

_14:
addi $a0, $zero, 0
add $a1, $zero, $fp
jal base
addi $t0, $v0, -20
lw $t0, 0($t0)
sw $t0, 0($sp)
addi $sp, $sp, -4

_15:
addi $a0, $zero, 0
add $a1, $zero, $fp
jal base
addi $t0, $v0, -12
lw $t0, 0($t0)
sw $t0, 0($sp)
addi $sp, $sp, -4

_16:
addi $sp, $sp, 4
lw $t0, 0($sp)
lw $t1, 4($sp)
slt $t0, $t1, $t0
sw $t0, 4($sp)

_17:
addi $sp, $sp, 4
lw $t0, 0($sp)
beq $t0, $zero, _20

_18:
addi $a0, $zero, 0
add $a1, $zero, $fp
jal base
addi $t0, $v0, -20
lw $t0, 0($t0)
sw $t0, 0($sp)
addi $sp, $sp, -4

_19:
addi $a0, $zero, 0
add $a1, $zero, $fp
jal base
addi $sp, $sp, 4
addi $t0, $v0, -12
lw $t1, 0($sp)
sw $t1, 0($t0)

_20:
addi $a0, $zero, 0
add $a1, $zero, $fp
jal base
addi $t0, $v0, -16
lw $t0, 0($t0)
sw $t0, 0($sp)
addi $sp, $sp, -4

_21:
li $t0, 1
sw $t0, 0($sp)
addi $sp, $sp, -4

_22:
addi $sp, $sp, 4
lw $t0, 0($sp)
lw $t1, 4($sp)
add $t0, $t0, $t1
sw $t0, 4($sp)

_23:
addi $a0, $zero, 0
add $a1, $zero, $fp
jal base
addi $sp, $sp, 4
addi $t0, $v0, -16
lw $t1, 0($sp)
sw $t1, 0($t0)

_24:
j _8

_25:
addi $a0, $zero, 0
add $a1, $zero, $fp
jal base
addi $t0, $v0, -12
lw $t0, 0($t0)
sw $t0, 0($sp)
addi $sp, $sp, -4

_26:
li $v0, 1
addi $sp, $sp, 4
lw $a0, 0($sp)
syscall

_27:
li $v0, 11
li $a0, 10
syscall

_28:
add $sp, $zero, $fp
lw $ra, -8($sp)
lw $fp, -4($sp)
jr $ra

end:
li $v0, 10
syscall

base:
beqz $a0, baseend
baseloop:
lw $a1, 0($a1)
addi $a0, $a0, -1
bgtz $a0, baseloop
baseend:
add $v0, $a1, $zero
jr $ra


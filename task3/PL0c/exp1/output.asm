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
addi $sp, $sp, -28

_2:
li $t0, 1
sw $t0, 0($sp)
addi $sp, $sp, -4

_3:
addi $a0, $zero, 0
add $a1, $zero, $fp
jal base
addi $sp, $sp, 4
addi $t0, $v0, -12
lw $t1, 0($sp)
sw $t1, 0($t0)

_4:
li $t0, 1
sw $t0, 0($sp)
addi $sp, $sp, -4

_5:
addi $a0, $zero, 0
add $a1, $zero, $fp
jal base
addi $sp, $sp, 4
addi $t0, $v0, -16
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
addi $t0, $v0, -24
lw $t1, 0($sp)
sw $t1, 0($t0)

_8:
addi $a0, $zero, 0
add $a1, $zero, $fp
jal base
addi $t0, $v0, -24
lw $t0, 0($t0)
sw $t0, 0($sp)
addi $sp, $sp, -4

_9:
li $t0, 20
sw $t0, 0($sp)
addi $sp, $sp, -4

_10:
addi $sp, $sp, 4
lw $t0, 0($sp)
lw $t1, 4($sp)
slt $t0, $t0, $t1
xori $t0, $t0, 1
sw $t0, 4($sp)

_11:
addi $sp, $sp, 4
lw $t0, 0($sp)
beq $t0, $zero, _28

_12:
addi $a0, $zero, 0
add $a1, $zero, $fp
jal base
addi $t0, $v0, -12
lw $t0, 0($t0)
sw $t0, 0($sp)
addi $sp, $sp, -4

_13:
li $v0, 1
addi $sp, $sp, 4
lw $a0, 0($sp)
syscall

_14:
li $v0, 11
li $a0, 10
syscall

_15:
addi $a0, $zero, 0
add $a1, $zero, $fp
jal base
addi $t0, $v0, -12
lw $t0, 0($t0)
sw $t0, 0($sp)
addi $sp, $sp, -4

_16:
addi $a0, $zero, 0
add $a1, $zero, $fp
jal base
addi $t0, $v0, -16
lw $t0, 0($t0)
sw $t0, 0($sp)
addi $sp, $sp, -4

_17:
addi $sp, $sp, 4
lw $t0, 0($sp)
lw $t1, 4($sp)
add $t0, $t0, $t1
sw $t0, 4($sp)

_18:
addi $a0, $zero, 0
add $a1, $zero, $fp
jal base
addi $sp, $sp, 4
addi $t0, $v0, -20
lw $t1, 0($sp)
sw $t1, 0($t0)

_19:
addi $a0, $zero, 0
add $a1, $zero, $fp
jal base
addi $t0, $v0, -16
lw $t0, 0($t0)
sw $t0, 0($sp)
addi $sp, $sp, -4

_20:
addi $a0, $zero, 0
add $a1, $zero, $fp
jal base
addi $sp, $sp, 4
addi $t0, $v0, -12
lw $t1, 0($sp)
sw $t1, 0($t0)

_21:
addi $a0, $zero, 0
add $a1, $zero, $fp
jal base
addi $t0, $v0, -20
lw $t0, 0($t0)
sw $t0, 0($sp)
addi $sp, $sp, -4

_22:
addi $a0, $zero, 0
add $a1, $zero, $fp
jal base
addi $sp, $sp, 4
addi $t0, $v0, -16
lw $t1, 0($sp)
sw $t1, 0($t0)

_23:
addi $a0, $zero, 0
add $a1, $zero, $fp
jal base
addi $t0, $v0, -24
lw $t0, 0($t0)
sw $t0, 0($sp)
addi $sp, $sp, -4

_24:
li $t0, 1
sw $t0, 0($sp)
addi $sp, $sp, -4

_25:
addi $sp, $sp, 4
lw $t0, 0($sp)
lw $t1, 4($sp)
add $t0, $t0, $t1
sw $t0, 4($sp)

_26:
addi $a0, $zero, 0
add $a1, $zero, $fp
jal base
addi $sp, $sp, 4
addi $t0, $v0, -24
lw $t1, 0($sp)
sw $t1, 0($t0)

_27:
j _8

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


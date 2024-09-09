# embedding func for pcode to mips
def pcode_to_mips():
    # head codes
    mips = [
        ".data",            
        "stack: .space 16384",   # define the max space for stack
        ".text",            
        "main:",                 # main func
        
        "la $fp, stack",      
        "addi $fp, $fp, 16384",
        "add $sp, $zero, $fp",
        
        "la $t0, end",
        "sw $sp, 0($sp)",
        "sw $sp, -4($sp)", 
        "sw $t0, -8($sp)",
        "addi $sp, $sp, -12"
    ]
    
    # open the pcode file, which is fa.tmp
    with open(r'C:\Users\86158\Desktop\PL0c\fa.tmp') as program:
        # go thru every line in the tmp file
        for instruction in program.readlines():
            parts = instruction.strip().split()
            # instruction = num + command + x + y
            # eg: 1 int 0 7
            # split each instruction into different variables
            num = parts[0]
            command = parts[1]
            x = parts[2]
            y = parts[3]
            
            # set jump point as num
            mips.append(f"_{num}:")
            
            # set stack movement
            # ATTENTION: _read2 && base is perplex commands
            
            # 1: basic operations for stack-pointer
            stack_up = "addi $sp, $sp, 4"
            stack_down = "addi $sp, $sp, -4"
            
            # 2: read 1 figure, hold the stack-pointer
            _read1 = "lw $t0, 0($sp)"       # load to t0
            read1_ = "sw $t0, 0($sp)"       # load back
            
            # 3: read 2 figures, move the stack-pointer
            _read2 = [
                stack_up,
                "lw $t0, 0($sp)",
                "lw $t1, 4($sp)"
            ]
            read2_ = "sw $t0, 4($sp)"
            
            # 4: get base level
            base = [
                f"addi $a0, $zero, {x}",
                "add $a1, $zero, $fp",
                "jal base"
            ]
            
            # judge command
            if command == 'opr':
                # return 
                if y == '0':
                    mips.append("add $sp, $zero, $fp")
                    mips.append("lw $ra, -8($sp)")
                    mips.append("lw $fp, -4($sp)")
                    mips.append("jr $ra")   
                    
                # neg
                elif y == '1':
                    mips.append(_read1)
                    mips.append("neg $t0, $t0")     # neg
                    mips.append(read1_)
                    
                # add
                elif y == '2':
                    for code in _read2:
                        mips.append(code)
                    mips.append("add $t0, $t0, $t1")# add, store at t0
                    mips.append(read2_)
                    
                # sub
                elif y == '3':
                    for code in _read2:
                        mips.append(code)
                    mips.append("sub $t0, $t0, $t1")# sub, store at t0
                    mips.append(read2_)
                    
                # mul
                elif y == '4':
                    for code in _read2:
                        mips.append(code)
                    mips.append("mul $t0, $t0, $t1")# mul, store at t0
                    mips.append(read2_)
                    
                # div
                elif y == '5':
                    for code in _read2:
                        mips.append(code)
                    mips.append("div $t0, $t0, $t1")# div, store at t0
                    mips.append("mflo $t0")         # remove the remaining
                    mips.append(read2_)
                    
                # if singular?
                elif y == '6':
                    mips.append(read1_)
                    mips.append("andi $t0, $t0, 1") # and, store at t0
                    mips.append(_read1)
                    
                # y == '7' is not defined
                
                # equal
                elif y == '8':
                    for code in _read2:
                        mips.append(code)
                    
                    mips.append("sub $t0, $t1, $t0")
                    mips.append("sltu $t0, $zero, $t0")
                    mips.append("xori $t0, $t0, 1")
                    
                    mips.append(read2_)
                    
                # not equal
                elif y == '9':
                    for code in _read2:
                        mips.append(code)
                    
                    mips.append("sub $t0, $t1, $t0")
                    mips.append("sltu $t0, $zero, $t0")
                    
                    mips.append(read2_)
                    
                # less than
                elif y == '10':
                    for code in _read2:
                        mips.append(code)
                    
                    mips.append("slt $t0, $t1, $t0")
                    
                    mips.append(read2_)
                    
                # less equal than
                elif y == '11':
                    for code in _read2:
                        mips.append(code)
                    
                    mips.append("slt $t0, $t1, $t0")
                    mips.append("xori $t0, $t0, 1")
                    
                    mips.append(read2_)
                    
                # greater than
                elif y == '12':
                    for code in _read2:
                        mips.append(code)
                    
                    mips.append("slt $t0, $t0, $t1")
                    mips.append("xori $t0, $t0, 1")
                    
                    mips.append(read2_)
                    
                # greater equal than
                elif y == '13':
                    for code in _read2:
                        mips.append(code)
                    
                    mips.append("slt $t0, $t0, $t1")
                    mips.append("xori $t0, $t0, 1")
                    
                    mips.append(read2_)
                    
                # output stack_up
                elif y == '14':
                    mips.append("li $v0, 1")        # output integer, using func 1
                    mips.append(stack_up)
                    mips.append("lw $a0, 0($sp)")   # load sp to a0
                    mips.append("syscall")          # call system
                    
                # change line
                elif y == '15':
                    mips.append("li $v0, 11")       # output character, using func 11
                    mips.append("li $a0, 10")       # load change-line ASCII to a0
                    mips.append("syscall")          # call system
                    
                # input
                elif y == '16':
                    # show ?
                    mips.append("li $v0, 11")
                    mips.append("la $a0, '?'")
                    mips.append("syscall")          # call system
                    
                    mips.append("li $v0, 5")        # read integer, using func 5
                    mips.append("syscall")          # call system
                    mips.append("sw $v0, 0($sp)")   # load to stack_up
                    mips.append(stack_down)         # stack_down
            
            # load to stack-top
            elif command == 'lit':
                mips.append(f"li $t0, {y}")         # put y into t0
                mips.append("sw $t0, 0($sp)")       # load to stack_up
                mips.append(stack_down)             # stack_down
            
            # direct jump
            elif command == 'jmp':
                mips.append(f"j _{y}")              # jump to [_y]
            
            # conditional jump
            elif command == 'jpc':
                mips.append(stack_up)
                mips.append("lw $t0, 0($sp)")       # load to t0
                mips.append(f"beq $t0, $zero, _{y}")    # if t0 = 0, then jump to [_y]
            
            # allocate memory
            elif command == 'int':
                offset = int(y) * 4
                mips.append(f"addi $sp, $sp, -{offset}")    # stack down y-step
            
            # call sub-process
            elif command == 'cal':
                for baseline in base:
                    mips.append(baseline)
                number = int(num) + 1
                mips.append(f"la $t0, _{number}")
                mips.append("sw $v0, 0($sp)")
                mips.append("sw $fp, -4($sp)")
                mips.append("sw $t0, -8($sp)")
                mips.append("add $fp, $sp, $zero")
                mips.append(f"j _{y}")
            
            # store stack top to variable
            elif command == 'sto':
                for baseline in base:
                    mips.append(baseline)
                mips.append(stack_up)
                offset = int(y) * 4
                mips.append(f"addi $t0, $v0, -{offset}")
                mips.append("lw $t1, 0($sp)")
                mips.append("sw $t1, 0($t0)")
            
            # load variable to stack top
            elif command == 'lod':
                for baseline in base:
                    mips.append(baseline)
                offset = int(y) * 4
                mips.append(f"addi $t0, $v0, -{offset}")
                mips.append("lw $t0, 0($t0)")
                mips.append("sw $t0, 0($sp)")
                mips.append(stack_down)
            
            # error alert
            else:
                print(f"unknown command: {instruction}")
            
            mips.append("")
    
    return mips

# apply the embedding func
mips = pcode_to_mips()

# end codes
end = [
    "end:",
    "li $v0, 10",
    "syscall",
    ""
]

# base codes
base_end = [
    "base:",
    "beqz $a0, baseend",
    "baseloop:",
    "lw $a1, 0($a1)",
    "addi $a0, $a0, -1",
    "bgtz $a0, baseloop",
    "baseend:",
    "add $v0, $a1, $zero",
    "jr $ra",
    ""
]

for endline in end:
    mips.append(endline)

for baseendline in base_end:
    mips.append(baseendline)

# output mips in the terminal
for line in mips:
    print(line)

# if you'd like to save the converted codes as an asm file, 
# you need to follow the command below:
# -----------------------------------------------------------------------
# py pcode2mips.py | out-file "output.asm" -encoding ascii
# -----------------------------------------------------------------------
# then there should be an asm file on the same path of this python file

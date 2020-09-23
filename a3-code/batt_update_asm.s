

.text
.global  set_batt_from_ports

## ENTRY POINT FOR REQUIRED FUNCTION
set_batt_from_ports:
        ## assembly instructions here

        ## a useful technique for this problem

      movw  BATT_VOLTAGE_PORT(%rip), %dx   # copy global var to reg dx  (16-bit word)
      movb  BATT_STATUS_PORT(%rip), %cl    # copy global var to reg cl  (8-bit byte)
      # movl  %r8d, BATT_DISPLAY_PORT(%rip)  # copy reg r8d to global var (32-bit long-word)

      cmpw $0, %dx
      jl .negative           # if (BATT_VOLTAGE_PORT < 0), return 1
      jge .positive          # BATT_VOLTAGE_PORT >= 0, calculate
      # volts 0(%rdi) 2bytes
      # percent 2(%rdi) 1
      # mode 3(%rdi) 1
.negative:

      movl $1, %eax          # return 1
      ret

.positive:
      movw %dx, 0(%rdi)     # batt->volts = BATT_VOLTAGE_PORT
      cmpw $3800, %dx
      jge .more3800
      cmpw $3000, %dx
      jl .NOPERCENT
      jge .calculate

.NOPERCENT:
      movw $0,2(%rdi)
      andb $1, %cl          # batt->mode = (BATT_STATUS_PORT & 0x01);
      movb %cl, 3(%rdi)
      movl $0, %eax         # return
      ret
.calculate:
      subw $3000, %dx       # BATT_VOLTAGE_PORT - 3000
      sarw $3, %dx          # EQUAL TO X/3
      movw %dx,2(%rdi)      # move reult to batt.mode
      andb $1, %cl          # batt->mode = (BATT_STATUS_PORT & 0x01);
      movb %cl, 3(%rdi)
      movl $0, %eax         # return
      ret


.more3800:
      movb $100, 2(%rdi)    # batt->percent = 100;
      andb $1, %cl          # batt->mode = (BATT_STATUS_PORT & 0x01);
      movb %cl, 3(%rdi)
      movl $0, %eax          # return
      ret










### Data area associated with the next function
.data
my_int:
        .int 0x00          # initial my_int = 0

my_array:
        .int 0b0111111     # declare multiple ints in a row
        .int 0b0000011
        .int 0b1101101
        .int 0b1100111
        .int 0b1010011
        .int 0b1110110
        .int 0b1111110
        .int 0b0100011
        .int 0b1111111
        .int 0b1110111


.text
.global  set_display_from_batt

## ENTRY POINT FOR REQUIRED FUNCTION
set_display_from_batt:
        ## assembly instructions here
# 8q 4l 2w 1b


# %rax %eax Dividend and quotient
# ebx my_int
# Arg1 %rdi batt_t
# Arg2 %rsi *display
# Arg3 %edx remainder
# Arg4 %ecx  my_int(%rip)
# Arg5 %r8 my_array(%rip)
# 6    r9d   left_digit
# 7    r10d  middle_digit
# 8    r11d  right_digit
#  r13 batt.volt
#  r14 batt.percent
#  r15 batt.mode

	# two useful techniques for this problem
        movl    my_int(%rip),%ebx    # load my_int into register ebx
        leaq    my_array(%rip),%r8  # load pointer to beginning of my_array into r8



        # batt.mode
        movq %rdi, %r14           # copy
        shrq $24, %r14            # move mode in structure to index of 0
        andq $0xFF, %r14
        # batt.percent
        movq %rdi, %r15           # copy
        shrq $16, %r15            # move percent in structure to index of 0
        andq $0xFF, %r15
        # batt.volt
        movq %rdi, %r13           # copy
        andq $0xFFFF, %r13


        cmpb $0, %r14b            # compare 0 and batt.mode
        je .voltMode              # when mode = 0, voltage mode
        jne .percentMode          # when mode = 1(not 0), percent mode

.voltMode:
        movl $1, %r9d        # my_int = (my_int) | (1 << 21);  //. on
        shll $21, %r9d
        orl %r9d, %ebx

        movl $1, %r9d         # my_int = (my_int) | (1 << 22);  //V on
        shll $22, %r9d
        orl %r9d, %ebx

        addl $5, %r13d    # batt.volts += 5;

        movl %r13d, %eax    # calculate  left_digit r9 ,int left_digit = batt.volts  / 1000;
        cqto
        movl $1000, %r9d
        idivl %r9d
        movl %eax, %r9d

        movl %edx, %eax    # calculate  middle_digit r10, int middle_digit = (batt.volts - left_digit * 1000) / 100;
        cqto
        movl $100, %r10d
        idivl %r10d
        movl %eax, %r10d

        movl %edx, %eax     # calculate  right_digit, int right_digit = (batt.volts - middle_digit * 100 - left_digit * 1000) / 10;
        cqto
        movl $10, %r11d
        idivl %r11d
        movl %eax, %r11d

        movl (%r8,%r9,4),%r9d  # my_int = (my_int) | (masks[left_digit] << 14);
        sall $14, %r9d
        orl %r9d, %ebx

        movl (%r8,%r10,4),%r10d # my_int = (my_int) | (masks[middle_digit] << 7);
        sall $7, %r10d
        orl %r10d, %ebx

        movl (%r8,%r11,4),%r11d # my_int = (my_int) | masks[right_digit] ;
        orl %r11d, %ebx

        jmp .showBars



.percentMode:

        movq $1, %r9       # my_int = (my_int) | (1 << 23);  //% on
        shlq $23, %r9
        orl %r9d, %ebx

        movl %r15d, %eax     # calculate  left_digit r9, int left_digit = (batt.percent / 100) % 10;
        cqto
        movl $100, %r9d
        idivl %r9d
        movl %eax, %r9d

        movl %edx, %eax       # calculate  middle_digit  r10, int middle_digit = (batt.percent / 10) % 10;
        cqto
        movl $10, %r10d
        idivl %r10d
        movl %eax, %r10d


        movl %edx, %r11d         # calculate  right_digit r11, int right_digit = (batt.percent) % 10;


        cmpl $0, %r9d            # when percent = 100, which needs denote three digits
        jg .allPercent

        cmpl $0, %r10d           # when percent between 10 to 99, which needs denote two digits
        jg .zeroToFull

        cmpl $0, %r11d          # when percent between 0 to 9, which needs denote one digits
        jge .onlyright


.allPercent:
        movl (%r8,%r9,4),%r9d   # left
        sall $14, %r9d
        orl %r9d, %ebx

        movl (%r8,%r10,4),%r10d      # mid
        sall $7, %r10d
        orl %r10d, %ebx

        movl (%r8,%r11,4),%r11d     # right
        orl %r11d, %ebx

        jmp .showBars

.zeroToFull:

        movl (%r8,%r10,4),%r10d      # mid
        sall $7, %r10d
        orl %r10d, %ebx

        movl (%r8,%r11,4),%r11d     # right
        orl %r11d, %ebx

        jmp .showBars
.onlyright:

        movl (%r8,%r11,4),%r11d     # right
        orl %r11d, %ebx

        jl .barsLess5
        jmp .showBars







# show bars in virual format according to percentage of battery
.showBars:
        cmpl $5, %r15d
        jl .barsLess5

        cmpl $30, %r15d
        jl .barsLess30

        cmpl $50, %r15d
        jl .barsLess50

        cmpl $70, %r15d
        jl .barsLess70

        cmpl $90, %r15d
        jl .barsLess90

        cmpl $100, %r15d
        jle .barsLess100

.barsLess5:                  # when percent less than 5, no bar shows
        movl %ebx,(%rsi)
        movl $0, %eax
        ret

.barsLess30:                 # onr bar showed
        movq $1, %r12        # my_int = (my_int) | (1 << 28);
        shlq $28, %r12
        orl %r12d, %ebx

        movl %ebx,(%rsi)
        movl $0, %eax
        ret

.barsLess50:                 # two bars showed
        movq $1, %r12        # my_int = (my_int) | (1 << 28);
        shlq $28, %r12
        orl %r12d, %ebx

        movq $1, %r12        # my_int = (my_int) | (1 << 27);
        shlq $27, %r12
        orl %r12d, %ebx

        movl %ebx,(%rsi)
        movl $0, %eax
        ret
.barsLess70:                 # three bars showed
        movq $1, %r12        # my_int = (my_int) | (1 << 28);
        shlq $28, %r12
        orl %r12d, %ebx

        movq $1, %r12        # my_int= (my_int) | (1 << 27);
        shlq $27, %r12
        orl %r12d, %ebx

        movq $1, %r12        # my_int = (my_int) | (1 << 26);
        shlq $26, %r12
        orl %r12d, %ebx

        movl %ebx,(%rsi)
        movl $0, %eax
        ret
.barsLess90:                 # four bars showed
        movq $1, %r12        # my_int = (my_int) | (1 << 28);
        shlq $28, %r12
        orl %r12d, %ebx

        movq $1, %r12        # my_int = (my_int) | (1 << 27);
        shlq $27, %r12
        orl %r12d, %ebx

        movq $1, %r12        # my_int = (my_int) | (1 << 26);
        shlq $26, %r12
        orl %r12d, %ebx

        movq $1, %r12        # my_int = (my_int) | (1 << 25);
        shlq $25, %r12
        orl %r12d, %ebx

        movl %ebx,(%rsi)
        movl $0, %eax
        ret

.barsLess100:                # five bars showed
        movq $1, %r12        # my_int = (my_int) | (1 << 28);
        shlq $28, %r12
        orl %r12d, %ebx

        movq $1, %r12        # my_int = (my_int) | (1 << 27);
        shlq $27, %r12
        orl %r12d, %ebx

        movq $1, %r12        # my_int = (my_int) | (1 << 26);
        shlq $26, %r12
        orl %r12d, %ebx

        movq $1, %r12        # my_int = (my_int) | (1 << 25);
        shlq $25, %r12
        orl %r12d, %ebx

        movq $1, %r12        # my_int = (my_int) | (1 << 24);
        shlq $24, %r12
        orl %r12d, %ebx

        movl %ebx,(%rsi)
        movl $0, %eax
        ret





.text
.global batt_update

## ENTRY POINT FOR REQUIRED FUNCTION
batt_update:
	## assembly instructions here

  pushq %rdi                          # pushed onto the stack
  movq %rsp, %rdi                     # move to stack
  call set_batt_from_ports            # call function
  cmpl $1, %eax
  je .ERROR

  movq (%rsp),%rdi
  movq %rsp, %rsi
  call set_display_from_batt
  popq %rdi                           #  restore the stack
  movl %edi, BATT_DISPLAY_PORT(%rip)
  movl $0, %eax                       # return 0
  ret

.ERROR:
  popq %rdi
  movl $1, %eax                       # return 1
  ret

; file ps.as
mcr shawarma
.entry LENGTH
.extern W
MAIN: mov r3 ,LENGTH
      add #45 , r2
R7: sub #-45 , R7
endmcr
shawarma
LOOP: jmp L1(#-1    ,      r6)
      jsr R7(r0,r0)
cmp r6   ,  #-55
prn #-5
not L1
clr STR
lea LENGTH , r2
inc r3
dec r3
red R7
rts
bne W(r4,r5)
sub r1, r4
bne L3
L1: inc K
.entry LOOP
bne LOOP(K,W) 
END: stop
STR: .string "abcdefghijk&^%$#@!&*()_-+=`~1234567890"
LENGTH: .data 6,-9,15 ,    986,+91,    -500
K: .data 22,333
.extern L3
;****************** HW2.s ***************
; Program written: USMAN FROOQI
; Date Created: 8/30/2017 
; Last Modified: 2/1/2018

GPIO_PORTB_DATA_R  EQU   0x400053FC
GPIO_PORTB_DIR_R   EQU   0x40005400
GPIO_PORTB_DEN_R   EQU   0x4000551C
SYSCTL_RCGCGPIO_R  EQU   0x400FE608
    

;-----RAM area----------------        
        THUMB
        AREA    DATA, ALIGN=2
;global variables go here, 
icount  SPACE 2  ;signed
bcount  SPACE 1  ;unsigned
keepOut SPACE 1  ;this location should not be modified
N       SPACE 4
        EXPORT icount
        EXPORT bcount
        EXPORT keepOut
        EXPORT N

;-----ROM area----------------        
    ALIGN
    AREA    |.text|, CODE, READONLY, ALIGN=2

; enable clock for Port B and make PB5 output
; must be friendly, leave over clock bits as they are
; only use registers R0,R1,R2,R3,R12
; system will crash if you turn off Port A clock
    EXPORT  InitPB5
InitPB5
  ; Select regular digital function
  ; Enable Port B clock
    LDR R1, =SYSCTL_RCGCGPIO_R
    LDR R0, [R1]
    ORR R0, R0, #0x02
    STR R0, [R1]
    NOP
    NOP
    

    
  ; Set direction register
    LDR R1, =GPIO_PORTB_DIR_R       ; 5) set direction register
    LDR R0, [R1]
    ORR R0, R0, #0x20                   ; PB5
    STR R0, [R1]


  ; Enable the port digitally
    LDR R1, =GPIO_PORTB_DEN_R       ; set all PF pins to digital
    LDR R0, [R1]
    ORR R0, R0, #0x20                   ; 1 means enable digital I/O
    STR R0, [R1]
    
    
    
    BX  LR
    
; assuming PB5 is output, make PB5 high
; must be friendly
; only use registers R0,R1,R2,R3,R12
    EXPORT  SetPB5
SetPB5
; put solution here
    LDR R1, =GPIO_PORTB_DATA_R
    
    LDR R0, [R1]
    
    MOV R2, #0x20
    
    ORR R0, R2
    
    STRB R0, [R1]
    
    BX  LR

; increment 16-bit signed icount
; only use registers R0,R1,R2,R3,R12
    EXPORT  Inc16
Inc16
; put solution here
    LDR R1, =icount
    
    LDRSH R0, [R1]
    
    ADD R0, #1
    
    LDR R2, =icount
    
    STRH R0, [R2]
    
    
    
    
    BX  LR
 
; decrement 8-bit unsigned bcount
; only use registers R0,R1,R2,R3,R12
    EXPORT  Dec8
Dec8
; put solution here
    LDR R1, =bcount
    
    LDRB R0, [R1]
    
    ADD R0, #-1
    
    LDR R2, =bcount
    
    STRB R0, [R2]
    
    BX  LR
    
; sets bit 3 and clears bit 29 of a 32-bit global variable named N
; be friendly
; only use registers R0,R1,R2,R3,R12
    EXPORT  Set3Clear29
Set3Clear29
; put solution here
    LDR R1, =N
    
    LDR R0, [R1]
    
    ORR R0, R0, #0x8
    
    BIC R0, R0, #0x20000000
      
    STR R0, [R1]
    
    BX  LR 
    
    

    ALIGN        ; make sure the end of this section is aligned
    END          ; end of file
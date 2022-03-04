    LDA #00
    LDX #100
Z_LOOP:
    STA 200,X
    DEX
    BNE Z_LOOP
    STA 200,X
    LDY #01        
S_LOOP:
    CPY #101
    BCS S_DONE
    TYA            
I_LOOP:
    CMP #101
    BCS I_DONE
    TAX            
    INC $200,X     
    STY 01         
    ADC 01
    BCC I_LOOP
I_DONE:
    INY
    BNE S_LOOP
S_DONE:
    LDX #100
C_LOOP:
    LDA $200,X
    AND #$01
    STA $200,X
    DEX
    BNE C_LOOP

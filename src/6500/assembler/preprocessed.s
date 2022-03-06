    LDA #00
    LDX #100
    STA 200,X
    DEX
    BNE 2
    STA 200,X
    LDY #01        
    CPY #101
    BCS 19
    TYA            
    CMP #101
    BCS 17
    TAX            
    INC $200,X     
    STY 01         
    ADC 01
    BCC 10
    INY
    BNE 7
    LDX #100
    LDA $200,X
    AND #$01
    STA $200,X
    DEX
    BNE 20

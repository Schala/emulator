   .CR 6502
   .TF AP1BEER.O,AP1
   .LF AP1BEER.LST
   .OR $0BEE
;-------------------------------------;
; BEER SONG IN 6502 ASSEMBLY LANGUAGE ;
;       BY BARRYM 2010-05-30          ;
; THANKS TO SBPROJECTS.COM FOR LOTS   ;
;   OF VALUABLE INFORMATION AND A     ;
;   VERY NICE ASSEMBLER!              ;
;-------------------------------------;
; THE TARGET MACHINE FOR THIS PROGRAM ;
;   IS THE APPLE 1, BUT IT WOULD BE   ;
;   EASY TO MAKE IT RUN ON OTHER 65XX ;
;   MACHINES BY CHANGING THE NEXT TWO ;
;   EQUATES.  SOME MACHINE-TESTED     ;
;   EXAMPLES:                         ;
;   APPLE II, +, E, C: $FDED, $80     ;
;   COMMODORE 64:      $FFD2, $00     ;
;-------------------------------------;
ECHO     =  $FFEF  ;EMIT A REG AS ASCII
ORMASK   =  $80    ;($00 FOR + ASCII)
;
MAXBEER  =  99     ;INITIAL BEER COUNT
;-------------------------------------;
; X REG. IS THE BOTTLE COUNTER.       ;
; Y REG. IS THE STRING INDEX POINTER, ;
;   AND THE TENS DIGIT IN THE BINARY- ;
;   TO-ASCII CONVERSION ROUTINE.      ;
; A REG. HANDLES EVERYTHING ELSE WITH ;
;   A LITTLE HELP FROM THE STACK.     ;
; ZERO PAGE ISN'T DIRECTLY DISTURBED. ;
;-------------------------------------;
; EMIT COMPLETE CORRECT SONG ADJUSTED ;
;   FOR UPPER-CASE 40-COLUMN DISPLAY. ;
;-------------------------------------;
   LDX #MAXBEER    ;X=MAXBEER
   BNE PRSONG      ;SING THE SONG & RTS
;-------------------------------------;
; EMIT WHOLE SONG UP TO LAST SENTENCE.;
;-------------------------------------;
BEERME:
   LDY #TAKE1-TXT  ;? "TAKE ... AROUND,"
   JSR PRBOB       ;? X;" BOT ... WALL."
PRSONG: ;          ;?
   LDY #CR-TXT     ;? X;" BOT ... WALL,"
   JSR PRBOB       ;? X;" BOT ... BEER."
   DEX             ;X=X-1
   BPL BEERME      ;IF X>=0 THEN BEERME
;-------------------------------------;
; EMIT LAST SENTENCE AND FALL THROUGH.;
;-------------------------------------;
   LDX #MAXBEER    ;X=MAXBEER:
;                  ;? "GO TO ... MORE,"
;-------------------------------------;
; PRINT A PROPERLY PUNCTUATED "BOTTLE ;
;   OF BEER" SENTENCE.                ;
;-------------------------------------;
PRBOB:
   TYA
   PHA             ;SAVE THE PRE$ PTR
   JSR PUTS        ;? PRE$;
   TXA             ;IF X=0 THEN
   BEQ PRBOTT      ;   ? "NO MORE";
   LDY #"0"-1      ;ELSE
   SEC             ;(
DIV10:
   SBC #10         ;   Y=INT(X/10)
   INY
   BCS DIV10
   ADC #10+'0'
   CPY #"0"
   BEQ ONEDIG
   PHA             ;   IF Y>0 THEN
   TYA                   ? Y;
   JSR PUTCH
   PLA             ;   ? X MOD 10;
ONEDIG:
   LDY #BOTTL-TXT  ;)
PRBOTT:
   JSR PUTCH       ;? " BOTTLE";
   CPX #1
   BNE PLURAL
   INY             ;IF X<>1 THEN ? "S";
PLURAL:
   JSR PUTS        ;? " OF BEER";
   PLA             ;RECALL THE PRE$ PTR
   CMP #COMCR-TXT
   BEQ PRDOT
   PHA             ;IF APPROPRIATE THEN
   JSR PUTS        ;   ? " ON THE WALL";
   PLA
   LDY #COMCR-TXT  ;IF APPROPRIATE THEN
   CMP #CR-TXT     ;   ? ",":
   BEQ PRBOB       ;   ? X;" ... BEER";
PRDOT:
   LDY #DOTCR-TXT  ;? "."
;-------------------------------------;
; EMIT A HI-BIT-SET TERMINATED STRING ;
;   @ OFFSET Y AND EXIT WITH Y @ THE  ;
;   BEGINNING OF THE NEXT STRING.     ;
;-------------------------------------;
PUTS:
   LDA TXT,Y       ;GRAB A STRING CHAR
   INY             ;ADVANCE STRING PTR
PUTCH:
   PHA
   ORA #ORMASK
   AND #ORMASK+127 ;FORMAT CHAR FOR ECHO
   JSR ECHO        ;SHOOT IT TO CONSOLE
   PLA
   BPL PUTS        ;LOOP IF APPROPRIATE
   RTS
;-------------------------------------;
; OPTIMIZED SONG LYRIC STRINGS.       ;
;-------------------------------------;
TXT:
TAKE1:
   .AS "TAKE ONE DOWN AND"
   .AS " PASS IT AROUND"
COMCR:
   .AS ","
CR:
   .AT #13
   .AS "NO MORE"
BOTTL:
   .AT " BOTTLE"
   .AT "S OF BEER"
   .AT " ON THE WALL"
DOTCR:
   .AT ".",#13
   .AS "GO TO THE STORE AND"
   .AT " BUY SOME MORE,",#13
   .EN

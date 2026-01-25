.syntax unified
.thumb
.text

.thumb_func
.global Update_Number
Update_Number:
    @ R0 = command (1 for increment, -1 for decrement)
    @ R1 = pointer to digit_tens
    @ R2 = pointer to digit_ones
    @ R3 = pointer to digit_tenths
    
    PUSH    {R4-R7}          @ Save registers

    MOV     R4, R1              @ R4 = &digit_tens
    MOV     R5, R2              @ R5 = &digit_ones
    MOV     R6, R3              @ R6 = &digit_tenths

    @ Load the actual digit values
    LDR    R1, [R4]      @ R1 = digit_tens value
    LDR    R2, [R5]      @ R2 = digit_ones value
    LDR    R3, [R6]      @ R3 = digit_tenths value

    CMP     R0, #0x00000001     @ Check if increment (1)

    BNE     Decrement
    CMP     R3, #9
    BLT     Increment_Tenths
    MOV     R3, #0
check_ones:
    CMP     R2, #9
    BLT     Increment_Ones
    MOV     R2, #0
check_tenths:
    CMP     R1, #9
    BLT     Increment_Tens
    MOV     R1, #0          @ Reset tens digit if overflow
    B       End_Update

Increment_Tenths:
    ADD     R3, #1
    B       End_Update
Increment_Ones:
    ADD     R2, #1
    B       End_Update
Increment_Tens:
    ADD     R1, #1
    B       End_Update

Decrement:
    CMP     R3, #0
    BGT     Decrement_Tenths
    MOV     R3, #9
check_decrement_ones:
    CMP     R2, #0
    BGT     Decrement_Ones
    MOV     R2, #9
check_decrement_tens:
    CMP     R1, #0
    BGT     Decrement_Tens
    BEQ     Prevent_Negative_Tens
    BLT     Loop_Tens_Underflow
Prevent_Negative_Tens:
    MOV     R1, #0          @ Prevent negative tens digit
    B       End_Update
Loop_Tens_Underflow:
    MOV     R1, #9          @ Wrap around tens digit
    B       End_Update
Decrement_Tenths:
    SUB     R3, #1
    B       End_Update
Decrement_Ones:
    SUB     R2, #1
    B       End_Update
Decrement_Tens:
    SUB     R1, #1
End_Update:
    @ Store the updated values back to memory
    STR    R1, [R4]            @ Store digit_tens
    STR    R2, [R5]            @ Store digit_ones
    STR    R3, [R6]            @ Store digit_tenths
    
    POP     {R4-R7}             @ Restore registers
    BX      LR                  @ Return to caller
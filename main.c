#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t A;
    uint8_t B;
    uint8_t C;
    uint8_t D;
    uint8_t E;
    uint8_t F;
    uint8_t G;
} registers;

#define ZERO_FLAG_BYTE_POSITION  7
#define SUBTRACT_FLAG_BYTE_POSITION  6
#define HALF_CARRY_FLAG_BYTE_POSITION  5
#define CARRY_FLAG_BYTE_POSITION  4




typedef struct {
    bool zero;
    bool subtract;
    bool half_carry;
    bool carry;
} flagsRegister;

//Accesses the register struct 
//Turns B into 16 bit int and moves it 8 bits left
//Ors that with transformed 16-bit C which has all its bits on the right
// So 8 left and 8 right combine to 16 bit unsigned int.
uint16_t get_BC(registers* reg){
    return ((uint16_t)reg->B << 8) | (uint16_t)reg->C;
}



//Splits BC into registers B and C
// Typecasts the 16 bit input value and uses a bitmask
//For B it shifts the upper 8 bits to the right
//For C it doesn't need to do that since they are already in lower 8
void set_BC(registers* reg, uint16_t value){
    reg->B = (uint8_t)((value & 0xFF00) >> 8);
    reg->C = (uint8_t)(value & 0XFF);
}

// Checks each flag in the flags register. If they are true
// Then it shifts a 1 by however many digits are specified
// If its a false it shifts a 0
// Then it ORs everything together at the end.
// So we should have the upper 4 bits of an unsigned 8 bit.
uint8_t flags_register_to_u8 (flagsRegister flag){
    return (
        ((flag.zero     ? 1 : 0) << ZERO_FLAG_BYTE_POSITION) |
        ((flag.subtract ? 1 : 0) << SUBTRACT_FLAG_BYTE_POSITION) |
        ((flag.half_carry ? 1 : 0) << HALF_CARRY_FLAG_BYTE_POSITION) |
        ((flag.carry ? 1 : 0) << HALF_CARRY_FLAG_BYTE_POSITION)
    );
}


//Shifts each bit by its corresponding Flag amount, so that it is in the right
//most spot. It then does a 1 bit mask so everything else is set to zero except
// the leftmost bit. It then checks if it is a 1 or a 0.
flagsRegister u8_to_flags_register (uint8_t value){
    flagsRegister flag;

    flag.zero = ((value) >> ZERO_FLAG_BYTE_POSITION & 0b1) != 0;
    flag.subtract = ((value) >> SUBTRACT_FLAG_BYTE_POSITION & 0b1) != 0;
    flag.subtract = ((value) >> SUBTRACT_FLAG_BYTE_POSITION & 0b1) != 0;
    flag.half_carry = ((value) >> HALF_CARRY_FLAG_BYTE_POSITION & 0b1) != 0;
    flag.carry = ((value) >> CARRY_FLAG_BYTE_POSITION & 0b1) != 0;

    return flag;
}

int main(){


return 0;
}
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define ZERO_FLAG_BYTE_POSITION  7
#define SUBTRACT_FLAG_BYTE_POSITION  6
#define HALF_CARRY_FLAG_BYTE_POSITION  5
#define CARRY_FLAG_BYTE_POSITION  4

typedef struct {
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t f;
    uint8_t h;
    uint8_t l;
} registers;

typedef struct {
    bool zero;
    bool subtract;
    bool half_carry;
    bool carry;
} flagsRegister;

//Accesses the register struct 
//Typecasts B into 16 bit int and moves it 8 bits left
//Ors that with transformed 16-bit C which has all its bits on the right
// So 8 left and 8 right combine to 16 bit unsigned int.
uint16_t get_bc(registers* reg){
    return ((uint16_t)reg->b << 8) | (uint16_t)reg->c;
}


//Splits BC into registers B and C
// Typecasts the 16 bit input value and uses a bitmask
//For B it shifts the upper 8 bits to the right
//For C it doesn't need to do that since they are already in lower 8
void set_bc(registers* reg, uint16_t value){
    reg->b = (uint8_t)((value & 0xFF00) >> 8);
    reg->c = (uint8_t)(value & 0XFF);
}

uint16_t get_de(registers* reg){
    return ((uint16_t)reg->d << 8) | (uint16_t)reg->e;
}


void set_de(registers* reg, uint16_t value){
    reg->d = (uint8_t)((value & 0xFF00) >> 8);
    reg->e = (uint8_t)(value & 0XFF);
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




int main(){


return 0;
}

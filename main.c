#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define ZERO_FLAG_BYTE_POSITION  7
#define SUBTRACT_FLAG_BYTE_POSITION  6
#define HALF_CARRY_FLAG_BYTE_POSITION  5
#define CARRY_FLAG_BYTE_POSITION  4

typedef struct {
    bool zero;
    bool subtract;
    bool half_carry;
    bool carry;
} flag_register;

typedef struct {
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    flag_register f;
    uint8_t h;
    uint8_t l;
} registers;



// Checks each flag in the flags register. If they are true
// Then it shifts a 1 by however many digits are specified
// If its a false it shifts a 0
// Then it ORs everything together at the end.
// So we should have the upper 4 bits of an unsigned 8 bit.
uint8_t flag_register_to_u8 (flag_register* flag){
    return (
        ((flag->zero     ? 1 : 0) << ZERO_FLAG_BYTE_POSITION) |
        ((flag->subtract ? 1 : 0) << SUBTRACT_FLAG_BYTE_POSITION) |
        ((flag->half_carry ? 1 : 0) << HALF_CARRY_FLAG_BYTE_POSITION) |
        ((flag->carry ? 1 : 0) << CARRY_FLAG_BYTE_POSITION)
    );
}

void u8_to_flag_register(flag_register* flag, uint8_t byte){
    if (((byte >> ZERO_FLAG_BYTE_POSITION) & 0b1) != 0){
        flag->zero = true;
    }
    else {flag->zero = false;}

    if (((byte >> SUBTRACT_FLAG_BYTE_POSITION) & 0b1) != 0){
        flag->subtract = true;
    }
    else{flag->subtract = false;}

    if (((byte >> HALF_CARRY_FLAG_BYTE_POSITION) & 0b1) != 0){
        flag->half_carry = true;
    }
    else{flag->half_carry = false;}

    if (((byte >> CARRY_FLAG_BYTE_POSITION) & 0b1) != 0){
        flag->carry = true;
    }
    else{flag->carry = false;}
}


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

uint16_t get_hl(registers* reg){
    return ((uint16_t)reg->h << 8) | (uint16_t)reg->l;
}


void set_hl(registers* reg, uint16_t value){
    reg->h = (uint8_t)((value & 0xFF00) >> 8);
    reg->l = (uint8_t)(value & 0XFF);
}

uint16_t get_af(registers* reg){
    uint8_t f = flag_register_to_u8(&reg->f);
    return ((uint16_t)reg->a << 8) | (uint16_t)f;
}


void set_af(registers* reg, uint16_t value){
    reg->a = (uint8_t)((value & 0xFF00) >> 8);
    u8_to_flag_register(&reg->f, value & 0x00FF);
}





int main(){
    registers reg;
    uint16_t value = 0B1111111110100000;
    set_af(&reg, value);
    
    uint16_t ret = get_af(&reg);
    printf("test value %b\n", value);
    printf("af value: %b\n", ret);
    printf("register a: %b\n\n", reg.a);

    printf("flag registers\n");
    printf("zero flag: %d\n",reg.f.zero);
    printf("subtract flag: %d\n",reg.f.subtract);
    printf("half-carry flag: %d\n",reg.f.half_carry);
    printf("zero carry: %d\n",reg.f.carry);


return 0;
}

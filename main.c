#include <stdio.h>
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

typedef enum {
    A,
    B,
    C,
    D,
    E,
    H,
    L,
    HL
} ArithmeticTarget;


typedef enum {
    ADD
} InstructionType;

// Instruction type is just ADD,SUB, MUL, etc
// the union chooses 1 register do the instruction to
typedef struct {
    InstructionType type;
    union {
	    ArithmeticTarget target;
	};
} Instruction;

typedef struct {
    registers Registers;
} CPU;



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

uint8_t add(CPU *cpu, uint8_t value){
    uint16_t result = cpu->Registers.a + value;             //Since we are adding 2 8 bit numbers they could overflow
                                                            //so use store in 16 bit result for safe keeping (:

    //Carry
    if (result > 0xFF){
        cpu->Registers.f.carry = true;
    }
    else {cpu->Registers.f.carry = false;}

    //Half-Carry
    if ((cpu->Registers.a & 0xF)+(value & 0xF) > 0xF){
        cpu->Registers.f.half_carry = true;
    }
    else {cpu->Registers.f.half_carry = false;}


    //Subtract
    cpu->Registers.f.subtract = false;

    //Zero
    if (result == 0){
        cpu->Registers.f.zero = true;
    }
    else {cpu->Registers.f.zero = false;}

    return (uint8_t)result;                                //typecast the 16 bits down to 8 bits, modulos out the overflow
}


void execute(CPU *cpu, Instruction inst){
    uint8_t value;
    uint16_t address;
    uint8_t new_value;
    switch (inst.type) {                                    //What is our instruction?
        case ADD:
            switch (inst.target) {                          //What is our register to add from?
                case A:
                    value = cpu->Registers.a;
                     new_value = add(cpu, value);
                    cpu->Registers.a = new_value;
                    break;
                case B:
                    value = cpu->Registers.b;               //value to pulled from B to add to A
                     new_value = add(cpu, value);           //Add the value to register A
                    cpu->Registers.a = new_value;           //Set A to new value
                    break;
                case C:
                    value = cpu->Registers.c;
                     new_value = add(cpu, value);
                    cpu->Registers.a = new_value;
                    break;
                case D:
                    value = cpu->Registers.d;
                    new_value = add(cpu, value);
                    cpu->Registers.a = new_value;
                    break;
                case E:
                    value = cpu->Registers.e;
                     new_value = add(cpu, value);
                    cpu->Registers.a = new_value;
                    break;
                case H:
                    value = cpu->Registers.h;
                     new_value = add(cpu, value);
                    cpu->Registers.a = new_value;
                    break;
                case L:
                    value = cpu->Registers.l;
                     new_value = add(cpu, value);
                    cpu->Registers.a = new_value;
                    break;
                case HL:
                    uint8_t memory[0xffff];                     // 64 KB memory
                    memory[0xaaaa] = 69;                        // set test value in memory
                    address = get_hl(&cpu->Registers);          //get 16 bit address by combing h and l
                    value = memory[address];                    //use that value as your address in memory
                    new_value = add(cpu, value);                //now just add to a
                    cpu->Registers.a = new_value;
                //TODO: need to add add d8 instruction
                default:
            }
        //Other Instructions ...
        default:
    }
}




int main(){
    Instruction check;
    CPU cpu;
    
    cpu.Registers.a = 0;
    cpu.Registers.h = 0XAA;
    cpu.Registers.l = 0XAA;
    check.type = ADD;
    check.target = HL;
    execute(&cpu, check);

    printf("value in a: %d\n", cpu.Registers.a);

    return 0;
}

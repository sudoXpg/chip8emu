#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<unistd.h>
#include "display.h"

#define START_ADDRESS 0x200;
/*
Memory Map:
+---------------+= 0xFFF (4095) End of Chip-8 RAM
|               |
|               |
|               |
|               |
|               |
| 0x200 to 0xFFF|
|     Chip-8    |
| Program / Data|
|     Space     |
|               |
|               |
|               |
+- - - - - - - -+= 0x600 (1536) Start of ETI 660 Chip-8 programs
|               |
|               |
|               |
+---------------+= 0x200 (512) Start of most Chip-8 programs
| 0x000 to 0x1FF|
| Reserved for  |
|  interpreter  |
+---------------+= 0x000 (0) Start of Chip-8 RAM
*/

/*
Keyboard:
    1	2	3	C
    4	5	6	D
    7	8	9	E
    A	0	B	F
*/

/*
Diaplay:
    (0,0)	(63,0)
    (0,31)	(63,31)
*/

unsigned short opcode;
unsigned char memory[4096];
unsigned char V[16];
unsigned int index_reg;
unsigned int pc;
unsigned char gfx[2048];
unsigned char delay_timer;
unsigned char sound_timer;
unsigned int stack[16];
unsigned short sp;
unsigned char key_board[16];
unsigned char fontset[80];
unsigned char draw_flag;
unsigned char sound_flag;


/*
 * Memory map:
 *
 * Total: 4096 bytes (4K)
 *
 * 0x000 - 0x1FF     INTERPRETER
 * 0x200 - 0xFFF     Program/Data space
 *
 * 
 * opcodes are stored big-endian!
 *
 * 
 */


void init_cpu(){
    tick();
    printf("cpu init bein . . .\n");
    // Font set >> devernay.free.fr/hacks/chip8/C8TECH10.HTM#0.1
    unsigned char fontset[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
        0x20, 0x60, 0x20, 0x20, 0x70,  // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
        0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
        0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
        0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
        0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
        0xF0, 0x80, 0xF0, 0x80, 0x80   // F
    };

    memset(memory,0,sizeof(memory));                          // initialise memory
    memset(V,0,sizeof(V));                                    // initialise registers 
    index_reg=0;                                              // initialise index_reg register
    pc=START_ADDRESS;                                         // initialise PC at 0x200
    sp=0;                                                     // initialsie SP at 0x00
    memset(stack,0,sizeof(stack));                            // initialise stack 
    memset(key_board,0,sizeof(key_board));                    // initialise keyboard
    memset(gfx,0,sizeof(gfx));                                // initialise

    // set timers/flags
    delay_timer=0;
    sound_timer=0;
    sound_flag = 0;
    draw_flag = 0;

    memcpy(memory,fontset,sizeof(fontset));     // copy the fonts into memory
    tick();
    printf("cpu init end . . .\n");
}

int load_rom(char *rom){
    FILE *fp = fopen(rom,"rb");
    if(!fp){
        fprintf(stderr,"Error in opening %s\n",rom);
        return -1;
    }

    struct stat file_nfo;                                   // struct for file information
    stat(rom,&file_nfo);                                    // populate the struct
    size_t file_size=file_nfo.st_size;                      // get the file size

    size_t bytes_read=fread(memory+0x200,1,sizeof(memory)-0x200,fp);    // copy the contents from the rom onto the memory
    fclose(fp);

    if(bytes_read!=file_size){
        fprintf(stderr,"read error\n");
        return -1;
    }

    return 0;
}

void cpu_cycle(){
    draw_flag=0;
    sound_flag=0;

    opcode=memory[pc]<<8|memory[pc+1];
    unsigned short x = (opcode & 0x0F00) >> 8;              // get Vx reg from the opcode [()XNN ] 
    unsigned short y = (opcode & 0x00F0) >> 4;              // get Vy reg from the opcode [()NYN ]
    switch(opcode & 0xF000) {
    case 0x0000:
        switch (opcode & 0x00FF) {
            case 0x00E0:
                memset(gfx, 0, sizeof(gfx));
                printf("CLS\n");
                pc += 2;
                break;
            case 0x00EE:
                pc = stack[sp];
                sp--;
                printf("RET\n");
                pc += 2;
                break;
            default:
                printf("Invalid opcode! 0x0aaa\n");
                break;
        }
        break;

    case 0x1000:
        pc = opcode & 0x0FFF;
        printf("JP\n");
        break;

    case 0x2000:
        sp++;
        stack[sp] = pc;
        pc = opcode & 0x0FFF;
        printf("CALL\n");
        break;
        
    case 0x3000:
        if (V[x] == (opcode & 0x00FF)) {
            pc += 2;
            printf("SE\n");
        }
        pc += 2;
        break;

    case 0x4000:
        if (V[x] != (opcode & 0x00FF)) {
            pc += 2;
            printf("SNE\n");
        }
        pc += 2;
        break;

    case 0x5000:
        if (V[x] == V[y]) {
            pc += 2;
            printf("SE\n");
        }
        pc += 2;
        break;

    case 0x6000:
        V[x] = (opcode & 0x00FF);
        printf("LD\n");
        pc += 2;
        break;

    case 0x7000:
        V[x] += (opcode & 0x00FF);
        printf("ADD\n");
        pc += 2;
        break;

    case 0x8000:
        switch (opcode & 0x000F) {
            case 0x0000:
                V[x] = V[y];
                printf("LD\n");
                pc += 2;
                break;
            case 0x0001:
                V[x] = (V[x] | V[y]);
                printf("OR\n");
                pc += 2;
                break;
            case 0x0002:
                V[x] = (V[x] & V[y]);
                printf("AND\n");
                pc += 2;
                break;
            case 0x0003:
                V[x] = (V[x] ^ V[y]);
                printf("XOR\n");
                pc += 2;
                break;
            case 0x0004:
                V[x] = (V[x] + V[y]);
                printf("ADD\n");
                pc += 2;
                break;
            case 0x0005:
                V[0xF] = (V[x] > V[y]) ? 1 : 0;
                V[x] = (V[x] - V[y]);
                printf("SUB\n");
                pc += 2;
                break;
            case 0x0006:
                V[0xF] = V[x] & 0x0F;
                V[x] = (V[x] >> 1);
                printf("SHR\n");
                pc += 2;
                break;
            case 0x0007:
                V[0xF] = (V[y] > V[x]) ? 1 : 0;
                V[x] = (V[y] - V[x]);
                printf("SUBN\n");
                pc += 2;
                break;
            case 0x000E:
                V[0xF] = (V[x] >> 7) & 0x1;
                V[x] = (V[x] << 1);
                printf("SHL\n");
                pc += 2;
                break;
            default:
                printf("Invalid opcode! 0x8aaa \n");
                break;
        }
        break;

    case 0x9000:
        if (V[x] != V[y]) {
            pc += 2;
            printf("SNE\n");
        }
        pc += 2;
        break;

    case 0xA000:
        index_reg = opcode & 0x0FFF;
        printf("LD\n");
        pc += 2;
        break;

    case 0xB000:
        pc = (opcode & 0x0FFF) + V[0];
        printf("JP\n");
        pc += 2;
        break;

    case 0xC000:
        V[x] = (rand() % 256) & (opcode & 0x00FF);
        printf("RND\n");
        pc += 2;
        break;

    case 0xD000:
            draw_flag = 1;
            unsigned short height = opcode & 0x000F;
            unsigned short px;
            V[0xF] = 0;
            for (int yline = 0; yline < height; yline++) {
                px = memory[index_reg + yline];
                for (int xline = 0; xline < PIXEL_SIZE; xline++) {
                    if ((px & (0x80 >> xline)) != 0) {
                        if (gfx[(V[x] + xline + ((V[y] + yline) * 64))] ==
                            1) {
                            V[0xF] = 1;
                        }
                        gfx[V[x] + xline + ((V[y] + yline) * 64)] ^= 1;
                    }
                }
            }
            pc += 2;
            break;

    case 0xE000:
        switch (opcode & 0x00FF) {
            case 0x009E:
                if (key_board[V[x]]) {
                    pc += 2;
                    printf("SKP\n");
                }
                pc += 2;
                break;
            case 0x00A1:
                if (!key_board[V[x]]) {
                    pc += 2;
                    printf("SKNP\n");
                }
                pc += 2;
                break;
        }
        break;

    case 0xF000:
        switch (opcode & 0x00FF) {
            case 0x0007:
                V[x] = delay_timer;
                printf("LD\n");
                pc += 2;
                break;

            case 0x000A:
                for (int i = 0; i < sizeof(key_board); i++) {
                    if (key_board[i]) {
                        V[x] = i;
                        pc += 2;
                        printf("LD\n");
                        break;
                    }
                }
                break;

            case 0x0015:
                delay_timer = V[x];
                printf("LD DT\n");
                pc += 2;
                break;

            case 0x0018:
                sound_timer = V[x];
                printf("LD ST\n");
                pc += 2;
                break;

            case 0x001E:
                index_reg += V[x];
                printf("ADD index_reg\n");
                pc += 2;
                break;

            case 0x0029:
                index_reg = V[x] * 5;
                printf("LD F\n");
                pc += 2;
                break;

            case 0x0033:
                memory[index_reg] = (V[x] / 100);
                memory[index_reg + 1] = (V[x] % 100) / 10;
                memory[index_reg + 2] = (V[x] % 10);
                printf("LD B\n");
                pc += 2;
                break;

            case 0x0055:
                for (int i = 0; i < sizeof(V); i++) {
                    memory[i + index_reg] = V[i];
                }
                printf("LD\n");
                pc += 2;
                break;

            case 0x0065:
                for (int i = 0; i < sizeof(V); i++) {
                    V[i] = memory[i + index_reg];
                }
                printf("LD\n");
                pc += 2;
                break;

            default:
                printf("Invalid opcode! 0xFxxx\n");
                break;
        }
        break;

    default:
        printf("Invalid opcode!\n");
        break;
}

    // update timers
    if(delay_timer>0) delay_timer--;
    if(sound_timer>0){
        sound_flag=1;
        if(sound_timer==1){
            printf("beep\n");
            play_beep();    
        }
        sound_timer--;
    }
    return;
}
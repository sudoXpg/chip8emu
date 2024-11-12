#include "lib/cpu.h"

int main(int argc, char *argv[]){
    // Check if the correct number of arguments are passed (program name and ROM file)
    if(argc != 2){
        fprintf(stderr, "usage: ./cpu rom.ch8\n");
        return 1;
    }

    tick();  
    printf("ROM : %s entered\n", argv[1]);  // Print the name of the ROM file entered as argument
    // Initialize the CPU (set registers and memory)

    init_cpu();  
    tick();
    printf("PC set at address: %x\n", pc);  // Print the initial Program Counter (PC) address, typically 0x200 for CHIP-8

    // Load the ROM file into memory, check for success or failure
    if(load_rom(argv[1]) < 0){
        return 1;
    }

    // Initialize the display and audio system (using SDL)
    init_disp();

    // Main loop for CPU execution and rendering
    while (!end) {
        cpu_cycle(); 
        sdl_ehandler(key_board);
        if (draw_flag) {
            draw(gfx);  // Draw the current screen state (based on the gfx array)
        }
        // Sleep for a small period to regulate the speed of the emulation
        usleep(2000);  
    }
    // program halt, close SDL
    stop_disp();
    return 0;
}

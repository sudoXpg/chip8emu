#ifndef SCREEN_H
#define SCREEN_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_mixer.h>

void tick(){
    printf("\xE2\x9C\x93");
}


// Beep flag to indicate when to play the beep sound
int play_beep_flag = 0;

// SDL window and renderer
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

// Beep sound
Mix_Chunk* beep = NULL;

// Audio thread for playing sound
SDL_Thread* audio_thread = NULL;


#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define PIXEL_SIZE 8

// Key mappings for CHIP-8 emulator
SDL_Scancode keymappings[16] = {
    SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3, SDL_SCANCODE_4,
    SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E, SDL_SCANCODE_R,
    SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_F,
    SDL_SCANCODE_Z, SDL_SCANCODE_X, SDL_SCANCODE_C, SDL_SCANCODE_V
};

int end = 0;

// Audio thread function to handle beep sound playback
int audio_thread_func(void* data) {
    while (1) {
        if (play_beep_flag) {
            // Play the beep sound if the flag is set
            if (Mix_PlayChannel(-1, beep, 0) == -1) {
                fprintf(stderr, "Failed to play beep sound: %s\n", Mix_GetError());
            }
            SDL_Delay(500);  // Wait for the sound to finish playing (adjust the delay as needed)
            play_beep_flag = 0;  // Reset the flag after the sound has played
        }
        SDL_Delay(10);  // Small delay to prevent tight loop
    }
}

// Initialize SDL and display components
void init_disp() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        exit(1);
    }

    // Create the window
    window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                              SCREEN_WIDTH * PIXEL_SIZE, SCREEN_HEIGHT * PIXEL_SIZE, 0);
    if (window == NULL) {
        fprintf(stderr, "Window could not be created! SDL Error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    // Create the renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        fprintf(stderr, "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }

    // Initialize SDL_mixer for audio
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }

    // Load the beep sound
    beep = Mix_LoadWAV("lib/beep.wav");
    if (beep == NULL) {
        fprintf(stderr, "Failed to load beep sound! SDL_mixer Error: %s\n", Mix_GetError());
        Mix_CloseAudio();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }

    // Create the audio thread
    audio_thread = SDL_CreateThread(audio_thread_func, "AudioThread", NULL);
    if (audio_thread == NULL) {
        fprintf(stderr, "Unable to create audio thread! SDL Error: %s\n", SDL_GetError());
        Mix_FreeChunk(beep);
        Mix_CloseAudio();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }
}

// Request to play the beep sound
void play_beep() {
    play_beep_flag = 1;
}

// Render the display (screen) based on the current state of the display buffer
void draw(const char *display) {
    // Clear the screen with black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
    SDL_RenderClear(renderer);

    // Set the draw color to white for the pixels
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // Loop through each pixel and draw it if it is set in the display buffer
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            if (display[x + (y * SCREEN_WIDTH)]) {
                SDL_Rect rect;
                rect.x = x * PIXEL_SIZE;
                rect.y = y * PIXEL_SIZE;
                rect.w = PIXEL_SIZE;
                rect.h = PIXEL_SIZE;

                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    // Update the screen with the new render
    SDL_RenderPresent(renderer);
}

// Handle SDL events (user input, window events)
void sdl_ehandler(unsigned char* keypad) {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        const Uint8* state = SDL_GetKeyboardState(NULL);
        switch (event.type) {
            case SDL_QUIT:
                end = 1;
                break;
            default:
                if (state[SDL_SCANCODE_ESCAPE]) {
                    end = 1;
                }

                // Update the keypad state based on the current key states
                for (int keycode = 0; keycode < 16; keycode++) {
                    keypad[keycode] = state[keymappings[keycode]];
                }
                break;
        }
    }
}

// Clean up and free resources
void stop_disp() {
    // Free the beep sound
    if (beep != NULL) {
        Mix_FreeChunk(beep);
    }

    // Close SDL_mixer and clean up SDL
    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

#endif

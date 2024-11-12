all:
	gcc -o main main.c `sdl2-config --cflags --libs` -lSDL2 -lSDL2_mixer
	./main ROMS/LANDING.ch8

clean:
	rm main
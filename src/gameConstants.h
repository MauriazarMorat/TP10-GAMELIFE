#ifndef GAMECONSTANTS_H
#define GAMECONSTANTS_H

#include <stdint.h>

#define FPS 60.0f
#define DELTA_TIME 1.0f/FPS // Similar a como motores de video juegos como Godot manejan el tiempo.

#define C_alto  50 // Cuantas celulas de alto
#define C_ancho 50 // Cuantas celulas de largo

#define BACKGROUND_COLOR al_map_rgb(60, 62, 66)

typedef struct gameStatus{
	uint8_t isRunning : 1;
	uint8_t isEditing : 1;
	uint8_t isPaused : 1;
	uint8_t redraw : 1;
	uint8_t nextGen : 1;
	// espacio para flags necesarios.
	uint16_t generacion;
	int world[C_alto][C_ancho]; // Creamos la matriz mundo, que sera la principal.
	int auxWorld[C_alto][C_ancho]; // Creamos una matriz auxiliar que guardara la generacion 0
} GAME_STATUS;

#endif

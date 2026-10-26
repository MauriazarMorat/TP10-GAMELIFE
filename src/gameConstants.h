#ifndef GAMECONSTANTS_H
#define GAMECONSTANTS_H

#include <stdint.h>

#define FPS 60.0f
#define DELTA_TIME 1.0f/FPS // Similar a como motores de video juegos como Godot manejan el tiempo.

#define VELOCIDAD_INICIAL 0.5 //Elegimos la velocidad inicial
#define MIN_VEL 1.0 //Definimos el minimo
#define CAMBIO_DE_VELOCIDAD VELOCIDAD_INICIAL/10 //Cuanto queremos variar cada cambio de velocidad
#define MEDIAVELOCIDAD 100/(MIN_VEL/2) //Esto va a ser el 100%

#define C_alto  50 // Cuantas celulas de alto
#define C_ancho 50 // Cuantas celulas de largo

// Constantes colores.
#define BACKGROUND_COLOR al_map_rgb(24, 27, 33)
#define BACKGROUND_COLOR al_map_rgb(24, 27, 33)
#define COLOR_CELULA_MUERTA al_map_rgb(39, 44, 52)
#define COLOR_CELULA_VIVA al_map_rgb(80, 220, 150)
#define COLOR_TEXTO al_map_rgb(230, 233, 238)

#define SIZE_FUENTE 12

// Constantes intro.
#define INTRO_ANCHO 600
#define INTRO_ALTO 400
#define DURACION_FADE_IN 4.0
#define DURACION_VISIBLE 2.0
#define DURACION_FADE_OUT 5.0
#define DURACION_TOTAL (DURACION_FADE_IN + DURACION_VISIBLE + DURACION_FADE_OUT)

typedef struct gameStatus{
	uint8_t isRunning : 1;
	uint8_t isEditing : 1;
	uint8_t isPaused : 1;
	uint8_t redraw : 1;
	uint8_t isHelp : 1;
	uint8_t isHelpMenuBlenderReady : 1; //Es un flag para confirmar si se aplico la opacidad en el menu help
	// espacio para flags necesarios.
	float umbralNextGen;
	uint16_t generacion;
	int world[C_alto][C_ancho]; // Creamos la matriz mundo, que sera la principal.
	int auxWorld[C_alto][C_ancho]; // Creamos una matriz auxiliar que guardara la generacion 0
} GAME_STATUS;

#endif

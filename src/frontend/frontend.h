#ifndef FRONT_H
#define FRONT_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

#include "../backend/backend.h"
#include "../gameConstants.h"

#define CELL_SIZE 10// Pixeles de ancho de cada celula
#define CELL_MARGIN 5 // Margen de pixeles de cada celula
#define PantallaMargin 20 // Pixeles de margen
#define PantallaTextMargin 50 // Pixeles inferiores de margen

#define alto C_alto*CELL_SIZE + (C_alto-1)*CELL_MARGIN + 2*PantallaMargin + PantallaTextMargin // Alto total de pantalla
#define largo C_ancho*CELL_SIZE + (C_ancho-1)*CELL_MARGIN + 2*PantallaMargin // Ancho total de pantalla.

typedef struct {
	int x;
	int y;
} position_t; // Una estructura usada para pasar informacion de posicion bidimensional.

int init_front(void); // Inicializa todos lo necesario del front.

// FRONT GRAFICO

void mainDraw(ALLEGRO_FONT *font, GAME_STATUS * gameStatus); // Ejecuta el buffer principal de dibujo.
void pixelCoordToWorldCoord(int posX, int posY, position_t * worldPos); // Devuelve la posicion del mundo dada un posicion pixel.
void helpDraw(ALLEGRO_FONT *font,GAME_STATUS * gameStatusS);

// FRONT INPUT
void manejar_teclado(ALLEGRO_EVENT ev, GAME_STATUS*gameStatus); // Encargada de manejar todo el input del teclado.

// FRONT TIMER
void manejar_timer(ALLEGRO_EVENT ev, GAME_STATUS*gameStatus, double*nextGenCounter); // Encargada de manejar timer del programa.

// FRONT MOUSE
void manejar_mouse(ALLEGRO_EVENT ev, GAME_STATUS*gameStatus); // Funcion encargada de manejar el timer.
#endif

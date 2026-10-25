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

#define alto C_alto*CELL_SIZE + (C_alto-1)*CELL_MARGIN + 2*PantallaMargin + PantallaTextMargin
#define largo C_ancho*CELL_SIZE + (C_ancho-1)*CELL_MARGIN + 2*PantallaMargin
#define OUTOFMAT -1 // DEBE SER UN NUMERO NEGATIVO SI O SI

typedef struct {
	int x;
	int y;
} position_t;

int init_front(void);
void dibujarCelulas(int mundo[C_alto][C_ancho]);
int POStoCELL(int posicion);
void mainDraw(ALLEGRO_FONT *font, int generacionActual, int timerStatus);
void pixelCoordToWorldCoord(int posX, int posY, position_t * worldPos);

#endif

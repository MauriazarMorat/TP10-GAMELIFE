

#ifndef FRONT_H
#define FRONT_H

#include "back.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include "constant.h"

#define C_CellAncho 10 // Pixeles de ancho de cada celula
#define C_CellMargin 5 // Margen de pixeles de cada celula
#define PantallaMargin 20 // Pixeles de margen
#define PantallaTextMargin 50 // Pixeles inferiores de margen

#define alto C_alto*C_CellAncho + (C_alto-1)*C_CellMargin + 2*PantallaMargin + PantallaTextMargin
#define largo C_ancho*C_CellAncho + (C_ancho-1)*C_CellMargin + 2*PantallaMargin
#define OUTOFMAT -1 // DEBE SER UN NUMERO NEGATIVO SI O SI

typedef struct {
	int x;
	int y;
} celula_t;

int inicializaciones(void);
void dibujarcelulas(int mundo[C_alto][C_ancho]);
int POStoCELL(int posicion);
void MainDraw(ALLEGRO_FONT *font);

#endif

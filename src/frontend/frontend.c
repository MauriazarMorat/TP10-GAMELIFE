// ----------- LIBRERIAS ---------- //

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

#include "frontend.h"

// ---------- FUNCIONES ---------- //

int init_front(void){
	if(!al_init()) 
	{
		fprintf(stderr, "Error al inicializar Allegro.\n");
		return 0;
	}
	if(!al_init_primitives_addon())
	{
		fprintf(stderr, "Error al inicializar Allegro primitives addon.\n");
		return 0;
	}
	if(!al_install_mouse())
	{
	    fprintf(stderr, "Error al inicializar el mouse.\n");
	    return 0;
	}
	if(!al_install_keyboard())
	{
	    fprintf(stderr, "Error al inicializar el teclado.\n");
	    return 0;
	}
	if(!al_init_font_addon())
	{
	    fprintf(stderr, "Error al inicializar el font addon.\n");
	    return 0;
	}

	return 1;
}

void dibujarCelulas(int mundo[C_alto][C_ancho])
{
	ALLEGRO_COLOR color;
	int cursorX;
	int cursorY = PantallaMargin;
	for (int y=0 ; y<C_alto ; y++)
	{
		cursorX = PantallaMargin;
		for (int x=0; x<C_ancho ; x++)
		{
			if (mundo[y][x])
			{
				color = al_map_rgb(0,0,0);
			} else
			{
				color = al_map_rgb(255,255,255);
			}
			al_draw_filled_rectangle(cursorX, cursorY, cursorX+CELL_SIZE, cursorY+CELL_SIZE, color);
			cursorX += CELL_SIZE + CELL_MARGIN;
		} // Dibujo una row
		cursorY+= CELL_SIZE + CELL_MARGIN;
	}
}

void mainDraw(ALLEGRO_FONT *font, int generacionActual, int timerStatus)
{
	al_clear_to_color(al_map_rgb(60, 62, 66));
	al_draw_textf(font,al_map_rgb(255, 255, 255),
	PantallaMargin,
					largo-10,                   // Coordenadas X e Y en pantalla
	            0,"Generacion actual: %d Timer Status: %d",generacionActual,timerStatus
	);
}

// Devuelve la posicion del mundo en base a la posicion de pixeles dada.
void pixelCoordToWorldCoord(int posX, int posY, position_t * worldPos)
{
	// Validamos si la posicion pasada esta fuera del mundo graficamente.
	if(posX < PantallaMargin || posX > (PantallaMargin+((CELL_SIZE+CELL_MARGIN)*C_ancho)))
	{
		printf("Outside the bounds of map");
		worldPos->x = -1; // retornamos un valor imposible, recordar que trabajamos en el semieje x positivo.
		return;
	}
	if(posY < PantallaMargin || posY > (PantallaMargin+((CELL_SIZE+CELL_MARGIN)*C_alto)))
	{
		printf("Outside the bounds of map");
		worldPos->y = -1; // retornamos un valor imposible, recordar que trabajamos en el semieje y positivo.
		return;
	}

	/* 	Traducimos la cordenadas de la pantalla al mundo de la celula, notese que restamos el margin,
	* 	eso haria que comencemos en (0,0) de nuestro mundo, y tratamos una celula como su SIZE mas tu MARGIN.
	*/
	worldPos->x = (posX - PantallaMargin) / (CELL_SIZE+CELL_MARGIN);
	worldPos->y = (posY - PantallaMargin) / (CELL_SIZE+CELL_MARGIN);
}

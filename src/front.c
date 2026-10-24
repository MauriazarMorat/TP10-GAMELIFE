// ----------- LIBRERIAS ---------- //

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include "front.h"

// ---------- FUNCIONES ---------- //

int inicializaciones(void){
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

void dibujarcelulas(int mundo[C_alto][C_ancho])
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
			al_draw_filled_rectangle(cursorX, cursorY, cursorX+C_CellAncho, cursorY+C_CellAncho, color);
			cursorX += C_CellAncho + C_CellMargin;

		} // Dibujo una row
		cursorY+= C_CellAncho + C_CellMargin;
	}
}

void MainDraw(ALLEGRO_FONT *font)
{
	al_draw_text(font,al_map_rgb(255, 255, 255),
	PantallaMargin,
					largo-10,                   // Coordenadas X e Y en pantalla
	            0,"Para Avanzar 1 generacion presiona enter"
	);
	al_flip_display();
}

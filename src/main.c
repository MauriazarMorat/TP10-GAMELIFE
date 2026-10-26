// ---------- LIBRERIAS ---------- //

#include <stdio.h>
#include "backend/backend.h"
#include "frontend/frontend.h"
#include "stdbool.h"

// ------------ MAIN ------------- //

int main (void)
{
	// inicializamos nuestro estado de juego.
	GAME_STATUS gameStatus = {
			.isRunning = 1,
			.isEditing = 0,
			.redraw = 1,
			.generacion = 0,
			.isPaused = 1,
			.isHelp = 0,
			.isHelpMenuBlenderReady = 0,
			.umbralNextGen = VELOCIDAD_INICIAL,
	};

	// Rellenamos con cero las matrices del mundo.
	rellenar_mundo(gameStatus.world);
	rellenar_mundo(gameStatus.auxWorld);

	if(!init_front())
    {
		return -1; // Alguna inicilizacion grafica fallo.
	}
	else
    {
		printf("Se ha inicializado ALLEGRO correctamente.\n");
	}

	/* 
	Mostramos la intro, la cual se trata de un homenaje a la iconica presentación 
	de la desarrolladora de videojuegos Konami en el clasico de Playstation, Metal Gear Solid.
	La frase mostrada hace referencia Interplay, empresa detrás de los inicios franquicias como Fallout.
	Finalmente el nombre del equipo mostrado, es el que usaremos para competir en la competencia IEEE EXTREME.
	*/
	if (!mostrar_intro())
	{
		return 0;
	}

	// Creamos el event queue
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	ALLEGRO_FONT *font = al_load_ttf_font(
    "assets/fuente.otf",
    SIZE_FUENTE,
    0
);
	ALLEGRO_DISPLAY *display = al_create_display(largo,alto);
	ALLEGRO_TIMER *timer = al_create_timer(DELTA_TIME);

	// Creamos counters del programa.
	double nextGenCounter = 0.0f; // Contador para saber cuanto tiempo paso desde la anterior pasada de generacion.

	if(event_queue == NULL || display == NULL || font == NULL || timer == NULL )
    {
		fprintf(stderr, "El siguiente error fatal ha ocurrido.\n");
		al_destroy_event_queue(event_queue);
		return -1;
	}

	ALLEGRO_BITMAP *icono = al_load_bitmap("assets/icono.png");

	if (icono)
	{
		al_set_display_icon(display, icono);
		al_destroy_bitmap(icono);
	}
	
	al_set_window_title(display, "Juego de la Vida");

	// Configuramos eventos
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

    al_start_timer(timer);

    mainDraw(font, &gameStatus);

    /* Bucle principal del programa. */

    ALLEGRO_EVENT ev;

    while (gameStatus.isRunning) // Si el usuario ingresa la letra 'q' termina el programa.
    {
    	al_wait_for_event(event_queue, &ev); // Esperamos algun evento

    	switch(ev.type)
    	{
    		case ALLEGRO_EVENT_DISPLAY_CLOSE:
    			gameStatus.isRunning = false;
    			break;
    		case ALLEGRO_EVENT_TIMER:
    			manejar_timer(ev, &gameStatus, &nextGenCounter);
    			break;
    		case ALLEGRO_EVENT_KEY_DOWN:
    			manejar_teclado(ev, &gameStatus);
    			break;
    		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
    			manejar_mouse(ev, &gameStatus);
    			break;
    	}
    	if (gameStatus.redraw && al_is_event_queue_empty(event_queue))
		{
			if (gameStatus.isHelp)
			{
				helpDraw(font, &gameStatus);
			}
			else
			{
				mainDraw(font, &gameStatus);
			}

			gameStatus.redraw = false;
		}
    }
    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    return 0;
}

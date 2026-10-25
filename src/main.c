// ---------- LIBRERIAS ---------- //

#include <stdio.h>

#include "backend/backend.h"
#include "frontend/frontend.h"

// ------------ MAIN ------------- //

int main (void)
{
	int mundo[C_alto][C_ancho]; // Creamos la matriz mundo, que sera la principal.
	int mundoInicial[C_alto][C_ancho]; // Creamos una matriz auxiliar que guardara la generacion 0

	rellenar_mundo(mundo);
	rellenar_mundo(mundoInicial);

	if(!init_front())
    {
		return -1; // Alguna inicilizacion grafica fallo.
	}
	else
    {
		printf("Se ha inicializado ALLEGRO correctamente.\n");
	}

	// Creamos el event queue

	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();

	if(!event_queue) 
    {
		printf("FATAL ERROR: ");
		fprintf(stderr, "Error al crear la cola de eventos.\n");
		return -1;
	}

	// Creamos el display y fuentes

	ALLEGRO_FONT *font = al_create_builtin_font();
	ALLEGRO_DISPLAY *display = al_create_display(largo,alto);
	ALLEGRO_TIMER *timer = al_create_timer(DELTA_TIME);
	double timeCounter = 0.0;

	if(display == NULL)
    {
		printf("FATAL ERROR: ");
		fprintf(stderr, "Error al crear el display.\n");
		al_destroy_event_queue(event_queue);
		return -1;
	}
	if(font == NULL){
		printf("FATAL ERROR: ");
		fprintf(stderr, "Error al crear font.\n");
		al_destroy_event_queue(event_queue);
		return -1;
	}
	if(timer == NULL){
		printf("FATAL ERROR: ");
		fprintf(stderr, "Error al crear el timer.\n");
		al_destroy_event_queue(event_queue);
		return -1;
	}

	// Configuramos eventos
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));


	bool running = true;
	bool redibujar = true;
    int generacion = 0;

    /* Coloca un glider en el mundo.*/

    copiar_mundo(mundo, mundoInicial);
    mainDraw(font, generacion, 0);

    /* Bucle principal del programa. */

    ALLEGRO_EVENT ev;

    while (running) // Si el usuario ingresa la letra 'q' termina el programa.
    {

    	al_wait_for_event(event_queue, &ev); // Esperamos algun evento

    	// Esto pausa el programa hasta que, se mueva el mouse, se toca una tecla..

    	if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) 
        {
    		running = false;
    	}
    	if(ev.type == ALLEGRO_EVENT_TIMER)
    	{
    		timeCounter += DELTA_TIME;

    		if(timeCounter >= 0.25f) {
    			printf("un segundo paso!\n");
    			siguiente_generacion(mundo, &generacion);
    			timeCounter -= 0.25;
    		}

    		redibujar = true;
    	}
    	if (ev.type == ALLEGRO_EVENT_KEY_DOWN) // Se presionó una tecla
        {
    		if (ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE) // Si la tecla es escape.
            {
    			running = false;
    		}
    		else if(ev.keyboard.keycode == ALLEGRO_KEY_T){
    			printf("Siguiente generacion!\n");
    			siguiente_generacion(mundo, &generacion);
    			redibujar = true;
    		}
    		else if(ev.keyboard.keycode == ALLEGRO_KEY_SPACE){
    			if(al_get_timer_started(timer)){
    				al_stop_timer(timer);
    			} else {
    				al_resume_timer(timer);
    			}
    			redibujar = true;
    		}
    	}
    	if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
    		int mousePosX = ev.mouse.x;
    		int mousePosY = ev.mouse.y;


    		// definimos la posicion que deseamos buscar.
    		position_t posCell;

    		pixelCoordToWorldCoord(mousePosY, mousePosX, &posCell);

    		// validamos si la posicion obtenida es valida.
    		if(posCell.x != -1 && posCell.y != -1){
    			// Si lo es, entonces a la posicion del mundo asignamos el estado de vida de una celula.
    			mundo[posCell.x][posCell.y] = !mundo[posCell.x][posCell.y];

    			generacion = 0;
    			redibujar = true;
    		}

    		printf("Mouse X coords: %d posCell X coords: %d\n", mousePosX, posCell.y);
    		printf("Mouse Y coords: %d posCell Y coords: %d\n", mousePosY, posCell.x);
    	}


    	if (redibujar && al_is_event_queue_empty(event_queue))
        {
    		mainDraw(font, generacion, al_get_timer_started(timer));
    		dibujarCelulas(mundo);
    		al_flip_display();
    		redibujar = false;
    	}
    }

    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    return 0;
}

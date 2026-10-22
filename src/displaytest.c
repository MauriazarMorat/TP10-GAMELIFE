#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

int main (void){
	printf("Iniciamos La prueba de allegro!\n");
	//Vemos si esta todo en orden
	if(!al_init()) {
	    fprintf(stderr, "Error al inicializar Allegro.\n");
	    return -1;
	}

	if(!al_init_primitives_addon()){
		fprintf(stderr, "Error al inicializar Allegro primitives addon.\n");
		return -1;
	}

	printf("Allegro iniciado correctamente!\n");

	//CREAMOS UNA COLA DE EVENTOS, un lugar para que se
	//acumulen los eventos
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	if(!event_queue) {
	    fprintf(stderr, "Error al crear la cola de eventos.\n");
	    return -1;
	}
	printf("La cola de eventos a sido creada, creamo ventana\n");

	//CREAMOS LA VENTANA
	ALLEGRO_DISPLAY *display = al_create_display(640,480);
	//VERIFACMOS QUE SE CREÓ
	if(!display) {
		fprintf(stderr, "Error al crear el display.\n");
	    al_destroy_event_queue(event_queue);
	    return -1;
	}
	//le decimos que preste atencion a este display
	al_register_event_source(event_queue, al_get_display_event_source(display));

	bool salir = false;
	//con esto entramos a un while
	while(!salir) {
	    ALLEGRO_EVENT ev;
	    al_wait_for_event(event_queue, &ev);
	    if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
	        salir = true;
	    }
	}
	printf("Cerramo el display\n");

	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	printf("terminamo programa\n");
	return 0;
}



#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
//Esto es una cuadradilla de 30x30 celulas
#define C_alto  30 //cuantas celulas de alto
#define C_ancho 30 //cuantas celulas de largo
#define C_CellAncho 10 //pixeles de ancho de cada celula
#define C_CellMargin 5 //margen de pixeles de cada celula
#define PantallaMargin 20 //pixeles de margen
//definimos pixeles
#define alto C_alto*C_CellAncho + (C_alto-1)*C_CellMargin + 2*PantallaMargin
#define largo C_ancho*C_CellAncho + (C_ancho-1)*C_CellMargin + 2*PantallaMargin

int mundo[C_alto][C_ancho];

//Colores

//DEFINIMOS FUNCIONES
int inicializaciones(void);
void dibujarcelulas(int mundo[C_alto][C_ancho]);

int main (void){
	printf("Iniciamos La prueba de allegro!\n");

	if(!inicializaciones()){
		return -1; //Alguna init fallo
	}

	printf("Allegro iniciado correctamente!\n");

	//CREAMOS UNA COLA DE EVENTOS, un lugar para que se
	//acumulen los eventos

	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	if(!event_queue) {
	    fprintf(stderr, "Error al crear la cola de eventos.\n");
	    return -1;
	}

	ALLEGRO_DISPLAY *display = al_create_display(alto,largo);
	if(!display) {
		fprintf(stderr, "Error al crear el display.\n");
	    al_destroy_event_queue(event_queue);
	    return -1;
	}
	//le decimos que preste atencion a este display
	al_register_event_source(event_queue, al_get_display_event_source(display));
	//pintamos la casa de gris oscuro
	al_clear_to_color(al_map_rgb(60, 62, 66));
	dibujarcelulas(mundo);
	al_flip_display();

	bool salir = false;
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

void dibujarcelulas(int mundo[C_alto][C_ancho]){
	int cursorX;
	int cursorY = PantallaMargin;
	for(int y=0 ; y<C_alto ; y++){
		cursorX = PantallaMargin;
		for(int x=0; x<C_ancho ; x++){
			if(!mundo[y][x]){
				//dibujamos un cuadradao blanco
				al_draw_filled_rectangle(cursorX, cursorY, cursorX+C_CellAncho, cursorY+C_CellAncho, al_map_rgb(0,0,0));
				cursorX += C_CellAncho + C_CellMargin;
			}
		}//dibujo una row
		cursorY+= C_CellAncho + C_CellMargin;
	}
}

int inicializaciones(void){
	if(!al_init()) {
		fprintf(stderr, "Error al inicializar Allegro.\n");
		return 0;
	}
	if(!al_init_primitives_addon()){
		fprintf(stderr, "Error al inicializar Allegro primitives addon.\n");
		return 0;
	}
	return 1;
}



#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "constant.h"
// Esto es una cuadradilla de 30x30 celulas

#define C_CellAncho 10 // Pixeles de ancho de cada celula
#define C_CellMargin 5 // Margen de pixeles de cada celula
#define PantallaMargin 20 // Pixeles de margen
#define PantallaTextMargin 50 // Pixeles inferiores de margen

// Definimos pixeles
#define alto C_alto*C_CellAncho + (C_alto-1)*C_CellMargin + 2*PantallaMargin + PantallaTextMargin
#define largo C_ancho*C_CellAncho + (C_ancho-1)*C_CellMargin + 2*PantallaMargin

#define OUTOFMAT -1 // DEBE SER UN NUMERO NEGATIVO SI O SI
int mundo[C_alto][C_ancho];

typedef struct {
	int x;
	int y;
} celula_t;

// ---------- FUNCIONES ---------- //

int inicializaciones(void);
void dibujarcelulas(int mundo[C_alto][C_ancho]);
int POStoCELL(int posicion);

int main (void)
{
	printf("Iniciamos La prueba de allegro!\n");

	if(!inicializaciones() ){
		return -1; // Alguna init fallo
	}

	printf("Allegro iniciado correctamente!\n");

	//CREAMOS UNA COLA DE EVENTOS, un lugar para que se
	//acumulen los eventos

	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	if(!event_queue) {
	    fprintf(stderr, "Error al crear la cola de eventos.\n");
	    return -1;
	}

	ALLEGRO_DISPLAY *display = al_create_display(largo,alto);
	if(!display) {
		fprintf(stderr, "Error al crear el display.\n");
	    al_destroy_event_queue(event_queue);
	    return -1;
	}
	//CONFIGURAMOS EVENTOS
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_mouse_event_source());



	//pintamos la casa de gris oscuro
	al_clear_to_color(al_map_rgb(60, 62, 66));

	bool salir = false;
	bool redibujar = true;
	celula_t cell={0,0};

	//BUCLE DEL CODIGO:
	while(!salir) {
	    ALLEGRO_EVENT ev;
	    al_wait_for_event(event_queue, &ev);
	    if(redibujar){
		    dibujarcelulas(mundo);
		    al_flip_display();
		    redibujar = false;
	    }
	    if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
	        salir = true;
	    }
	    if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
	    	if(ev.mouse.button == 1) { // 1 = Clic izquierdo
	    		cell.x = POStoCELL(ev.mouse.x);
	    		cell.y = POStoCELL(ev.mouse.y);
	    		printf("Se hizo click en la celula (%d,%d)\n",cell.x,cell.y);
	    		if( (cell.x == OUTOFMAT) || (cell.y == OUTOFMAT)){
	    		}else{
	    			mundo[cell.y][cell.x] = !mundo[cell.y][cell.x];
	    		}
	    		redibujar=true;
	    		fflush(stdout);
	    	}
	    }
	}

	printf("Cerramo el display\n");
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	printf("terminamo programa\n");
	return 0;
}


int POStoCELL(int posicion){
	//Nos mandan una cordenada cruda, devolvemos la celula que apunta
	posicion -= PantallaMargin;
	if(posicion<0){
		return OUTOFMAT;
	}
	float cell = (float)posicion/(C_CellAncho+C_CellMargin); //0, 1, 2 y así...
	float cell_p = cell - posicion/(C_CellAncho+C_CellMargin); //parte fraccionaria
	/*
	SI LA PARTE FRACCIONARA DE LA POSICION ES MAYOR QUE EL ANCHO
	DE LA CELULA, ESO SIGNIFICA QUE ESTA APUNTANDO A UN SITIO SIN NADA
	OSEA EL RESULTADO TIENE QUE SER -1, LIKE NO EXISTE
	 */
	if(cell_p>( (float)C_CellAncho/(C_CellAncho+C_CellMargin) ) ){
		cell = OUTOFMAT;
	}
return cell;
}


void dibujarcelulas(int mundo[C_alto][C_ancho]){
	ALLEGRO_COLOR color;
	int cursorX;
	int cursorY = PantallaMargin;
	for(int y=0 ; y<C_alto ; y++){
		cursorX = PantallaMargin;
		for(int x=0; x<C_ancho ; x++){
			if(mundo[y][x]){
				color = al_map_rgb(0,0,0);
			}else{
				color = al_map_rgb(255,255,255);
			}
			al_draw_filled_rectangle(cursorX, cursorY, cursorX+C_CellAncho, cursorY+C_CellAncho, color);
			cursorX += C_CellAncho + C_CellMargin;

		}//dibujo una row
		cursorY+= C_CellAncho + C_CellMargin;
	}
}






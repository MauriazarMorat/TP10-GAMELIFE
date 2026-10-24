// ---------- LIBRERIAS ---------- //

#include <stdio.h>
#include "front.h"
#include "back.h"


// ------------ MAIN ------------- //
int mundo[C_alto][C_ancho]; // Creamos la matriz mundo, que sera la principal.
int mundoInicial[C_alto][C_ancho]; // Creamos una matriz auxiliar que guardara la generacion 0.


int main (void)
{
	if(!inicializaciones() ){
		return -1; // Alguna init fallo
	}else{
		printf("LOG: ALLEGRO INICIO CORRECTAMENTE\n");
	}

	//Creamos el eventqueue
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	if(!event_queue) {
		fprintf(stderr, "Error al crear la cola de eventos.\n");
		return -1;
	}
	//Creamos el display y fuentes
	ALLEGRO_FONT *font = al_create_builtin_font();
	ALLEGRO_DISPLAY *display = al_create_display(largo,alto);
	if(!display) {
		fprintf(stderr, "Error al crear el display.\n");
		al_destroy_event_queue(event_queue);
		return -1;
	}
	//CONFIGURAMOS EVENTOS
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_clear_to_color(al_map_rgb(60, 62, 66));

	bool salir = false;
	bool redibujar = true;
	celula_t cell={0,0};
    int i;
    int input = 0;
    int generacion = 0;


    /* Coloca un glider en el mundo.*/

    mundoInicial[1][2] = 1;
    mundoInicial[2][3] = 1;
    mundoInicial[3][1] = 1;
    mundoInicial[3][2] = 1;
    mundoInicial[3][3] = 1;

    copiar_mundo(mundo, mundoInicial);

    /* Bucle principal del programa. */

    ALLEGRO_EVENT ev;
    while (!salir) // Si el usuario ingresa la letra 'q' termina el programa.
    {
    	MainDraw(font); //Dibujamos el texto en pantalla

    	al_wait_for_event(event_queue, &ev); //Esperamos algun evento
    	//ESto pausa el programa hasta que, se mueva el mouse, se toca una tecla..


    	if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
    		salir = true;
    	}
    	if(ev.type == ALLEGRO_EVENT_KEY_DOWN) { //Se presiono una tecla
    		if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
    			salir = true;
    		}
    	}

    	if(redibujar){
    		dibujarcelulas(mundo);
    		al_flip_display();
    		redibujar = false;
    	}

        for (i = 0; i < input; i++)
        {
            actualizar_mundo(mundo);
            generacion++;
            if(i==input-1){
            	redibujar = true;
            }
        }

        printf("\nSi desea salir del programa ingrese 'q'.");
                printf("\nSi desea entrar al modo editor ingrese 'p'.");
                printf("\nSi desea entrar al modo 'salto a generacion especifica' ingrese 's'\n");
                printf("\nGeneracion actual: %d", generacion);

                do
                {
                    printf("\nIngrese la cantidad de generaciones que desea avanzar: ");
                    input = input_usuario(NORMAL_INPUT_CONFIG);
                } while (input == -1);

                if (-input == 'p')
                {
                    estado_editor(mundoInicial);
                    copiar_mundo(mundo, mundoInicial);
                    print_mundo(mundo, 0, 0, 0);
                    input = 0;
                    generacion = 0;
                }

                else if(-input == 's')
                {
                    generacion = estado_saltar_generacion_especifica(generacion, mundo, mundoInicial);
                    print_mundo(mundo, 0, 0, 0);
                    input = 0;
                } else if(-input == 'q'){
                	salir = true;
                }



    }

    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    return 0;
}

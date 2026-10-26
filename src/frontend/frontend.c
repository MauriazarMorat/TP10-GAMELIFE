// ----------- LIBRERIAS ---------- //

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "frontend.h"

// ---------- PROTOTIPOS LOCALES ---------- //

static void dibujarCelulas(GAME_STATUS *gameStatus); // Dibuja las todas las celulas del mundo.

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
	if (!al_init_image_addon())
	{
		fprintf(stderr, "Error al inicializar el complemento de imagenes.\n");
		return 0;
	}
	if (!al_install_audio())
	{
		fprintf(stderr, "Error al inicializar el audio.\n");
		return 0;
	}
	if (!al_init_acodec_addon())
	{
		fprintf(stderr, "Error al inicializar los formatos de audio.\n");
		return 0;
	}
	if (!al_reserve_samples(1))
	{
		fprintf(stderr, "Error al reservar espacio para sonidos.\n");
		return 0;
	}
	if (!al_init_ttf_addon())
	{
		fprintf(stderr, "Error al inicializar las fuentes TTF.\n");
		return 0;
	}

	return 1;
}

// FRONT GRAFICO

// Funcion encargada de dibujar las celulas.
static void dibujarCelulas(GAME_STATUS *gameStatus)
{
	ALLEGRO_COLOR color;
	int cursorX;
	int cursorY = PantallaMargin; // Consideramos los margenes de la pantalla.
	for (int y=0 ; y<C_alto ; y++)
	{
		cursorX = PantallaMargin; // empezamos desde el margen de la pantalla.
		for (int x=0; x<C_ancho ; x++)
		{
			if (gameStatus->world[y][x]) // Si la celula esta viva, el color del rectangulo a imprimir sera blanco
			{
				color = COLOR_CELULA_VIVA;
			} else
			{
				color = COLOR_CELULA_MUERTA; // Si la celula esta muerta, el color del rectangulo a imprimir sera negro.
			}
			// Dibujamos celula, se viva o muerta.
			al_draw_filled_rectangle(cursorX, cursorY, cursorX+CELL_SIZE, cursorY+CELL_SIZE, color);
			cursorX += CELL_SIZE + CELL_MARGIN;
		} // Dibujo una row
		cursorY+= CELL_SIZE + CELL_MARGIN;
	}
}

void mainDraw(ALLEGRO_FONT *font, GAME_STATUS * gameStatus)
{
	// Limpiamos la pantalla al color background.
	al_clear_to_color(BACKGROUND_COLOR);

	dibujarCelulas(gameStatus);

	// escribimos el texto debajo del mapa del juego.
	al_draw_textf(font,COLOR_TEXTO,
	PantallaMargin,
					largo-10,                   // Coordenadas X e Y en pantalla
	            0,"Pulse H para ayuda, Generacion actual: %d Esta pausado?: %d Tiempo: %0.0f%%",
				gameStatus->generacion,
				gameStatus->isPaused,
				MEDIAVELOCIDAD*(MIN_VEL-gameStatus->umbralNextGen)
	);

	al_flip_display();

	gameStatus->redraw = false;
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


// FRONT INPUT

// Funcion encargada de manejar input de teclado que se ha recibido.
void manejar_teclado(ALLEGRO_EVENT ev, GAME_STATUS*gameStatus)
{
	switch(ev.keyboard.keycode)
	{
		case ALLEGRO_KEY_BACKSPACE: // Shortcut para cerrar programa.
			gameStatus->isRunning = false;
			break;
		case ALLEGRO_KEY_T: // Tecla para avanzar una generacion.
			siguiente_generacion(gameStatus->world, &(gameStatus->generacion));
			gameStatus->redraw = true;
			break;
		case ALLEGRO_KEY_SPACE: // Seteamos el flag si se ha pausado el juego.
			gameStatus->isPaused = !(gameStatus->isPaused);
			gameStatus->isHelp = 0;

			// Si se dan estas condiciones muy probablemente se haya editado el mundo.
			if(gameStatus->generacion == 0 && !(gameStatus->isPaused)){
				copiar_mundo(gameStatus->auxWorld, gameStatus->world);
			}

			gameStatus->redraw = true;
			break;
		case ALLEGRO_KEY_R: // Funcion para volver la generacion cero del programa.
			gameStatus->isPaused = true;
			gameStatus->generacion = 0;

			copiar_mundo(gameStatus->world, gameStatus->auxWorld);


			gameStatus->redraw = true;
			break;
		case ALLEGRO_KEY_C: // Funcion para volver la generacion cero del programa y borrando todas la celulas.
			gameStatus->isPaused = true;
			gameStatus->generacion = 0;

			rellenar_mundo(gameStatus->world);

			gameStatus->redraw = true;
			break;

		case ALLEGRO_KEY_H: //Funcion para poner el modo de ayuda
			if(!gameStatus->isHelp){
				gameStatus->isHelp = true;
				gameStatus->isPaused = true;
				gameStatus->isHelpMenuBlenderReady = false;
			}else{
				gameStatus->isHelp = false;
			}
			break;

		case ALLEGRO_KEY_RIGHT: //Funcion para subir la velocidad hasta 0.00
			gameStatus->umbralNextGen -= CAMBIO_DE_VELOCIDAD;
			if(gameStatus->umbralNextGen<=CAMBIO_DE_VELOCIDAD){
				gameStatus->umbralNextGen = CAMBIO_DE_VELOCIDAD; //Topeamos el valor para que despues comienze.
			}
			break;

		case ALLEGRO_KEY_LEFT: //funcion para bajar la velocidad hasta 1.00

			gameStatus->umbralNextGen += CAMBIO_DE_VELOCIDAD; //Sumamos un nivel de velocidad
			if(gameStatus->umbralNextGen>=MIN_VEL){
				gameStatus->umbralNextGen = MIN_VEL-CAMBIO_DE_VELOCIDAD; //Topeamos el valor para que despues comienze.
			}
			break;
	}
}

// FRONT TIMER

// Manejamos el timer.
void manejar_timer(ALLEGRO_EVENT ev, GAME_STATUS*gameStatus, double*nextGenCounter)
{

	// Sumamos a nuestro contador de tiempo para siguiente generacion, el delta time.
	// Similar a como motores de videojuegos funcionan.
	*nextGenCounter += DELTA_TIME;

	if(*nextGenCounter >= (gameStatus->umbralNextGen) ) {
	    // Al cumplirse el umbral de tiempo comprobamos si el flag de pausado esta prendido, para ver si pasamos a la siguiente generacion.
	    if(!(gameStatus->isPaused)){

	    	siguiente_generacion(gameStatus->world, &(gameStatus->generacion));
	    }

	    *nextGenCounter -= gameStatus->umbralNextGen;
	}

	gameStatus->redraw = true;
}


// Al detectarse un click, buscaremos si el usuario pretente cambiar el estado de vida de una celula.
void manejar_mouse(ALLEGRO_EVENT ev, GAME_STATUS*gameStatus){

	// Al detectarse un click
	int mousePosX = ev.mouse.x;
	int mousePosY = ev.mouse.y;

	// definimos la posicion que deseamos buscar.
	position_t posCell;

	pixelCoordToWorldCoord(mousePosY, mousePosX, &posCell);
	// validamos si la posicion obtenida es valida y si NO estamos en modo ayuda
	if((posCell.x != -1 && posCell.y != -1)&&!(gameStatus->isHelp)){
		// Si lo es, entonces a la posicion del mundo asignamos el estado de vida de una celula.
	    gameStatus->world[posCell.x][posCell.y] = !(gameStatus->world[posCell.x][posCell.y]);

	    gameStatus->generacion = 0;
	    gameStatus->isPaused = true;
	    gameStatus->redraw = true;
	}

	printf("Mouse X coords: %d posCell X coords: %d\n", mousePosX, posCell.y);
	printf("Mouse Y coords: %d posCell Y coords: %d\n", mousePosY, posCell.x);
}

void helpDraw(ALLEGRO_FONT *font, GAME_STATUS *gameStatus){
	//Verificamos si ya se dibujo alguna vez, esto para opacar el fondo 1 sola vez
    if(!gameStatus->isHelpMenuBlenderReady){
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
        al_draw_filled_rectangle(0, 0, largo, alto, al_map_rgba(0, 0, 0, 100));
        gameStatus->isHelpMenuBlenderReady = true;
    }
    //Seteamos las cordenadas donde vamos a poner el cuadrado basadas en la pagina
    int x1 = largo / 3;
    int x2 = (largo * 2) / 4;
    int y1 = alto / 3;
    int y2 = (alto * 2) / 2;
    float offset = -0.5;
    int grado = 20;

    //Dibujamos un recuadro grande donde vamos a escrtibir el help text
    al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgba(40, 40, 50, 240));
    al_draw_rectangle(x1, y1, x2, y2, al_map_rgb(200, 200, 200), 2.0);

    //Escribimos los posibles comandos para el usuario
    //Dibujamos el texto de arriba hacia abajo
    al_draw_textf(font,al_map_rgb(255, 255, 255),x2+5,y2+(++offset)*grado,0,
    		"Bienvenido al overlay de ayuda"
    	);
    al_draw_textf(font,al_map_rgb(255, 255, 255),x2+5,y2+(++offset)*grado,0,
        	"Pulse Espacio para pausar el juego"
        );
    al_draw_textf(font,al_map_rgb(255, 255, 255),x2+5,y2+(++offset)*grado,0,
        	"Pulse Q para salir del juego"
        );
    al_draw_textf(font,al_map_rgb(255, 255, 255),x2+5,y2+(++offset)*grado,0,
            "Pulse T para avanzar 1 generacion"
        );
    al_draw_textf(font,al_map_rgb(255, 255, 255),x2+5,y2+(++offset)*grado,0,
        	"Pulse R para ir a la generacion de origen"
        );
    al_draw_textf(font,al_map_rgb(255, 255, 255),x2+5,y2+(++offset)*grado,0,
        	"Puede clickear una celula para cambiar su estado,"
        );
    al_draw_textf(font,al_map_rgb(255, 255, 255),x2+5,y2+(++offset)*grado,0,
            "Seteando asi una nueva generacion de origen"
        );
    al_draw_textf(font,al_map_rgb(255, 255, 255),x2+5,y2+(++offset)*grado,0,
            "Con la Right Arrow Key se aumenta la velocidad"
        );
    al_draw_textf(font,al_map_rgb(255, 255, 255),x2+5,y2+(++offset)*grado,0,
            "Con la Left Arrow Key se disminuye la velocidad"
        );
    al_draw_textf(font,al_map_rgb(255, 255, 255),x2+5,y2+(++offset)*grado,0,
        		"Pulse H para salir."
        	);
    //A partir de aca empezamos desde abajo hacia arriba
    offset = -0.5;

    al_draw_textf(font,al_map_rgb(255, 255, 255),x2+5,y1-(++offset)*grado,0,
                "Mauricio Morat Taylor"
            );

    al_draw_textf(font,al_map_rgb(255, 255, 255),x2+5,y1-(++offset)*grado,0,
                	"Santiago Oyarbide, Leonel Cidades, Victoria Olmos Lozano,"
                );
    al_draw_textf(font,al_map_rgb(255, 255, 255),x2+5,y1-(++offset)*grado,0,
                		"Creditos: GRUPO 4: "
                	);

    al_flip_display();
}

int mostrar_intro(void)
{
	ALLEGRO_DISPLAY *ventana = al_create_display(INTRO_ANCHO, INTRO_ALTO);

    if (!ventana)
    {
        return 0;
    }

	// Se cambia el titulo de la ventana y se carga el icono.
    al_set_window_title(ventana, "Grupo 4");
	ALLEGRO_BITMAP *icono = al_load_bitmap("assets/icono.png");
	al_set_display_icon(ventana, icono);
	al_destroy_bitmap(icono);

	// Se cargan los assets.
	ALLEGRO_BITMAP *logo = al_load_bitmap("assets/Godot_c_logo.png");
	ALLEGRO_SAMPLE *musica = al_load_sample("assets/intro.wav");
	ALLEGRO_EVENT_QUEUE *cola = al_create_event_queue();
	ALLEGRO_TIMER *timer = al_create_timer(DELTA_TIME);

	// Si falta un asset se cancela la intro.
	if (!logo || !musica || !cola || !timer)
	{
		if (logo) al_destroy_bitmap(logo);
		if (musica) al_destroy_sample(musica);
		if (cola) al_destroy_event_queue(cola);
		if (timer) al_destroy_timer(timer);

		al_destroy_display(ventana);
		return 0;
	}

	al_register_event_source(cola, al_get_display_event_source(ventana));
	al_register_event_source(cola, al_get_timer_event_source(timer));
	
	// Se inicia la ventana en negro.
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();

	// Se inicia la musica
	al_play_sample(
		musica,
		1.0,
		0.0,
		1.0,
		ALLEGRO_PLAYMODE_ONCE,
		NULL
	);

	// Guardamos el tiempo en el que inicio la musica y comenzamos el timer.
	double inicio = al_get_time();
	al_start_timer(timer);

	ALLEGRO_EVENT evento; // Guarda el evento recibido de la cola.
	int salir = 0;       // Indica cuándo terminar el bucle de la intro.
	int continuar = 0;   // Indica si al terminar la intro debe abrirse el juego.

	// Configuramos transparencia.
	al_set_blender(
		ALLEGRO_ADD,
		ALLEGRO_ONE,
		ALLEGRO_INVERSE_ALPHA
	);

	while (salir == 0)
	{
		al_wait_for_event(cola, &evento);

		// Si se presiona la X se cierra la ventana y no se abre el juego.
		if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			salir = 1;
		}
		else if (evento.type == ALLEGRO_EVENT_TIMER)
		{
			// Calcula los segundos transcurridos tras iniciar la intro.
			double tiempo = al_get_time() - inicio;

			// Si el tiempo es mayor a la duración de la intro, se cierra la ventana y se setea el flag para abrir el juego.
			if (tiempo >= DURACION_TOTAL)
			{
				continuar = 1;
				salir = 1;
			}
			else if (al_is_event_queue_empty(cola))
			{
				float oscuridad = 0.0;

				// Durante el fade in, la oscuridad baja de 1 a 0.
				if (tiempo < DURACION_FADE_IN)
				{
					oscuridad = 1.0 - tiempo / DURACION_FADE_IN;
				}
				// Durante el fade out, la oscuridad sube de 0 a 1.
				else if (tiempo >= DURACION_FADE_IN + DURACION_VISIBLE)
				{
					oscuridad = 1.0 - (DURACION_TOTAL - tiempo) / DURACION_FADE_OUT;
				}

				al_clear_to_color(al_map_rgb(0, 0, 0));

				// Dibuja el logo ajustado al tamaño de la ventana.
				al_draw_scaled_bitmap(
					logo,
					0, 0,
					al_get_bitmap_width(logo),
					al_get_bitmap_height(logo),
					0, 0,
					INTRO_ANCHO, INTRO_ALTO,
					0
				);

				// Cubre el logo con una rectangulo negro de opacidad variable.
				al_draw_filled_rectangle(
					0, 0, INTRO_ANCHO, INTRO_ALTO,
					al_map_rgba_f(0, 0, 0, oscuridad)
				);

				al_flip_display();
			}
		}
	}

	al_stop_samples();

	al_destroy_sample(musica);
	al_destroy_timer(timer);
	al_destroy_event_queue(cola);
	al_destroy_bitmap(logo);
	al_destroy_display(ventana);

	return continuar;
}
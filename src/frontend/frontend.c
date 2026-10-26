// ----------- LIBRERIAS ---------- //

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "frontend.h"

// ----------- SONIDOS ---------- //

ALLEGRO_SAMPLE *sonidoH = NULL;
ALLEGRO_SAMPLE *sonidoR = NULL;
ALLEGRO_SAMPLE *sonidoRight = NULL;
ALLEGRO_SAMPLE *sonidoLeft = NULL;
ALLEGRO_SAMPLE *sonidoEspacioPausa = NULL;
ALLEGRO_SAMPLE *sonidoEspacioDespausa = NULL;
ALLEGRO_SAMPLE *sonidoClickPoner = NULL;
ALLEGRO_SAMPLE *sonidoClickQuitar = NULL;
ALLEGRO_SAMPLE *sonidoC = NULL;
ALLEGRO_SAMPLE *sonidoX = NULL;

// ---------- PROTOTIPOS LOCALES ---------- //

static void dibujarCelulas(GAME_STATUS *gameStatus); // Dibuja las todas las celulas del mundo.

// ---------- FUNCIONES ---------- //

int init_front(void)
{
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
	if (!al_reserve_samples(11))
	{
		fprintf(stderr, "Error al reservar espacio para sonidos.\n");
		return 0;
	}
	if (!al_init_ttf_addon())
	{
		fprintf(stderr, "Error al inicializar las fuentes TTF.\n");
		return 0;
	}

	// Cargamos efectos de sonido.
	sonidoH = al_load_sample("assets/sonidoH.ogg");
	sonidoR = al_load_sample("assets/sonidoR.ogg");
	sonidoRight = al_load_sample("assets/sonidoRight.ogg");
	sonidoLeft = al_load_sample("assets/sonidoLeft.ogg");
	sonidoEspacioPausa = al_load_sample("assets/sonidoEspacioPausa.wav");
	sonidoEspacioDespausa = al_load_sample("assets/sonidoEspacioDespausa.wav");
	sonidoClickPoner = al_load_sample("assets/sonidoClickPoner.wav");
	sonidoClickQuitar = al_load_sample("assets/sonidoClickQuitar.wav");
	sonidoC = al_load_sample("assets/sonidoC.wav");
	sonidoX = al_load_sample("assets/sonidoX.wav");

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
			if (gameStatus->world[y][x])
			{
				color = COLOR_CELULA_VIVA;
			} else
			{
				color = COLOR_CELULA_MUERTA;
			}

			// Dibujamos celula, sea viva o muerta.
			al_draw_filled_rectangle(cursorX, cursorY, cursorX+CELL_SIZE, cursorY+CELL_SIZE, color);
			cursorX += CELL_SIZE + CELL_MARGIN;
		} // Dibujamos una row
		cursorY+= CELL_SIZE + CELL_MARGIN;
	}
}

void mainDraw(ALLEGRO_FONT *font, GAME_STATUS * gameStatus)
{
	// Limpiamos la pantalla al color background.
	al_clear_to_color(BACKGROUND_COLOR);

	dibujarCelulas(gameStatus);

	// Escribimos el texto debajo del mapa del juego.
	al_draw_textf(font,COLOR_TEXTO,
				PantallaMargin,
				largo-10,                   // Coordenadas X e Y en pantalla
	            0,"Pulse H para ayuda, Generacion actual: %d Pausado?: %d Velocidad: %0.0f%%",
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
			if (sonidoX) 
			{
				al_play_sample(sonidoX, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
			al_rest(0.5); // Pausa breve para que el sonido termine antes de destruir la ventana
			gameStatus->isRunning = false;
			break;

		case ALLEGRO_KEY_T: // Tecla para avanzar una generacion.
			if (sonidoH) 
			{
				al_play_sample(sonidoH, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
			siguiente_generacion(gameStatus->world, &(gameStatus->generacion));
			gameStatus->redraw = true;
			break;

		case ALLEGRO_KEY_SPACE: // Seteamos el flag si se ha pausado el juego.
			gameStatus->isPaused = !(gameStatus->isPaused);
			gameStatus->isHelp = 0;

			if (gameStatus->isPaused) 
			{
				if (sonidoEspacioPausa) 
				{
					al_play_sample(sonidoEspacioPausa, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
			} else 
			{
				if (sonidoEspacioDespausa)
				{
					al_play_sample(sonidoEspacioDespausa, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
			}
			// Si se dan estas condiciones muy probablemente se haya editado el mundo.
			if (gameStatus->generacion == 0 && !(gameStatus->isPaused))
			{
				copiar_mundo(gameStatus->auxWorld, gameStatus->world);
			}

			gameStatus->redraw = true;
			break;

		case ALLEGRO_KEY_R: // Funcion para volver la generacion cero del programa.
			if (sonidoR) 
			{
				al_play_sample(sonidoR, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
			gameStatus->isPaused = true;
			gameStatus->generacion = 0;

			copiar_mundo(gameStatus->world, gameStatus->auxWorld);

			gameStatus->redraw = true;
			break;

		case ALLEGRO_KEY_C: // Funcion para volver la generacion cero del programa y borrando todas la celulas.
			if (sonidoC) 
			{
				al_play_sample(sonidoC, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
			gameStatus->isPaused = true;
			gameStatus->generacion = 0;

			rellenar_mundo(gameStatus->world);

			gameStatus->redraw = true;
			break;

		case ALLEGRO_KEY_H: //Funcion para poner el modo de ayuda
			if (sonidoH) 
			{
				al_play_sample(sonidoH, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
			if (!gameStatus->isHelp){
				gameStatus->isHelp = true;
				gameStatus->isPaused = true;
			} else
			{
				gameStatus->isHelp = false;
			}
			break;

		case ALLEGRO_KEY_RIGHT: //Funcion para subir la velocidad hasta 0.00
			if (sonidoRight) 
			{
				al_play_sample(sonidoRight, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
			gameStatus->umbralNextGen -= CAMBIO_DE_VELOCIDAD;
			if(gameStatus->umbralNextGen<=CAMBIO_DE_VELOCIDAD)
			{
				gameStatus->umbralNextGen = CAMBIO_DE_VELOCIDAD; //Topeamos el valor para que despues comienze.
			}
			break;

		case ALLEGRO_KEY_LEFT: //funcion para bajar la velocidad hasta 1.00
			if (sonidoLeft) 
			{
				al_play_sample(sonidoLeft, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
			gameStatus->umbralNextGen += CAMBIO_DE_VELOCIDAD; //Sumamos un nivel de velocidad
			if(gameStatus->umbralNextGen>=MIN_VEL)
			{
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

	if(*nextGenCounter >= (gameStatus->umbralNextGen) ) 
	{
	    // Al cumplirse el umbral de tiempo comprobamos si el flag de pausado esta prendido, para ver si pasamos a la siguiente generacion.
	    if(!(gameStatus->isPaused))
		{
	    	siguiente_generacion(gameStatus->world, &(gameStatus->generacion));
	    }

	    *nextGenCounter -= gameStatus->umbralNextGen;
	}

	gameStatus->redraw = true;
}


// Al detectarse un click, buscaremos si el usuario pretente cambiar el estado de vida de una celula.
void manejar_mouse(ALLEGRO_EVENT ev, GAME_STATUS*gameStatus)
{
	// Al detectarse un click
	int mousePosX = ev.mouse.x;
	int mousePosY = ev.mouse.y;

	// definimos la posicion que deseamos buscar.
	position_t posCell;

	pixelCoordToWorldCoord(mousePosY, mousePosX, &posCell);
	// validamos si la posicion obtenida es valida y si NO estamos en modo ayuda
	if ((posCell.x != -1 && posCell.y != -1)&&!(gameStatus->isHelp))
	{
		// Si lo es, entonces a la posicion del mundo asignamos el estado de vida de una celula.
	    gameStatus->world[posCell.x][posCell.y] = !(gameStatus->world[posCell.x][posCell.y]);

		if (gameStatus->world[posCell.x][posCell.y] == 1) 
		{
			if (sonidoClickPoner) al_play_sample(sonidoClickPoner, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		} else 
		{
			if (sonidoClickQuitar) al_play_sample(sonidoClickQuitar, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		}

	    gameStatus->generacion = 0;
	    gameStatus->isPaused = true;
	    gameStatus->redraw = true;
	}
}

void helpDraw(ALLEGRO_FONT *font, GAME_STATUS *gameStatus)
{
    int margen = 20;
    int espacio = al_get_font_line_height(font) + 6;

    // Esquina inferior derecha del panel.
    int x2 = (largo) - margen;
    int y2 = (alto) - margen;

    // El panel tiene 760 píxeles de ancho y espacio para 16 líneas.
    int x1 = x2 - 760;
    int y1 = y2 - (16 * espacio + 2 * margen);

    // Posición donde empieza el texto.
    int x = x1 + margen;
    int y = y1 + margen;

    ALLEGRO_COLOR blanco = al_map_rgb(255, 255, 255);

    // Dibuja nuevamente el tablero.
    al_clear_to_color(BACKGROUND_COLOR);
    dibujarCelulas(gameStatus);

    // Configura la transparencia y oscurece el tablero.
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);

    al_draw_filled_rectangle(
        0, 0, largo, alto,
        al_map_rgba_f(0, 0, 0, 0.4)
    );

    // Dibuja el panel oscuro y su borde blanco.
    al_draw_filled_rectangle(
        x1, y1, x2, y2,
        al_map_rgb(40, 40, 50)
    );

    al_draw_rectangle(x1, y1, x2, y2, blanco, 2.0);

    // Después de cada texto, baja una línea.
    al_draw_text(font, blanco, x, y, 0,
                 "Bienvenido al overlay de ayuda");
    y += espacio;

    al_draw_text(font, blanco, x, y, 0,
                 "Pulse Espacio para iniciar o pausar");
    y += espacio;

    al_draw_text(font, blanco, x, y, 0,
                 "Pulse Backspace para salir del juego");
    y += espacio;

    al_draw_text(font, blanco, x, y, 0,
                 "Pulse T para avanzar una generacion");
    y += espacio;

    al_draw_text(font, blanco, x, y, 0,
                 "Pulse R para volver a la generacion inicial");
    y += espacio;

    al_draw_text(font, blanco, x, y, 0,
                 "Pulse C para limpiar el tablero");
    y += espacio;

    al_draw_text(font, blanco, x, y, 0,
                 "Click en una celula para cambiar su estado");
    y += espacio;

    al_draw_text(font, blanco, x, y, 0,
                 "Esto establece una nueva generacion de origen");
    y += espacio;

    al_draw_text(font, blanco, x, y, 0,
                 "Flecha derecha: aumentar la velocidad");
    y += espacio;

    al_draw_text(font, blanco, x, y, 0,
                 "Flecha izquierda: disminuir la velocidad");
    y += espacio;

    al_draw_text(font, blanco, x, y, 0,
                 "Pulse H para cerrar la ayuda");

    // Deja una línea vacía antes de los créditos.
    y += 2 * espacio;

    al_draw_text(font, blanco, x, y, 0,
                 "Creditos: GRUPO 4");
    y += espacio;

    al_draw_text(font, blanco, x, y, 0,
                 "Santiago Oyarbide - Leonel Cidades");
    y += espacio;

    al_draw_text(font, blanco, x, y, 0,
                 "Victoria Olmos Lozano");
    y += espacio;

    al_draw_text(font, blanco, x, y, 0,
                 "Mauricio Morat Taylor");

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

void limpiar_sonidos_front(void)
{
    // Detiene las reproducciones antes de liberar los sonidos.
    al_stop_samples();

    if (sonidoH) al_destroy_sample(sonidoH);
    if (sonidoR) al_destroy_sample(sonidoR);
    if (sonidoRight) al_destroy_sample(sonidoRight);
    if (sonidoLeft) al_destroy_sample(sonidoLeft);
    if (sonidoEspacioPausa) al_destroy_sample(sonidoEspacioPausa);
    if (sonidoEspacioDespausa) al_destroy_sample(sonidoEspacioDespausa);
    if (sonidoClickPoner) al_destroy_sample(sonidoClickPoner);
    if (sonidoClickQuitar) al_destroy_sample(sonidoClickQuitar);
    if (sonidoC) al_destroy_sample(sonidoC);
    if (sonidoX) al_destroy_sample(sonidoX);

    sonidoH = NULL;
    sonidoR = NULL;
    sonidoRight = NULL;
    sonidoLeft = NULL;
    sonidoEspacioPausa = NULL;
    sonidoEspacioDespausa = NULL;
    sonidoClickPoner = NULL;
    sonidoClickQuitar = NULL;
    sonidoC = NULL;
    sonidoX = NULL;
}
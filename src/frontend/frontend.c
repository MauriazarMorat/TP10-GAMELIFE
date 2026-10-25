// ----------- LIBRERIAS ---------- //

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

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
				color = al_map_rgb(0,0,0);
			} else
			{
				color = al_map_rgb(255,255,255); // Si la celula esta muerta, el color del rectangulo a imprimir sera negro.
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
	al_draw_textf(font,al_map_rgb(255, 255, 255),
	PantallaMargin,
					largo-10,                   // Coordenadas X e Y en pantalla
	            0,"Pulse H para ayuda, Generacion actual: %d Esta pausado?: %d",gameStatus->generacion,gameStatus->isPaused
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
		case ALLEGRO_KEY_H:
			if(!gameStatus->isHelp){
				gameStatus->isHelp = 1;
				gameStatus->isPaused = 1;
				gameStatus->isDrew = 0;
			}else{
				gameStatus->isHelp = 0;
			}


			break;
	}
}

// FRONT TIMER

// Manejamos el timer.
void manejar_timer(ALLEGRO_EVENT ev, GAME_STATUS*gameStatus, double*nextGenCounter, double*umbralNextGen)
{

	// Sumamos a nuestro contador de tiempo para siguiente generacion, el delta time.
	// Similar a como motores de videojuegos funcionan.
	*nextGenCounter += DELTA_TIME;

	if(*nextGenCounter >= *umbralNextGen) {
	    // Al cumplirse el umbral de tiempo comprobamos si el flag de pausado esta prendido, para ver si pasamos a la siguiente generacion.
	    if(!(gameStatus->isPaused)){
	    	printf("un segundo paso!\n");
	    	siguiente_generacion(gameStatus->world, &(gameStatus->generacion));
	    }

	    *nextGenCounter -= *umbralNextGen;
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
	// validamos si la posicion obtenida es valida.
	if(posCell.x != -1 && posCell.y != -1){
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
    if(!gameStatus->isDrew){
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
        al_draw_filled_rectangle(0, 0, largo, alto, al_map_rgba(0, 0, 0, 100));
        gameStatus->isDrew = 1;
    }

    int x1 = largo / 3;
    int x2 = (largo * 2) / 4;
    int y1 = alto / 3;
    int y2 = (alto * 2) / 2;
    float offset = -0.5;
    int grado = 20;

    al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgba(40, 40, 50, 240));
    al_draw_rectangle(x1, y1, x2, y2, al_map_rgb(200, 200, 200), 2.0);
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
        		"Pulse H para salir."
        	);
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

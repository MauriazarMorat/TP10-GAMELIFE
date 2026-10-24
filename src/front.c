// ----------- LIBRERIAS ---------- //

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "front.h"

// ---------- CONSTANTES ---------- //

#define TAM_CELULA 25
#define ALTO_VENTANA 600
#define ANCHO_VENTANA 600

// --------- VARIABLES ----------- //

static ALLEGRO_DISPLAY *ventana;

// ---------- FUNCIONES ---------- //

int iniciar_front()
{
    if (!al_init())
    {
        return 1;
    } 
    else if (!al_init_primitives_addon())
    {
        return 1;
    }

    ventana = al_create_display(ANCHO_VENTANA, ALTO_VENTANA);
    
    if (ventana == NULL)
    {
        return 1;
    }

    return 0;
}

void cerrar_front()
{
    al_destroy_display(ventana);
    ventana = NULL;
}

void dibujar_mundo(int mundo[ALTO][ANCHO])
{
    int i, j;
    int x1, y1, x2, y2;
    ALLEGRO_COLOR color;

    al_clear_to_color(al_map_rgb(50, 50, 50));

    for (i = 0; i < ALTO; i++)
    {
        for (j = 0; j < ANCHO; j++)
        {
            // Elegimos el color según el estado de la célula.
            if (mundo[i][j] == 1)
            {
                color = al_map_rgb(255, 255, 255);
            }
            else
            {
                color = al_map_rgb(0, 0, 0);
            }

            // Convertimos los índices de la matriz a píxeles.
            x1 = j * TAM_CELULA;
            y1 = i * TAM_CELULA;

            x2 = x1 + TAM_CELULA;
            y2 = y1 + TAM_CELULA;

            // Dibujamos la célula.
            al_draw_filled_rectangle (x1, y1, x2 - 1, y2 - 1, color);
        }
    }

    // Mostramos la matriz completa.
    al_flip_display();
}

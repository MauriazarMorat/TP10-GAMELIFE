// ----------- LIBRERIAS ---------- //

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "front.h"

// ---------- CONSTANTES ---------- //

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

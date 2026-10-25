// ---------- LIBRERIAS ---------- //

#include "backend.h"

#include <stdio.h>
#include <stdint.h>

// ---------- FUNCIONES ---------- //

void rellenar_mundo (int matriz [C_alto][C_ancho])
{
	/* Inicializa el mundo con celulas muertas, recorre todas las posiciones de la matriz y les asigna un valor de 0.*/

    int i, j;

    for (i = 0; i < C_alto; i++)
    {
        for (j = 0; j < C_ancho; j++)
        {
            matriz[i][j] = 0;
        }
    }
}

int  contar_vecinos (int matriz [C_alto][C_ancho], int y, int x)
{
	/* Cuenta la cantidad de celulas vivas que rodean a una celula determinada.
	Recorre las 8 posiciones vecinas a la celula, verificando primero que
	cada posicion se encuentre dentro de los limites del mundo. */

	int vecinos = 0;
    int i,j;

	/* Recorre las posiciones alrededor de la celula en y. */

    	for (i = y - 1; i <= y + 1; i++)
    	{
    		/* Verifica que la posicion a analizar se encuentre dentro de los limites del mundo. */

        	for (j = x - 1; j <= x + 1; j++)
        	{
            		if (i >= 0 && i < C_alto && j >= 0 && j < C_ancho)
            		{
            			/* Si la posicion contiene una celula viva y no es la propia celula
               		    a analizar, se incrementa el contador de vecinos. */

                		if (matriz[i][j] == 1 && (i != y || j != x))
                		{
                    			vecinos++;
                		}
            		}
        	}
    	}

    	return vecinos;
}

void aplicar_reglas_mundo (int matriz[C_alto][C_ancho])
{
	/* Se encarga de actualizar el estado de cada celula en el mundo siguiendo las reglas del juego de la vida de Conway.
	Para esto primero actualiza el mundo en una matriz aparte para que los cambios no interfieran en el estado de otras celulas. */

	int i,j;
	int cant_vecinos;
	int sig_mundo[C_alto][C_ancho];

	/* Recorre todas las posiciones del mundo y llama a la función "contar_vecinos" para cada celula. */

	for (i = 0; i < C_alto; i++)
	{
		for (j = 0; j < C_ancho; j++)
		{

			/* Aplicamos las reglas del juego de la vida. */

			cant_vecinos = contar_vecinos(matriz, i, j);
			if (cant_vecinos == 3 || (matriz[i][j] == 1 && cant_vecinos == 2))
			{
				sig_mundo[i][j] = 1;
			}
			else
			{
				sig_mundo[i][j] = 0;
			}
		}
	}

   	copiar_mundo(matriz, sig_mundo);
}

void siguiente_generacion (int matriz[C_alto][C_ancho], uint16_t * generacionActual)
{
		aplicar_reglas_mundo(matriz);
		(*generacionActual)++;
}

void copiar_mundo(int mundo[C_alto][C_ancho], int mundoToCopy[C_alto][C_ancho])
{
	/* Una funcion de ayuda para poder igualar el mundo y mundo inicial (y viceversa) */

	int y;
	int x;

	for (y = 0; y < C_alto; y++)
    {
		for (x = 0; x < C_ancho; x++)
        {
			mundo[x][y] = mundoToCopy[x][y];
		}
	}
}

/* Hace el calculo recursivo para saltar a la generacion especifica. Notese como se distingue si es la generacion pedida es previa o posterior a la actual.
En base a eso, se trata de optimizar, el punto de arranque.*/
/*
int ejecutar_hasta_generacion_especifica(int generacionTarget, int generacionActual, int mundoActual[ALTO][ANCHO], int mundoInicial[ALTO][ANCHO]){
	int i = 0;

	if(generacionTarget > generacionActual)
	{
		for(i = generacionActual; i <= generacionTarget; i++)
		{
			actualizar_mundo(mundoActual);
		}
	}
	else if (generacionTarget < generacionActual && generacionTarget > 0)
	{

		* Si la generacion pedida es anterior a la actual, se partira del calculo de generacion desde el mundo inicial.

		 *
		copiar_mundo(mundoActual, mundoInicial);

		for(i = 0; i <= generacionTarget; i++)
		{
			actualizar_mundo(mundoActual);
		}
	}
	else if (generacionTarget == 0)
	{
		* Si la generacion es cero, se copia exactamente los valores de la matriz del mundo inicial a la de mundo,.
		copiar_mundo(mundoActual, mundoInicial);
	}

	return generacionTarget;
}
*/

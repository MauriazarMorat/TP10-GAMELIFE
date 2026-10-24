// ---------- LIBRERIAS ---------- //

#include <stdio.h>
#include <allegro5/allegro.h>

#include "front.h"
#include "back.h"


// ------------ MAIN ------------- //

int main (void)
{
    int i; // Contador para for.
    int mundo[ALTO][ANCHO]; // Creamos la matriz mundo, que sera la principal.
    int mundoInicial[ALTO][ANCHO]; // Creamos una matriz auxiliar que guardara la generacion 0.
    int input = 0;
    int generacion = 0;

    rellenar_mundo(mundoInicial);
    rellenar_mundo(mundo); // Inicializa el mundo con celulas muertas. (Rellena la matriz con 0s).

    /* Coloca un glider en el mundo.*/

    mundoInicial[1][2] = 1;
    mundoInicial[2][3] = 1;
    mundoInicial[3][1] = 1;
    mundoInicial[3][2] = 1;
    mundoInicial[3][3] = 1;

    copiar_mundo(mundo, mundoInicial);
	print_mundo(mundo, 0, 0, 0);

    /* Bucle principal del programa. */

    while (-input != 'q') // Si el usuario ingresa la letra 'q' termina el programa.
    {
        /* Al recibir un input equivalente a un numero natural, se actualiza y se imprime el mundo esa
        cantidad de veces. */

        for (i = 0; i < input; i++)
        {
            actualizar_mundo(mundo);
            generacion++;
            print_mundo(mundo, 0, 0, 0);
        }

        /* Se llama a la función para recibir un input del usuario, mientras ese input sea invalido
        se continua pidiendo un valor.*/

        printf("\nSi desea salir del programa ingrese 'q'.");
        printf("\nSi desea entrar al modo editor ingrese 'p'.");
        printf("\nSi desea entrar al modo 'salto a generacion especifica' ingrese 's'\n");
        printf("\nGeneracion actual: %d", generacion);

        do
        {
            printf("\nIngrese la cantidad de generaciones que desea avanzar: ");
            input = input_usuario(NORMAL_INPUT_CONFIG);
        } while (input == -1);


        /* Si el usuario ingresa la letra 'p', se llama a la funcion "estado_editor" para
        editar el mundo. Una vez el usuario termina de editar, se vuelve a imprimir el mundo con los cambios realizados y se
        setea el valor de input a 0 para que no entre al for que actualiza el mundo. */

        if (-input == 'p')
        {
            estado_editor(mundoInicial);
            copiar_mundo(mundo, mundoInicial);
            print_mundo(mundo, 0, 0, 0);
            input = 0;
            generacion = 0;
        }

        /* Si el usuario ingresa la letra 's', se llama a la funcion "estado_saltar_generacion_especifica" para
        entrar al modo de salto a generacion especifica. Una vez el usuario termina, se vuelve a imprimir el mundo con la ultima generacion pedida y se
        setea el valor de input a 0 para que no entre al for que actualiza el mundo.*/

        else if(-input == 's')
        {
            generacion = estado_saltar_generacion_especifica(generacion, mundo, mundoInicial);
            print_mundo(mundo, 0, 0, 0);
            input = 0;
        }
    }

    return 0;
}

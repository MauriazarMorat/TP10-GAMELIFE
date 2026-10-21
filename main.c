#include <stdio.h>

/*

--- FUNCIONALIDADES EXTRA ---

1. Modo "editor", permite poner y sacar celulas mediante un cursor, evitando el hardcoding.
2. Modo "salto a generacion especifica", permite saltar hacer un calculo matematico recurente de la condicion de la generacion exacta pedida.
3. Saltar de estos modos de forma compatible y con una buena experiencia de usuario.
4. Propuesta grafica, mediante la implementacion de emojis, y una tecnica, se logro un buen resultado grafico.

Hemos disfrutado mucho de hacer este TP. Espero que disfrute de nuestro trabajo.

*/

// ---------- CONSTANTES ---------- //

#define ALTO 20
#define ANCHO 20

#define NORMAL_INPUT_CONFIG 1
#define SALTO_GENERACION_INPUT_CONFIG 0

// ------ ERRORES Y WARNINGS ------ //

#if ALTO <= 0 || ANCHO <= 0
#error Las constantes ALTO y ANCHO deben ser mayores a 0.
#elif ALTO > 50 || ANCHO > 50 || ANCHO * ALTO > 1000
#warning Dados los valores de ALTO y ANCHO elejidos puede que la salida sea dificil de ver.
#endif

// ---------- PROTOTIPOS ---------- //

void estado_editor(int matriz [ALTO][ANCHO]);
void limpiar_buffer (void);
int input_usuario (int considerar_salto_linea);
int contar_vecinos (int matriz [ALTO][ANCHO], int y, int x);
void print_mundo (int matriz [ALTO][ANCHO], int editor, int yCursor, int xCursor);
void rellenar_mundo(int matriz [ALTO][ANCHO]);
void actualizar_mundo (int matriz [ALTO][ANCHO]);

void copiar_mundo(int mundo[ALTO][ANCHO], int mundoToCopy[ALTO][ANCHO]);
int estado_saltar_generacion_especifica(int generacionIngresante, int mundoActual[ALTO][ANCHO], int mundoInicial[ALTO][ANCHO]);
int ejecutar_hasta_generacion_especifica(int generacionTarget, int generacionActual, int mundoActual[ALTO][ANCHO], int mundoInicial[ALTO][ANCHO]);

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

// ---------- FUNCIONES ---------- //

/* Limpia el buffer de caracteres ejecutando la función getchar hasta detectar un enter. */

void limpiar_buffer (void)
{
	int c;
    	while ((c = getchar()) != '\n')
    	{}
}

/* Por medio de getchar, detecta números naturales ingresados por el usuario y los returna en forma de int.
Tambien detecta inputs especiales tales como Enter, 'q' y 'p' y retorna -1 si se ingresa un input invalido
(Números no naturales o caracteres aleatorios). Ademas, dependiendo de la configuracion de donde es llamada
la funcion, tiene algunas funcionalidades que se adapta al programa*/

int input_usuario (int configuracionInput)
{
	int numInvalido = 0;
    	int c;
    	int num = 0;
	c = getchar();

    /* Los comandos especiales se retornan como su valor ASCII negativo para diferenciarlos
    de un numero ingresado por el usuario que coincida con ese valor ASCII. */

	if (c == '\n' && configuracionInput == NORMAL_INPUT_CONFIG)
	{
		/* Si el primer caracter es un enter, activa el flag de "numInvalido" para que no lea los siguientes caracteres
    	en el buffer y retorna un 1. Esto solo sucede solo si la configuracion del input es de modo normal del programa. */

		numInvalido = 1;
		num = 1;
	}
	else if (c == '\n' && configuracionInput == SALTO_GENERACION_INPUT_CONFIG)
	{
		/* Si el usuario hace un salto de linea cuando esta en el modo de salto de generacion especifica, significa que
		no ha hecho una entrada correcta.*/

		numInvalido = 2;
	}
	else if (c == 'q')
	{
		/* Si el primer caracter es la letra 'q', activa el flag de "numInvalido" para que no lea los siguientes caracteres
    	en el buffer y retorna -'q'. Es aplicado para salir de un modo o cerrar el programa */

		numInvalido = 1;
		num = -'q';
		limpiar_buffer();
	}
	else if (c == 'p' && configuracionInput == NORMAL_INPUT_CONFIG)
	{
		/* Si el primer caracter es la letra 'p', activa el flag de "numInvalido" para que no lea los siguientes caracteres
    	en el buffer y retorna -'p'. Es para saltar al modo editor.*/

		numInvalido = 1;
		num = -'p';
		limpiar_buffer();
	}
	else if(c == 's' && configuracionInput == NORMAL_INPUT_CONFIG)
    {
		/* Si el primer caracter es la letra 's', activa el flag de "numInvalido" para que no lea los siguientes caracteres
    	en el buffer y retorna -'s'. Es para saltar al modo de saltar de generacion.*/

		numInvalido = 1;
		num = -'s';
		limpiar_buffer();
	}

	/* Si el primer caracter no fué un input especial, se recorren todos los caracteres
    en el buffer hasta detectar un enter. Si el caracter leido es un número, se lo convierte a su valor
    decimal y se lo agrega a la variable a retornar. Si se detecta cualquier otro caracter se pone el flag de
    "numInvalido" en 2. */


	// Validamos si es que hubo un ingreso de numeros, si este fue valido.

	if (numInvalido == 0)
	{
		do
		{
			if (c >= '0' && c <= '9')
		     	{
				num = num * 10 + (c - '0');
		     	}
		     	else
		     	{
		         	numInvalido = 2;
		     	}
		} while ((c = getchar()) != '\n');
	    }

	if (numInvalido == 2)
	{
		/* Si luego de revisar todo el buffer el flag de "numInvalido" se encuentra en 2.
    	Se le indica al usuario que se ingreso un número invalido y se retorna -1. */

		printf("\nERROR: Ingrese un número entero.\n");
		num = -1;
	}

	return num;
}

/* Inicializa el mundo con celulas muertas, recorre todas las posiciones de la matriz y les asigna un valor de 0.*/

void rellenar_mundo (int matriz [ALTO][ANCHO])
{
    int i, j;

    for (i = 0; i < ALTO; i++)
    {
        for (j = 0; j < ANCHO; j++)
        {
            matriz[i][j] = 0;
        }
    }
}

/* Cuenta la cantidad de celulas vivas que rodean a una celula determinada.
Recorre las 8 posiciones vecinas a la celula, verificando primero que
cada posicion se encuentre dentro de los limites del mundo. */

int contar_vecinos (int matriz [ALTO][ANCHO], int y, int x)
{
	int vecinos = 0;
    	int i,j;

	/* Recorre las posiciones alrededor de la celula en y. */

    	for (i = y - 1; i <= y + 1; i++)
    	{
    		/* Verifica que la posicion a analizar se encuentre dentro de los limites del mundo. */

        	for (j = x - 1; j <= x + 1; j++)
        	{
            		if (i >= 0 && i < ALTO && j >= 0 && j < ANCHO)
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

/* Se encarga de actualizar el estado de cada celula en el mundo siguiendo las reglas del juego de la vida de Conway.
Para esto primero actualiza el mundo en una matriz aparte para que los cambios no interfieran en el estado de otras celulas. */

void actualizar_mundo (int matriz[ALTO][ANCHO])
{
	int i,j;
	int cant_vecinos;
	int sig_mundo[ALTO][ANCHO];

	/* Recorre todas las posiciones del mundo y llama a la función "contar_vecinos" para cada celula. */

	for (i = 0; i < ALTO; i++)
	{
		for (j = 0; j < ANCHO; j++)
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

/* Esta funcion imprime la pantalla usando caracteres UNICODE (especificamente emojis de rectangulos de colores).
A modo de funcionalidad extra, tambien muestra el cursor para modo editor, si es que el modo esta activo. */

void print_mundo (int matriz [ALTO][ANCHO], int editor, int yCursor, int xCursor)
{
	int i, j;

    	/* El grupo conocia la existencia de system("cls") sin embargo, eso es una funcionalidad de Windows.
    	Por lo tanto buscamos si existia una manera multiplaforma de hacer algo similar. En el siguiente posteo de Stack Overflow llegamos a esta tecnica.

    	https://stackoverflow.com/questions/17271576/clear-screen-in-c-and-c-on-unix-based-system

    	El comentario de Silvu Burcea dice:

    	"(...) \033[H will position the cursor to the home position and \033[J stands for erase lines below." */

    	printf("\033[H\033[J");

    	/* Ponemos en pantalla el estado de cada celda usando emojis. Si estamos en modo editor, mostramos el cursor. */

    	for (i = 0; i < ALTO; i++)
    	{
            for (j = 0; j < ANCHO; j++)
            {
                if (editor == 1 && i == yCursor && j == xCursor)
                {
                    if (matriz[i][j] == 1)
                    {
                        printf("🟩");
                    }
                    else
                    {
                        printf("🟦");
                    }
                }
                else if (matriz[i][j] == 1)
                {
                    printf("⬜");
                }
                else
                {
                    printf("⬛");
                }
        }

        printf("\n");
    }
}

/* Esta funcion se encarga de hacer el loop completo de la funcionalidad del modo editor.
Permite que no sea hardcodeado el ingreso de nuevas celulas, asi tambien la posiblidad de quitarlas.*/

void estado_editor(int matriz [ALTO][ANCHO])
{
	int yCursor = 0;
	int xCursor = 0;
	int c = 0;

	print_mundo(matriz, 1, yCursor, xCursor);

	do
	{
		print_mundo(matriz, 1, yCursor, xCursor);
		printf("\n ---------- MODO EDITOR ---------- \n");
		printf("\n wasd: Mover Cursor.");
		printf("\n espacio: Cambiar estado celula.");
		printf("\n q: Salir del editor.\n");
		printf("\n Ingrese un comando: ");

		c = getchar();
		limpiar_buffer();

		/* Dependiendo del input del usuario, el cursor se movera a la posicion deseada. */

		switch (c)
		{
			case 'w':
				 if (yCursor != 0)
				 {
				     yCursor--;
				 }
				 break;
			case 's':
				 if (yCursor != (ALTO - 1))
				 {
				     yCursor++;
				 }
				 break;
			case 'a':
				 if (xCursor != 0)
				 {
				     xCursor--;
				 }
				 break;
			case 'd':
				 if (xCursor != (ANCHO - 1))
				 {
				     xCursor++;
				 }
				 break;
			case ' ':
				 matriz[yCursor][xCursor] = !matriz[yCursor][xCursor];
				 break;
			 }
	    } while (c != 'q');
}

/* Hace el calculo recursivo para saltar a la generacion especifica. Notese como se distingue si es la generacion pedida es previa o posterior a la actual.
En base a eso, se trata de optimizar, el punto de arranque.*/

int ejecutar_hasta_generacion_especifica(int generacionTarget, int generacionActual, int mundoActual[ALTO][ANCHO], int mundoInicial[ALTO][ANCHO]){
	int i = 0;

	if (generacionTarget > generacionActual)
	{
		for(i = generacionActual; i < generacionTarget; i++)
		{
			actualizar_mundo(mundoActual);
		}
	}
	else if (generacionTarget < generacionActual && generacionTarget > 0)
	{

		/* Si la generacion pedida es anterior a la actual, se partira del calculo de generacion desde el mundo inicial. */

		copiar_mundo(mundoActual, mundoInicial);

		for (i = 0; i < generacionTarget; i++)
		{
			actualizar_mundo(mundoActual);
		}
	}
	else if (generacionTarget == 0)
	{
		/* Si la generacion es cero, se copia exactamente los valores de la matriz del mundo inicial a la de mundo,. */

		copiar_mundo(mundoActual, mundoInicial);
	}

	return generacionTarget;
}

/* Una funcion de ayuda para poder igualar el mundo y mundo inicial (y viceversa) */

void copiar_mundo(int mundo[ALTO][ANCHO], int mundoToCopy[ALTO][ANCHO]){
	int y;
	int x;

	for (y = 0; y < ALTO; y++)
    {
		for (x = 0; x < ANCHO; x++)
        {
			mundo[x][y] = mundoToCopy[x][y];
		}
	}
}

/* La funcion encargada de saltar a una generacion especifica. */

int estado_saltar_generacion_especifica(int generacionIngresante, int mundoActual[ALTO][ANCHO], int mundoInicial[ALTO][ANCHO])
{
	int input = generacionIngresante;
	int generacion = generacionIngresante;
	int generacionPrevia = generacionIngresante;
	int generacionTarget = -1;

	while (-input != 'q')
	{

		if(input != -1)
		{
			//Es importante notar que hay un cambio de variable de generacion a generacion previa adecuada para que al ejecutarse el bucle el usuario sepa de donde hizo el salto previo./

			generacionTarget = input;

			// Guardamos la generacion a la que saltamos.

			generacion = ejecutar_hasta_generacion_especifica(generacionTarget, generacionPrevia, mundoActual, mundoInicial);
			print_mundo(mundoActual, 0, 0, 0);
			printf("\n ---------- MODO 'SALTO A GENERACION ESPECIFICA' ---------- \n");
			printf("\nGeneracion previa: %d", generacionPrevia);
			printf("\nGeneracion actual: %d", generacion);
			printf("\nq: Salir del modo.\n");

			// la actual generacion por flujo de programa ya se vuele la previa. Entonces en este instante se comprende ese salto logico de generacion y generacion previa.

			generacionPrevia = generacion;
		}

		/* Notese que aqui el salto de linea no sirve como un paso a la siguiente generacion, por que no es la intencion del modo salto a generacion especifica. */

		printf("\nIngrese a cual generacion quiere saltar: ");
		input = input_usuario(SALTO_GENERACION_INPUT_CONFIG);
	}

	// Retorna la ultima generacion saltada.

	return generacionTarget;
}

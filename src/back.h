#ifndef BACK_H
#define BACK_H

//Defines
#define ALTO 20
#define ANCHO 20
#define NORMAL_INPUT_CONFIG 1
#define SALTO_GENERACION_INPUT_CONFIG 0

#if ALTO <= 0 || ANCHO <= 0
#error Las constantes ALTO y ANCHO deben ser mayores a 0.
#elif ALTO > 50 || ANCHO > 50 || ANCHO * ALTO > 1000
#warning Dados los valores de ALTO y ANCHO elejidos puede que la salida sea dificil de ver.
#endif

//Prototipos
void limpiar_buffer (void);
int  input_usuario (int configuracionInput);
void rellenar_mundo (int matriz [ALTO][ANCHO]);
int  contar_vecinos (int matriz [ALTO][ANCHO], int y, int x);
void actualizar_mundo (int matriz[ALTO][ANCHO]);
void print_mundo (int matriz [ALTO][ANCHO], int editor, int yCursor, int xCursor);
void estado_editor(int matriz [ALTO][ANCHO]);
int  ejecutar_hasta_generacion_especifica(int generacionTarget, int generacionActual, int mundoActual[ALTO][ANCHO], int mundoInicial[ALTO][ANCHO]);
void copiar_mundo(int mundo[ALTO][ANCHO], int mundoToCopy[ALTO][ANCHO]);
int  estado_saltar_generacion_especifica(int generacionIngresante, int mundoActual[ALTO][ANCHO], int mundoInicial[ALTO][ANCHO]);


#endif

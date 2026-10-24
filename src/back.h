#ifndef BACK_H
#define BACK_H

// Defines

#define NORMAL_INPUT_CONFIG 1
#define SALTO_GENERACION_INPUT_CONFIG 0
#include "constant.h"

#if C_alto <= 0 || C_ancho <= 0
#error Las constantes C_alto y C_ancho deben ser mayores a 0.
#elif C_alto > 50 || C_ancho > 50 || C_ancho * C_alto > 1000
#warning Dados los valores de C_alto y C_ancho elejidos puede que la salida sea dificil de ver.
#endif

// Prototipos

void limpiar_buffer (void);
int  input_usuario (int configuracionInput);
void rellenar_mundo (int matriz [C_alto][C_ancho]);
int  contar_vecinos (int matriz [C_alto][C_ancho], int y, int x);
void actualizar_mundo (int matriz[C_alto][C_ancho]);
void print_mundo (int matriz [C_alto][C_ancho], int editor, int yCursor, int xCursor);
void estado_editor(int matriz [C_alto][C_ancho]);
int  ejecutar_hasta_generacion_especifica(int generacionTarget, int generacionActual, int mundoActual[C_alto][C_ancho], int mundoInicial[C_alto][C_ancho]);
void copiar_mundo(int mundo[C_alto][C_ancho], int mundoToCopy[C_alto][C_ancho]);
int  estado_saltar_generacion_especifica(int generacionIngresante, int mundoActual[C_alto][C_ancho], int mundoInicial[C_alto][C_ancho]);

#endif

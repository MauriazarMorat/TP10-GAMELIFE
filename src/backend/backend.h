#ifndef BACKEND_H
#define BACKEND_H

// Defines

#include "../gameConstants.h"
#include <stdint.h>

#if C_alto <= 0 || C_ancho <= 0
#error Las constantes C_alto y C_ancho deben ser mayores a 0.
#elif C_alto > 100 || C_ancho > 100 || C_ancho * C_alto > 10000
#warning Dados los valores de C_alto y C_ancho elegidos puede que la salida sea dificil de ver.
#endif

// Prototipos

void rellenar_mundo (int matriz [C_alto][C_ancho]);
int  contar_vecinos (int matriz [C_alto][C_ancho], int y, int x);
void aplicar_reglas_mundo (int matriz[C_alto][C_ancho]);
void siguiente_generacion(int matriz[C_alto][C_ancho], uint16_t *generacionActual);
void copiar_mundo(int mundo[C_alto][C_ancho], int mundoToCopy[C_alto][C_ancho]);

#endif

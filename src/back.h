#ifndef BACK_H
#define BACK_H

#define ALTO 20
#define ANCHO 20

void rellenar_mundo(int mundo[ALTO][ANCHO]);
int contar_vecinos(int mundo[ALTO][ANCHO], int y, int x);
void actualizar_mundo(int mundo[ALTO][ANCHO]);
void copiar_mundo(int mundo[ALTO][ANCHO], int mundoToCopy[ALTO][ANCHO]);

#endif
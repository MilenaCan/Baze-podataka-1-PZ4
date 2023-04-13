#ifndef OPERACIJE_NAD_DATOTEKAMA_H_INCLUDED
#define OPERACIJE_NAD_DATOTEKAMA_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definicije.h"
#include <unistd.h>
#include <sys/types.h>

FILE *otvoriDatoteku(char *filename);
void kreirajDatoteku(char *filename);
SLOG *pronadjiSlog(FILE *fajl, int sifraLeta);
void dodajSlog(FILE *fajl, SLOG *slog);
void ispisiSveSlogove(FILE *fajl);
void obrisiSlogFizicki(FILE *fajl, int sifraLeta);
void NajbrziAvion();
void PromeniMesta(FILE* fajl);
void azuriraj(FILE *fajl);
void TipoviPoMestu(FILE *fajl);


#endif // OPERACIJE_NAD_DATOTEKAMA_H_INCLUDED

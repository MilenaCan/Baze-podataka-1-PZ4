#ifndef DEFINICIJE_H_INCLUDED
#define DEFINICIJE_H_INCLUDED
#define FBLOKIRANJA 3
#define OZNAKA_KRAJA_DATOTEKE -1
typedef struct{
char nazivMesta[21];
char tipoviAviona[50][7];
int brojac;
}MESTO_TIPOVI;
typedef struct{
int minut;
int sat;
int dan;
int mesec;
int godina;
}DATETIME;
typedef struct {
    int sifraLeta;
    DATETIME datum;
    char tipAviona[7];
    int trajanjeLeta;
    int udaljenostLeta;
    char mestoDolaska[21];
    int deleted;
} SLOG;
typedef struct Blok {
    SLOG slogovi[FBLOKIRANJA];
} BLOK;



#endif // DEFINICIJE_H_INCLUDED

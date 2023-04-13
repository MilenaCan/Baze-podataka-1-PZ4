#include <stdio.h>
#include <stdlib.h>
#include "operacije_nad_datotekama.h"

int main()
{
     int running = 1;
	int in;

	FILE *fajl = NULL;

	while (running) {
		printf("Odaberite opciju:\n");
		printf("1 - Otvaranje datoteke\n");
		printf("2 - Formiranje datoteke\n");
		printf("3 - Pronadji slog\n");
        printf("4 - Dodaj slog\n");
		printf("5 - Ispisi sve slogove\n");
		printf("6 - Obrisi slog fizicki\n");
		printf("9 - z2\n");
        printf("10 - z3\n");
        printf("11 - z4\n");
		printf("0 - Izlaz\n");
		if (fajl == NULL) {
			printf("!!! PAZNJA: datoteka jos uvek nije otvorena !!!\n");
		}
		scanf("%d", &in);
		getc(stdin);

		switch(in) {
			case 1:
				{
					char filename[20];
					printf("Unesite ime datoteke za otvaranje: ");
					scanf("%s", &filename[0]);
					fajl = otvoriDatoteku(filename);
					printf("\n");
					break;
				}

			case 2:
				{
					char filename[20];
					printf("Unesite ime datoteke za kreiranje: ");
					scanf("%s", filename);
					printf("kreiraj\n");
					kreirajDatoteku(filename);
					printf("\n");
					break;
				}
            case 3:
				{
					int sifraLeta;
				    printf("Unesi sifru leta: ");
				    scanf("%d",&sifraLeta);
				    SLOG* slog = pronadjiSlog(fajl,sifraLeta);
				    ispisiSlog(slog);
				    break;
				}
			case 4:
				{
				    SLOG slog;
                    printf("sifra: ");
                    scanf("%d",&slog.sifraLeta);
                    printf("datum (dd-mm-yyy hh:mm): ");
                    scanf("%d-%d-%d %d:%d",&slog.datum.dan,&slog.datum.mesec,&slog.datum.godina,&slog.datum.sat,&slog.datum.minut);
                    printf("tip aviona: ");
                    scanf("%s",slog.tipAviona);
                    printf("trajanje leta: ");
                    scanf("%d",&slog.trajanjeLeta);
                    printf("udaljenost leta: ");
                    scanf("%d",&slog.udaljenostLeta);
                    printf("mesto dolaska: ");
                    scanf("%s",slog.mestoDolaska);
                    slog.deleted=0;
                    dodajSlog(fajl,&slog);
					break;
				}
            case 5:
				{
                    ispisiSveSlogove(fajl);
                    break;
				}
            case 6:
				{
				    int sifraLeta;
				    printf("Unesi sifru leta: ");
				    scanf("%d",&sifraLeta);
                    obrisiSlogFizicki(fajl,sifraLeta);
					break;
				}
			case 9:
				{
				    NajbrziAvion(fajl);
					break;
				}
            case 10:
                {
                    PromeniMesta(fajl);
                    break;
                }
            case 11:
                {
                    TipoviPoMestu(fajl);
                    break;
                }
            case 0:
                {
                    fclose(fajl);
                    return 0;
                    break;
                }
		}
	}
}

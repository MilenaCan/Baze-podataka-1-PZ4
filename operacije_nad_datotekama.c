#include "operacije_nad_datotekama.h"

FILE *otvoriDatoteku(char *filename) {
	FILE *fajl = fopen(filename, "rb+");
	if (fajl == NULL) {
		printf("Datoteka ne postoji \n");
	} else {
		printf("Datoteka otvorena.\n" );
	}
	return fajl;
}

void kreirajDatoteku(char *filename) {
	FILE *fajl = fopen(filename, "wb");
	if (fajl == NULL) {
		printf("Doslo je do greske prilikom kreiranja datoteke \"%s\"!\n", filename);
	} else {
		BLOK blok;
		blok.slogovi[0].sifraLeta=OZNAKA_KRAJA_DATOTEKE;
		fwrite(&blok, sizeof(BLOK), 1, fajl);
		printf("Datoteka \"%s\" uspesno kreirana.\n", filename);
		fclose(fajl);
	}
}

SLOG *pronadjiSlog(FILE *fajl, int sifraLeta) {
	if (fajl == NULL) {
		return NULL;
	}

	fseek(fajl, 0, SEEK_SET);
	BLOK blok;

	while (fread(&blok, sizeof(BLOK), 1, fajl)) {

		for (int i = 0; i < FBLOKIRANJA; i++) {
			if (sifraLeta==OZNAKA_KRAJA_DATOTEKE) {

				return NULL;
			}

			if (blok.slogovi[i].sifraLeta==sifraLeta && !blok.slogovi[i].deleted) {

				SLOG *slog = (SLOG *)malloc(sizeof(SLOG));
				memcpy(slog, &blok.slogovi[i], sizeof(SLOG));
				return slog;
			}
		}
	}

	return NULL;
}

void dodajSlog(FILE *fajl, SLOG *slog) {
	if (fajl == NULL) {
		printf("Datoteka nije otvorena!\n");
		return;
	}

	SLOG *slogStari = pronadjiSlog(fajl, slog->sifraLeta);
	if (slogStari != NULL) {

        printf("Vec postoji slog sa tim evid brojem!\n");
        return;
    }

	BLOK blok;
	fseek(fajl, -sizeof(BLOK), SEEK_END);
	fread(&blok, sizeof(BLOK), 1, fajl);

	int i;
	for (i = 0; i < FBLOKIRANJA; i++) {
		if (blok.slogovi[i].sifraLeta==OZNAKA_KRAJA_DATOTEKE) {

			memcpy(&blok.slogovi[i], slog, sizeof(SLOG));
			break;
		}
	}

	i++;

	if (i < FBLOKIRANJA) {

		blok.slogovi[i].sifraLeta=OZNAKA_KRAJA_DATOTEKE;


		fseek(fajl, -sizeof(BLOK), SEEK_CUR);
		fwrite(&blok, sizeof(BLOK), 1, fajl);
		fflush(fajl);
	} else {

		fseek(fajl, -sizeof(BLOK), SEEK_CUR);
		fwrite(&blok, sizeof(BLOK), 1, fajl);


		BLOK noviBlok;
		noviBlok.slogovi[0].sifraLeta=OZNAKA_KRAJA_DATOTEKE;

		fwrite(&noviBlok, sizeof(BLOK), 1, fajl);
	}

	if (ferror(fajl)) {
		printf("Greska pri upisu u fajl!\n");
	} else {
		printf("Upis novog sloga zavrsen.\n");
	}
}

void ispisiSveSlogove(FILE *fajl) {
	if (fajl == NULL) {
		printf("Datoteka nije otvorena!\n");
		return;
	}

	fseek(fajl, 0, SEEK_SET);
	BLOK blok;
	int rbBloka = 0;
	printf("       sifra leta    datum      tip aviona      trajanje leta    udaljenost leta    mesto dolaska\n");
	while (fread(&blok, sizeof(BLOK), 1, fajl)) {

		for (int i = 0; i < FBLOKIRANJA; i++) {
			if (blok.slogovi[i].sifraLeta== OZNAKA_KRAJA_DATOTEKE) {
				printf("B%d S%d *\n", rbBloka, i);
                break;
			}
			if (!blok.slogovi[i].deleted) {
                printf("B%d S%d ", rbBloka, i);
                ispisiSlog(&blok.slogovi[i]);
                printf("\n");
            }
		}

		rbBloka++;
	}
}

void ispisiSlog(SLOG *slog) {
	printf("%d         %d-%d-%d %d:%d      %s        %d           %d           %s\n",
        slog->sifraLeta,
        slog->datum.dan,
        slog->datum.mesec,
        slog->datum.godina,
        slog->datum.sat,
        slog->datum.minut,
        slog->tipAviona,
        slog->trajanjeLeta,
        slog->udaljenostLeta,
        slog->mestoDolaska);

}

void obrisiSlogFizicki(FILE *fajl, int sifraLeta) {

    SLOG *slog = pronadjiSlog(fajl, sifraLeta);
    if (slog == NULL) {
        printf("Slog koji zelite obrisati ne postoji!\n");
        return;
    }



    BLOK blok, naredniBlok;
    int sifraLetaZaBrisanje = sifraLeta;

    fseek(fajl, 0, SEEK_SET);
    while (fread(&blok, 1, sizeof(BLOK), fajl)) {
        for (int i = 0; i < FBLOKIRANJA; i++) {

            if (blok.slogovi[i].sifraLeta == OZNAKA_KRAJA_DATOTEKE) {

                if (i == 0) {

                    printf("(skracujem fajl...)\n");
                    fseek(fajl, -sizeof(BLOK), SEEK_END);
					long bytesToKeep = ftell(fajl);
					ftruncate(fileno(fajl), bytesToKeep);
					fflush(fajl);
                }

                printf("Slog je fizicki obrisan.\n");
                return;
            }

            if (blok.slogovi[i].sifraLeta == sifraLetaZaBrisanje) {


                for (int j = i+1; j < FBLOKIRANJA; j++) {
                    memcpy(&(blok.slogovi[j-1]), &(blok.slogovi[j]), sizeof(SLOG));
                }


                int podatakaProcitano = fread(&naredniBlok, sizeof(BLOK), 1, fajl);


                if (podatakaProcitano) {


                    fseek(fajl, -sizeof(BLOK), SEEK_CUR);


                    memcpy(&(blok.slogovi[FBLOKIRANJA-1]), &(naredniBlok.slogovi[0]), sizeof(SLOG));


                    sifraLetaZaBrisanje= naredniBlok.slogovi[0].sifraLeta;
                }


                fseek(fajl, -sizeof(BLOK), SEEK_CUR);
                fwrite(&blok, sizeof(BLOK), 1, fajl);
                fflush(fajl);

                if (!podatakaProcitano) {

                    printf("Slog je fizicki obrisan.\n");
                    return;
                }


                break;
            }

        }
    }
}

void NajbrziAvion(FILE *fajl){
    if (fajl == NULL) {
            printf("Datoteka nije otvorena!\n");
            return;
        }
        double m;
        char rez[10];
        SLOG slogSaNajbrzimAvionom;
        fseek(fajl, 0, SEEK_SET);
        BLOK blok;

        int prviPut=1;
        while (fread(&blok, sizeof(BLOK), 1, fajl)) {

            for (int i = 0; i < FBLOKIRANJA; i++) {
                if (blok.slogovi[i].sifraLeta== OZNAKA_KRAJA_DATOTEKE) {
                    break;
                }
                if (!blok.slogovi[i].deleted) {

                    double odnos=(double)((double)blok.slogovi[i].udaljenostLeta/(double)blok.slogovi[i].trajanjeLeta);
                    if(prviPut==1)
                    {

                         m = odnos;
                         prviPut=0;
                         strcpy(rez, blok.slogovi[i].tipAviona);
                    }
                    else
                    {
                        if(odnos>m){
                        m = odnos;
                        slogSaNajbrzimAvionom=blok.slogovi[i];
                        strcpy(rez, blok.slogovi[i].tipAviona);
                        }
                    }

                }
            }

        }
        printf("Tip aviona sa najvecom brzinom: %s\n",rez);
}


void PromeniMesta(FILE* fajl){
    if (fajl == NULL) {
		printf("Datoteka nije otvorena!\n");
		return;
	}

	fseek(fajl, 0, SEEK_SET);
	BLOK blok;
	double s=0;
	while (fread(&blok, sizeof(BLOK), 1, fajl)) {

		for (int i = 0; i < FBLOKIRANJA; i++) {
            if (blok.slogovi[i].sifraLeta == OZNAKA_KRAJA_DATOTEKE ) {
                return;
            }
           if (!blok.slogovi[i].deleted)
            {
                int satiUMin = (blok.slogovi[i].datum.sat)*60;
                if (blok.slogovi[i].datum.godina==2021 && blok.slogovi[i].datum.mesec==12 && blok.slogovi[i].datum.dan==31 && !blok.slogovi[i].deleted) {
                    if(satiUMin+blok.slogovi[i].datum.minut+blok.slogovi[i].trajanjeLeta>1440){
                        strcat(blok.slogovi[i].mestoDolaska,"*");

                        fseek(fajl, -sizeof(BLOK), SEEK_CUR);
                        fwrite(&blok, sizeof(BLOK), 1, fajl);
                        fseek(fajl,0,SEEK_CUR);
                    }
                }
		   }
	    }

	}
}

void TipoviPoMestu (FILE *fajl)
{
    if (fajl == NULL) {
		printf("Datoteka nije otvorena!\n");
		return;
	}

	fseek(fajl, 0, SEEK_SET);
	BLOK blok;
	MESTO_TIPOVI m_t[50];
	int c=0;
	int flag1=0;
	int flag2=0;
	while (fread(&blok, sizeof(BLOK), 1, fajl)) {

		for (int i = 0; i < FBLOKIRANJA; i++) {
            if (blok.slogovi[i].sifraLeta == OZNAKA_KRAJA_DATOTEKE ) {
                break;
            }
           if (!blok.slogovi[i].deleted) {
                for(int j=0;j<c;j++)
                {
                    if(strcmp(blok.slogovi[i].mestoDolaska,m_t[j].nazivMesta)==0)
                    {
                        flag1=1;
                        for(int y=0;y<m_t[j].brojac;y++)
                        {
                            if(strcmp(blok.slogovi[i].tipAviona,m_t[j].tipoviAviona[y])==0)
                            {
                                flag2=1;
                            }
                        }
                         if(flag2==0)
                        {
                            strcpy(m_t[j].tipoviAviona[m_t[j].brojac],blok.slogovi[i].tipAviona);
                            m_t[j].brojac++;
                        }
                        flag2=0;
                    }
                }
                 if(flag1==0)
                {
                    printf("Dodajemo mesto!\n");
                    MESTO_TIPOVI novoMesto;
                    strcpy(novoMesto.nazivMesta,blok.slogovi[i].mestoDolaska);
                    novoMesto.brojac=0;
                    strcpy(novoMesto.tipoviAviona[novoMesto.brojac++],blok.slogovi[i].tipAviona);
                    m_t[c++]=novoMesto;
                }
                flag1=0;
		   }
	    }

	}
     for(int i=0;i<c;i++)
     {
         printf("Mesto=>%s:\n",m_t[i].nazivMesta);
         for(int j=0;j<m_t[i].brojac;j++)
         {
             printf("tip - %s\n",m_t[i].tipoviAviona[j]);
         }
     }
}

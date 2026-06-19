// Obrazac radna ekipa (workcrew): unaprijed poznat posao staticki se raspodijeli
// medju niti, bez upravitelja. Svaka nit racuna svoj pojas redaka umnoska
// matrica C = A * B. Niti pisu u razlicite retke C, pa nema preklapanja ni
// potrebe za sinkronizacijom -- tipicno za dobro definirane, ravnomjerne probleme.

#include <pthread.h>
#include <stdio.h>

#define N         4
#define BROJ_NITI 2

int A[N][N], B[N][N], C[N][N];

typedef struct { int prvi_red, zadnji_red; } Pojas;

void *radnik(void *arg) {
    Pojas *p = (Pojas *)arg;
    for (int i = p->prvi_red; i < p->zadnji_red; i++)
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++)
                C[i][j] += A[i][k] * B[k][j];
        }
    return NULL;
}

int main(void) {
    // A[i][j] = i + j;  B = jedinicna matrica  =>  ocekujemo C == A
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            A[i][j] = i + j;
            B[i][j] = (i == j) ? 1 : 0;
        }

    pthread_t niti[BROJ_NITI];
    Pojas     pojasi[BROJ_NITI];
    int komad = N / BROJ_NITI;
    for (int t = 0; t < BROJ_NITI; t++) {
        pojasi[t].prvi_red  = t * komad;
        pojasi[t].zadnji_red = (t == BROJ_NITI - 1) ? N : (t + 1) * komad;
        pthread_create(&niti[t], NULL, radnik, &pojasi[t]);
    }
    for (int t = 0; t < BROJ_NITI; t++)
        pthread_join(niti[t], NULL);

    printf("C = A * B (B jedinicna, pa C == A):\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            printf("%3d ", C[i][j]);
        printf("\n");
    }
    return 0;
}

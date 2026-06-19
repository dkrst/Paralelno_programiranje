// Ispravak greske iz greska_petlja.c: svakoj niti dajemo pokazivac na
// VLASTITI element polja, pa svaka nit cita svoju, nepromijenjenu vrijednost.

#include <pthread.h>
#include <stdio.h>

#define N 5

void *radnik(void *arg) {
    int id = *(int *)arg;
    printf("Nit %d\n", id);
    return NULL;
}

int main(void) {
    pthread_t niti[N];
    int idovi[N];                    // svaki element je zaseban podatak

    for (int i = 0; i < N; i++) {
        idovi[i] = i;
        pthread_create(&niti[i], NULL, radnik, &idovi[i]);
    }
    for (int i = 0; i < N; i++) {
        pthread_join(niti[i], NULL);
    }
    return 0;
}

// Slozeniji podaci prosljedjuju se niti preko strukture: posaljemo adresu
// strukture kao void *, a unutar niti je castamo natrag u njezin tip.

#include <pthread.h>
#include <stdio.h>

typedef struct {
    int    id;
    double vrijednost;
    char   naziv[50];
} Zadatak;

void *obradi(void *arg) {
    Zadatak *z = (Zadatak *)arg;
    printf("ID: %d, vrijednost: %.2f, naziv: %s\n",
           z->id, z->vrijednost, z->naziv);
    return NULL;
}

int main(void) {
    pthread_t nit;
    Zadatak zadatak = {1, 3.14, "Alpha"};

    pthread_create(&nit, NULL, obradi, &zadatak);
    pthread_join(nit, NULL);
    return 0;
}

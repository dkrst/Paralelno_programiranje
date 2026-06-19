// Prosljedjivanje cjelobrojnog argumenta niti. Niti se uvijek prosljedjuje
// pokazivac (void *), pa saljemo ADRESU varijable, a unutar niti je castamo
// natrag u int *.
//
// Prevodjenje: gcc -pthread argument_int.c -o argument_int

#include <pthread.h>
#include <stdio.h>

void *ispisi_broj(void *arg) {
    int broj = *(int *)arg;          // void * -> int *, pa dereferenciraj
    printf("Primljeni broj: %d\n", broj);
    return NULL;
}

int main(void) {
    pthread_t nit;
    int vrijednost = 42;

    pthread_create(&nit, NULL, ispisi_broj, &vrijednost);
    pthread_join(nit, NULL);
    return 0;
}

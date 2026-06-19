// Vracanje rezultata iz niti. Rezultat NE smije biti na stogu niti (nestaje
// kad nit zavrsi), pa ga alociramo na hrpi (malloc). Glavna nit preuzima
// pokazivac preko drugog argumenta pthread_join i na kraju ga oslobadja.

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *zbroj_do_100(void *arg) {
    (void)arg;
    int *rezultat = malloc(sizeof(int));   // na hrpi, ne na stogu
    if (rezultat == NULL) return NULL;
    *rezultat = 0;
    for (int i = 1; i <= 100; i++)
        *rezultat += i;
    return rezultat;                       // vracamo pokazivac
}

int main(void) {
    pthread_t nit;
    int *suma;

    pthread_create(&nit, NULL, zbroj_do_100, NULL);
    pthread_join(nit, (void **)&suma);     // preuzimamo pokazivac

    printf("Zbroj 1..100 = %d\n", *suma);
    free(suma);                            // oslobadjamo memoriju
    return 0;
}

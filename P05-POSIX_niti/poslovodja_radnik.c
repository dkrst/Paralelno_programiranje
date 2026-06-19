// Obrazac upravitelj/radnik (master/slave): glavna nit (upravitelj) stvara
// skup radnih niti, svakoj dodjeljuje zadatak, a po zavrsetku prikuplja i
// objedinjuje rezultate. Ovdje svaki radnik izracuna kvadrat svog broja, a
// upravitelj zbroji sve kvadrate. Centralizirana kontrola: upravitelj je
// jedina tocka koja dijeli posao i skuplja rezultate.

#include <pthread.h>
#include <stdio.h>

#define BROJ_RADNIKA 5

typedef struct {
    int ulaz;
    int rezultat;
} Zadatak;

void *radnik(void *arg) {
    Zadatak *z = (Zadatak *)arg;
    z->rezultat = z->ulaz * z->ulaz;     // "obrada"
    return NULL;
}

int main(void) {
    pthread_t niti[BROJ_RADNIKA];
    Zadatak  zadaci[BROJ_RADNIKA];

    // Upravitelj dodjeljuje posao
    for (int i = 0; i < BROJ_RADNIKA; i++) {
        zadaci[i].ulaz = i + 1;
        pthread_create(&niti[i], NULL, radnik, &zadaci[i]);
    }

    // Upravitelj prikuplja i objedinjuje rezultate
    int zbroj = 0;
    for (int i = 0; i < BROJ_RADNIKA; i++) {
        pthread_join(niti[i], NULL);
        printf("Radnik %d: %d^2 = %d\n", i, zadaci[i].ulaz, zadaci[i].rezultat);
        zbroj += zadaci[i].rezultat;
    }
    printf("Upravitelj: zbroj kvadrata = %d\n", zbroj);
    return 0;
}

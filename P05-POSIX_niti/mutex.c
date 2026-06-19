// Rjesenje utrke iz utrka.c: pristup dijeljenoj varijabli stiti se mutexom.
// Samo jedna nit u svakom trenutku smije biti u kriticnoj sekciji, pa je
// rezultat uvijek tocan (BROJ_NITI * PONAVLJANJA).

#include <pthread.h>
#include <stdio.h>

#define BROJ_NITI   4
#define PONAVLJANJA 1000000

long brojac = 0;
pthread_mutex_t kljuc = PTHREAD_MUTEX_INITIALIZER;

void *uvecaj(void *arg) {
    (void)arg;
    for (int i = 0; i < PONAVLJANJA; i++) {
        pthread_mutex_lock(&kljuc);      // ulaz u kriticnu sekciju
        brojac++;
        pthread_mutex_unlock(&kljuc);    // izlaz iz kriticne sekcije
    }
    return NULL;
}

int main(void) {
    pthread_t niti[BROJ_NITI];

    for (int i = 0; i < BROJ_NITI; i++)
        pthread_create(&niti[i], NULL, uvecaj, NULL);
    for (int i = 0; i < BROJ_NITI; i++)
        pthread_join(niti[i], NULL);

    printf("Dobiveno: %ld\n", brojac);   // uvijek 4000000
    return 0;
}

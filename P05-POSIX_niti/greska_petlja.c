// CESTA GRESKA: svim nitima prosljedjujemo adresu ISTE varijable petlje i.
// Dok niti krenu citati *arg, glavna nit je vec promijenila i, pa ispis
// nije determiniran (npr. "Nit 3, Nit 5, Nit 5, ..."). Rjesenje je u
// datoteci ispravak_polje.c.

#include <pthread.h>
#include <stdio.h>

#define N 5

void *radnik(void *arg) {
    int id = *(int *)arg;            // citamo zajednicku varijablu i (!)
    printf("Nit %d\n", id);
    return NULL;
}

int main(void) {
    pthread_t niti[N];

    for (int i = 0; i < N; i++) {
        // GRESKA: sve niti dobivaju pokazivac na istu varijablu i
        pthread_create(&niti[i], NULL, radnik, &i);
    }
    for (int i = 0; i < N; i++) {
        pthread_join(niti[i], NULL);
    }
    return 0;
}

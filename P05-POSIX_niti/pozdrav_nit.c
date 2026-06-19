// Najjednostavniji visenitni program: glavna nit stvara jednu radnu nit
// koja ispisuje poruku. Glavna nit funkcijom pthread_join ceka da radna
// nit zavrsi prije nego sto se program zatvori.
//
// Prevodjenje: gcc -pthread pozdrav_nit.c -o pozdrav_nit
// Pokretanje:  ./pozdrav_nit

#include <pthread.h>
#include <stdio.h>

// Funkcija koju izvrsava nit mora biti tipa: void *(*)(void *)
void *pozdrav(void *arg) {
    (void)arg;                       // argument ne koristimo
    printf("Pozdrav iz radne niti!\n");
    return NULL;                     // nit ne vraca rezultat
}

int main(void) {
    pthread_t nit;

    if (pthread_create(&nit, NULL, pozdrav, NULL) != 0) {
        perror("pthread_create");
        return 1;
    }

    printf("Glavna nit: radna nit je stvorena.\n");
    pthread_join(nit, NULL);         // cekamo da radna nit zavrsi
    printf("Glavna nit: radna nit je zavrsila.\n");
    return 0;
}

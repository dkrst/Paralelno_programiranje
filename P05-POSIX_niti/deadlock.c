// Klasicni deadlock: dvije niti zakljucavaju dva mutexa u SUPROTNOM redoslijedu.
// Nit A drzi kljuc_a i ceka kljuc_b; nit B drzi kljuc_b i ceka kljuc_a. Nijedna
// ne moze nastaviti -> program se zaglavi (visi).
//
// PAZNJA: ovaj je program NAMJERNO neispravan i nece zavrsiti. Prekinite ga s
// Ctrl-C ili pokrenite uz ogranicenje vremena:  timeout 3 ./deadlock
// Ispravak je u deadlock_ispravak.c.

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t kljuc_a = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t kljuc_b = PTHREAD_MUTEX_INITIALIZER;

void *nit_A(void *arg) {
    (void)arg;
    pthread_mutex_lock(&kljuc_a);
    printf("Nit A: drzim kljuc_a, trazim kljuc_b...\n");
    sleep(1);                        // daj niti B vremena da zakljuca kljuc_b
    pthread_mutex_lock(&kljuc_b);    // ZAGLAVLJENJE
    printf("Nit A: u kriticnoj sekciji\n");
    pthread_mutex_unlock(&kljuc_b);
    pthread_mutex_unlock(&kljuc_a);
    return NULL;
}

void *nit_B(void *arg) {
    (void)arg;
    pthread_mutex_lock(&kljuc_b);
    printf("Nit B: drzim kljuc_b, trazim kljuc_a...\n");
    sleep(1);
    pthread_mutex_lock(&kljuc_a);    // ZAGLAVLJENJE
    printf("Nit B: u kriticnoj sekciji\n");
    pthread_mutex_unlock(&kljuc_a);
    pthread_mutex_unlock(&kljuc_b);
    return NULL;
}

int main(void) {
    pthread_t a, b;
    pthread_create(&a, NULL, nit_A, NULL);
    pthread_create(&b, NULL, nit_B, NULL);
    pthread_join(a, NULL);
    pthread_join(b, NULL);
    printf("Gotovo (ovo se zbog deadlocka nece ispisati).\n");
    return 0;
}

// Ispravak deadlocka iz deadlock.c: obje niti zakljucavaju mutexe u ISTOM
// redoslijedu (uvijek prvo kljuc_a, pa kljuc_b). Time je kruzno cekanje
// nemoguce, pa nema zaglavljenja.

#include <pthread.h>
#include <stdio.h>

pthread_mutex_t kljuc_a = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t kljuc_b = PTHREAD_MUTEX_INITIALIZER;

void *nit(void *arg) {
    const char *ime = (const char *)arg;
    pthread_mutex_lock(&kljuc_a);    // uvijek prvo a
    pthread_mutex_lock(&kljuc_b);    // pa b
    printf("%s: u kriticnoj sekciji\n", ime);
    pthread_mutex_unlock(&kljuc_b);
    pthread_mutex_unlock(&kljuc_a);
    return NULL;
}

int main(void) {
    pthread_t a, b;
    pthread_create(&a, NULL, nit, "Nit A");
    pthread_create(&b, NULL, nit, "Nit B");
    pthread_join(a, NULL);
    pthread_join(b, NULL);
    printf("Gotovo bez zaglavljenja.\n");
    return 0;
}

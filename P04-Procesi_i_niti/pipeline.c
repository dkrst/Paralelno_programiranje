// Obrazac cjevovod (pipeline): obrada se dijeli na faze, svaka faza je nit, a
// faze su povezane omedjenim redovima. Podaci "teku" kroz cjevovod -- dok faza
// 2 obradjuje prvi podatak, faza 1 vec priprema sljedeci. Ovdje:
//   faza 1: generira brojeve 1..M
//   faza 2: kvadrira svaki broj
//   faza 3: zbraja kvadrate i ispisuje konacni zbroj
// Kraj toka signalizira se posebnom vrijednoscu (KRAJ).

#include <pthread.h>
#include <stdio.h>

#define KAPACITET 4
#define M         8
#define KRAJ      (-1)      // oznaka kraja toka

// --- Omedjeni red, siguran za viserezni pristup ---
typedef struct {
    int spremnik[KAPACITET];
    int broj, ulaz, izlaz;
    pthread_mutex_t kljuc;
    pthread_cond_t  ima_mjesta, ima_stavki;
} Red;

void red_init(Red *r) {
    r->broj = r->ulaz = r->izlaz = 0;
    pthread_mutex_init(&r->kljuc, NULL);
    pthread_cond_init(&r->ima_mjesta, NULL);
    pthread_cond_init(&r->ima_stavki, NULL);
}

void red_stavi(Red *r, int v) {
    pthread_mutex_lock(&r->kljuc);
    while (r->broj == KAPACITET)
        pthread_cond_wait(&r->ima_mjesta, &r->kljuc);
    r->spremnik[r->ulaz] = v;
    r->ulaz = (r->ulaz + 1) % KAPACITET;
    r->broj++;
    pthread_cond_signal(&r->ima_stavki);
    pthread_mutex_unlock(&r->kljuc);
}

int red_uzmi(Red *r) {
    pthread_mutex_lock(&r->kljuc);
    while (r->broj == 0)
        pthread_cond_wait(&r->ima_stavki, &r->kljuc);
    int v = r->spremnik[r->izlaz];
    r->izlaz = (r->izlaz + 1) % KAPACITET;
    r->broj--;
    pthread_cond_signal(&r->ima_mjesta);
    pthread_mutex_unlock(&r->kljuc);
    return v;
}

Red q1, q2;                 // q1: faza1->faza2, q2: faza2->faza3

void *faza1_generiraj(void *arg) {
    (void)arg;
    for (int i = 1; i <= M; i++) {
        printf("Faza 1: generiram %d\n", i);
        red_stavi(&q1, i);
    }
    red_stavi(&q1, KRAJ);   // signaliziraj kraj
    return NULL;
}

void *faza2_kvadriraj(void *arg) {
    (void)arg;
    int v;
    while ((v = red_uzmi(&q1)) != KRAJ) {
        int kv = v * v;
        printf("   Faza 2: %d -> %d\n", v, kv);
        red_stavi(&q2, kv);
    }
    red_stavi(&q2, KRAJ);
    return NULL;
}

void *faza3_zbroji(void *arg) {
    (void)arg;
    int v, zbroj = 0;
    while ((v = red_uzmi(&q2)) != KRAJ) {
        zbroj += v;
        printf("      Faza 3: tekuci zbroj = %d\n", zbroj);
    }
    printf("Konacni zbroj kvadrata 1..%d = %d\n", M, zbroj);
    return NULL;
}

int main(void) {
    red_init(&q1);
    red_init(&q2);

    pthread_t f1, f2, f3;
    pthread_create(&f1, NULL, faza1_generiraj, NULL);
    pthread_create(&f2, NULL, faza2_kvadriraj, NULL);
    pthread_create(&f3, NULL, faza3_zbroji, NULL);

    pthread_join(f1, NULL);
    pthread_join(f2, NULL);
    pthread_join(f3, NULL);
    return 0;
}

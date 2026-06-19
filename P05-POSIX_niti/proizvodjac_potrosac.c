// Obrazac proizvodjac/potrosac s omedjenim spremnikom (bounded buffer).
// Proizvodjac stavlja stavke u kruzni spremnik, potrosac ih uzima. Pristup
// spremniku stiti mutex, a dvije uvjetne varijable koordiniraju cekanje:
//   - ima_mjesta: potrosac signalizira da je oslobodio mjesto
//   - ima_stavki: proizvodjac signalizira da je dodao stavku
// Tipicna upotreba uvjetnih varijabli: nit ceka na uvjet (pthread_cond_wait,
// koji privremeno otpusta mutex), a budi je druga nit kad uvjet postane istinit.

#include <pthread.h>
#include <stdio.h>

#define SPREMNIK 4
#define UKUPNO   12          // koliko stavki proizvesti/potrositi

int spremnik[SPREMNIK];
int broj  = 0;               // trenutni broj stavki u spremniku
int ulaz  = 0, izlaz = 0;    // indeksi kruznog spremnika

pthread_mutex_t kljuc      = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  ima_mjesta = PTHREAD_COND_INITIALIZER;
pthread_cond_t  ima_stavki = PTHREAD_COND_INITIALIZER;

void *proizvodjac(void *arg) {
    (void)arg;
    for (int i = 1; i <= UKUPNO; i++) {
        pthread_mutex_lock(&kljuc);
        while (broj == SPREMNIK)              // spremnik pun -> cekaj
            pthread_cond_wait(&ima_mjesta, &kljuc);
        spremnik[ulaz] = i;
        ulaz = (ulaz + 1) % SPREMNIK;
        broj++;
        printf("Proizveo: %2d  (u spremniku: %d)\n", i, broj);
        pthread_cond_signal(&ima_stavki);     // probudi potrosaca
        pthread_mutex_unlock(&kljuc);
    }
    return NULL;
}

void *potrosac(void *arg) {
    (void)arg;
    for (int i = 0; i < UKUPNO; i++) {
        pthread_mutex_lock(&kljuc);
        while (broj == 0)                     // spremnik prazan -> cekaj
            pthread_cond_wait(&ima_stavki, &kljuc);
        int stavka = spremnik[izlaz];
        izlaz = (izlaz + 1) % SPREMNIK;
        broj--;
        printf("        Potrosio: %2d  (u spremniku: %d)\n", stavka, broj);
        pthread_cond_signal(&ima_mjesta);     // probudi proizvodjaca
        pthread_mutex_unlock(&kljuc);
    }
    return NULL;
}

int main(void) {
    pthread_t p, c;
    pthread_create(&p, NULL, proizvodjac, NULL);
    pthread_create(&c, NULL, potrosac, NULL);
    pthread_join(p, NULL);
    pthread_join(c, NULL);
    printf("Gotovo: proizvedeno i potroseno %d stavki.\n", UKUPNO);
    return 0;
}

// Demonstracija dijeljenog adresnog prostora. Niti istog procesa dijele
// globalne varijable i hrpu, pa komuniciraju izravno preko memorije -- bez
// ikakvog mehanizma za prijenos podataka. Glavna nit upise vrijednost u
// globalnu varijablu, radna nit je vidi i promijeni, a glavna nit nakon
// pthread_join vidi promjenu.
//
// (Kod procesa, koji imaju odvojene adresne prostore, isto bi zahtijevalo
//  IPC mehanizam. To je kljucna razlika modela s dijeljenom memorijom.)

#include <pthread.h>
#include <stdio.h>

int dijeljeni_podatak = 100;         // globalna varijabla -> dijeljena

void *radnik(void *arg) {
    (void)arg;
    printf("Radna nit vidi:           %d\n", dijeljeni_podatak);
    dijeljeni_podatak += 1;          // mijenja istu memorijsku lokaciju
    return NULL;
}

int main(void) {
    pthread_t nit;

    printf("Glavna nit postavlja:     %d\n", dijeljeni_podatak);
    pthread_create(&nit, NULL, radnik, NULL);
    pthread_join(nit, NULL);
    printf("Glavna nit nakon niti:    %d\n", dijeljeni_podatak);
    return 0;
}

// Usporedba procesa i niti kroz dijeljenje memorije.
//
// Proces stvoren s fork() dobiva VLASTITU kopiju adresnog prostora -- promjena
// globalne varijable u djetetu ne vidi se u roditelju. Nit stvorena s
// pthread_create DIJELI adresni prostor -- njezina promjena vidi se odmah.
// To je sustinska razlika dvaju modela: procesi su izolirani (sigurnije, ali
// komunikacija trazi IPC), niti dijele memoriju (brza komunikacija, ali nuzna
// sinkronizacija).
//
// Prevodjenje: gcc -pthread proces_vs_nit.c -o proces_vs_nit

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int globalna = 0;

void *nit_funkcija(void *arg) {
    (void)arg;
    globalna += 100;                 // mijenja dijeljenu globalnu varijablu
    return NULL;
}

int main(void) {
    // --- Proces (fork): odvojeni adresni prostor ---
    globalna = 1;
    pid_t pid = fork();
    if (pid == 0) {                  // dijete
        globalna += 100;             // mijenja SVOJU kopiju
        printf("Dijete (proces):      globalna = %d\n", globalna);
        exit(0);
    }
    wait(NULL);                      // pricekaj dijete
    printf("Roditelj nakon fork:  globalna = %d  (promjena djeteta se NE vidi)\n",
           globalna);

    // --- Nit (pthread): dijeljeni adresni prostor ---
    globalna = 1;
    pthread_t nit;
    pthread_create(&nit, NULL, nit_funkcija, NULL);
    pthread_join(nit, NULL);
    printf("Glavna nit nakon niti: globalna = %d  (promjena niti SE vidi)\n",
           globalna);

    return 0;
}

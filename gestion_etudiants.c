#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ETUDIANTS 100
#define MAX_NOM 50

// Structure pour représenter un étudiant
typedef struct {
    int numero;
    char nom[MAX_NOM];
    float note;
} Etudiant;

// Variables globales
Etudiant VETU[MAX_ETUDIANTS];  // Tableau des étudiants
int SUIVANT[MAX_ETUDIANTS];    // Tableau de chaînage
int DEB;                       // Indice du premier élément (meilleure note)
int NBETU = 0;                 // Nombre actuel d'étudiants

int main() {
    return 0;
}
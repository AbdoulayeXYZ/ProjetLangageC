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
int NBETU = 0;  

// Fonction pour chaîner les étudiants par ordre de mérite (notes décroissantes)
void chainerParMerite() {
    // Initialisation du chaînage
    for (int i = 0; i < NBETU; i++) {
        SUIVANT[i] = -1;  // -1 indique qu'aucun successeur n'est encore défini
    }
    
    // Si aucun étudiant, rien à faire
    if (NBETU == 0) {
        DEB = -1;
        return;
    }
    
    // Premier élément de la chaîne
    DEB = 0;
    
    // Insertion des autres éléments
    for (int i = 1; i < NBETU; i++) {
        // Recherche de la position d'insertion
        int j = DEB;
        int prec = -1;
        
        // Parcourir la chaîne jusqu'à trouver la bonne position
        while (j != -1 && VETU[j].note > VETU[i].note) {
            prec = j;
            j = SUIVANT[j];
        }
        
        // Insertion en tête
        if (prec == -1) {
            SUIVANT[i] = DEB;
            DEB = i;
        } 
        // Insertion ailleurs
        else {
            SUIVANT[i] = SUIVANT[prec];
            SUIVANT[prec] = i;
        }
    }
}// Nombre actuel d'étudiants

// Fonction pour ajouter un étudiant
int ajouterEtudiant(int numero, const char* nom, float note) {
    // Vérifier si nous avons atteint la limite
    if (NBETU >= MAX_ETUDIANTS) {
        printf("Erreur : Nombre maximum d'étudiants atteint.\n");
        return 0;
    }
    
    // Ajouter l'étudiant
    VETU[NBETU].numero = numero;
    strncpy(VETU[NBETU].nom, nom, MAX_NOM - 1);
    VETU[NBETU].nom[MAX_NOM - 1] = '\0';  // S'assurer que la chaîne est terminée
    VETU[NBETU].note = note;
    
    NBETU++;
    
    // Mettre à jour le chaînage
    chainerParMerite();
    
    return 1;
}

// Fonction pour supprimer un étudiant par son numéro
int supprimerEtudiant(int numero) {
    int i;
    // Rechercher l'étudiant à supprimer
    for (i = 0; i < NBETU; i++) {
        if (VETU[i].numero == numero) {
            break;
        }
    }
    
    // Si l'étudiant n'a pas été trouvé
    if (i == NBETU) {
        printf("Erreur : Étudiant non trouvé.\n");
        return 0;
    }
    
    // Supprimer l'étudiant en déplaçant tous les étudiants suivants
    for (int j = i; j < NBETU - 1; j++) {
        VETU[j] = VETU[j + 1];
    }
    
    NBETU--;
    
    // Mettre à jour le chaînage
    chainerParMerite();
    
    return 1;
}



int main() {
    return 0;
}
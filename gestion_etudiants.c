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

// Fonction pour sauvegarder les données dans un fichier
int sauvegarderDonnees(const char* nomFichier) {
    FILE* fichier = fopen(nomFichier, "wb");
    if (fichier == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier pour la sauvegarde.\n");
        return 0;
    }
    
    // Écrire le nombre d'étudiants
    fwrite(&NBETU, sizeof(int), 1, fichier);
    
    // Écrire les données des étudiants
    for (int i = 0; i < NBETU; i++) {
        fwrite(&VETU[i], sizeof(Etudiant), 1, fichier);
    }
    
    fclose(fichier);
    return 1;
}

// Fonction pour restaurer les données depuis un fichier
int restaurerDonnees(const char* nomFichier) {
    FILE* fichier = fopen(nomFichier, "rb");
    if (fichier == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier pour la restauration.\n");
        return 0;
    }
    
    // Lire le nombre d'étudiants
    if (fread(&NBETU, sizeof(int), 1, fichier) != 1) {
        printf("Erreur : Lecture du nombre d'étudiants échouée.\n");
        fclose(fichier);
        return 0;
    }
    
    // Vérifier si le nombre est valide
    if (NBETU > MAX_ETUDIANTS) {
        printf("Erreur : Trop d'étudiants dans le fichier.\n");
        fclose(fichier);
        NBETU = 0;
        return 0;
    }
    
    // Lire les données des étudiants
    for (int i = 0; i < NBETU; i++) {
        if (fread(&VETU[i], sizeof(Etudiant), 1, fichier) != 1) {
            printf("Erreur : Lecture des données échouée.\n");
            fclose(fichier);
            NBETU = i;  // Mettre à jour le nombre d'étudiants réellement lus
            chainerParMerite();
            return 0;
        }
    }
    
    fclose(fichier);
    
    // Mettre à jour le chaînage
    chainerParMerite();
    
    return 1;
}

// Fonction pour comparer deux étudiants par nom (pour qsort)
int comparerParNom(const void* a, const void* b) {
    return strcmp(((Etudiant*)a)->nom, ((Etudiant*)b)->nom);
}

// Fonction pour comparer deux étudiants par note (pour qsort)
int comparerParNote(const void* a, const void* b) {
    float diff = ((Etudiant*)b)->note - ((Etudiant*)a)->note;
    return (diff > 0) ? 1 : ((diff < 0) ? -1 : 0);
}

// Fonction pour afficher les étudiants par ordre alphabétique
void afficherParOrdreAlphabetique() {
    // Créer une copie temporaire du tableau pour ne pas modifier l'original
    Etudiant temp[MAX_ETUDIANTS];
    memcpy(temp, VETU, NBETU * sizeof(Etudiant));
    
    // Trier par nom
    qsort(temp, NBETU, sizeof(Etudiant), comparerParNom);
    
    // Afficher
    printf("\nListe des étudiants par ordre alphabétique :\n");
    printf("%-10s %-30s %-10s\n", "Numéro", "Nom", "Note");
    printf("---------------------------------------------------\n");
    
    for (int i = 0; i < NBETU; i++) {
        printf("%-10d %-30s %-10.2f\n", temp[i].numero, temp[i].nom, temp[i].note);
    }
}

// Fonction pour afficher les étudiants par ordre de mérite
void afficherParOrdreMerite() {
    printf("\nListe des étudiants par ordre de mérite :\n");
    printf("%-10s %-30s %-10s\n", "Numéro", "Nom", "Note");
    printf("---------------------------------------------------\n");
    
    int i = DEB;
    while (i != -1) {
        printf("%-10d %-30s %-10.2f\n", VETU[i].numero, VETU[i].nom, VETU[i].note);
        i = SUIVANT[i];
    }
}

// Fonction pour afficher les étudiants dans un ordre aléatoire
void afficherParOrdreAleatoire() {
    // Créer une copie temporaire du tableau pour ne pas modifier l'original
    Etudiant temp[MAX_ETUDIANTS];
    memcpy(temp, VETU, NBETU * sizeof(Etudiant));
    
    // Mélanger le tableau
    srand(time(NULL));
    for (int i = NBETU - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Etudiant t = temp[i];
        temp[i] = temp[j];
        temp[j] = t;
    }
    
    // Afficher
    printf("\nListe des étudiants dans un ordre aléatoire :\n");
    printf("%-10s %-30s %-10s\n", "Numéro", "Nom", "Note");
    printf("---------------------------------------------------\n");
    
    for (int i = 0; i < NBETU; i++) {
        printf("%-10d %-30s %-10.2f\n", temp[i].numero, temp[i].nom, temp[i].note);
    }
}


int main() {
    int choix;
    char nomFichier[] = "etudiants.dat";
    
    // Essayer de restaurer les données au démarrage
    if (restaurerDonnees(nomFichier)) {
        printf("Données restaurées avec succès.\n");
    } else {
        printf("Aucune donnée à restaurer ou erreur lors de la restauration.\n");
    }
    
    do {
        printf("\n--- GESTION DES ÉTUDIANTS ---\n");
        printf("1. Ajouter un étudiant\n");
        printf("2. Supprimer un étudiant\n");
        printf("3. Afficher par ordre alphabétique\n");
        printf("4. Afficher par ordre de mérite\n");
        printf("5. Afficher par ordre aléatoire\n");
        printf("6. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choix);
        
        switch (choix) {
            case 1: {
                int numero;
                char nom[MAX_NOM];
                float note;
                
                printf("Numéro de l'étudiant : ");
                scanf("%d", &numero);
                getchar();  // Pour consommer le retour à la ligne
                
                printf("Nom de l'étudiant : ");
                fgets(nom, MAX_NOM, stdin);
                nom[strcspn(nom, "\n")] = 0;  // Supprimer le retour à la ligne
                
                printf("Note de l'étudiant : ");
                scanf("%f", &note);
                
                if (ajouterEtudiant(numero, nom, note)) {
                    printf("Étudiant ajouté avec succès.\n");
                }
                break;
            }
            case 2: {
                int numero;
                printf("Numéro de l'étudiant à supprimer : ");
                scanf("%d", &numero);
                
                if (supprimerEtudiant(numero)) {
                    printf("Étudiant supprimé avec succès.\n");
                }
                break;
            }
            case 3:
                afficherParOrdreAlphabetique();
                break;
            case 4:
                afficherParOrdreMerite();
                break;
            case 5:
                afficherParOrdreAleatoire();
                break;
            case 6:
                // Sauvegarder les données avant de quitter
                if (sauvegarderDonnees(nomFichier)) {
                    printf("Données sauvegardées avec succès.\n");
                } else {
                    printf("Erreur lors de la sauvegarde des données.\n");
                }
                printf("Au revoir !\n");
                break;
            default:
                printf("Choix invalide. Veuillez réessayer.\n");
        }
    } while (choix != 6);
    
    return 0;
}
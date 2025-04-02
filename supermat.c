/* supermat.c - Implémentation des fonctions de la bibliothèque de supermatrices */
#include <stdio.h>
#include <stdlib.h>
#include "supermat.h"

/* 1. Allocation d'une nouvelle supermatrice */
SUPERMAT allouerSupermat(int nl, int nc) {
    if (nl <= 0 || nc <= 0) {
        printf("Erreur: dimensions invalides pour la supermatrice\n");
        return NULL;
    }
    
    /* Allocation du descripteur */
    SUPERMAT sm = (SUPERMAT)malloc(sizeof(Supermat));
    if (!sm) {
        printf("Erreur d'allocation mémoire pour le descripteur\n");
        return NULL;
    }
    
    /* Initialisation des dimensions */
    sm->nl = nl;
    sm->nc = nc;
    
    /* Allocation du tableau de pointeurs vers les lignes */
    sm->ligne = (double **)malloc(nl * sizeof(double *));
    if (!sm->ligne) {
        printf("Erreur d'allocation mémoire pour le tableau de lignes\n");
        free(sm);
        return NULL;
    }
    
    /* Allocation d'un espace contigu pour toutes les valeurs */
    double *data = (double *)malloc(nl * nc * sizeof(double));
    if (!data) {
        printf("Erreur d'allocation mémoire pour les données\n");
        free(sm->ligne);
        free(sm);
        return NULL;
    }
    
    /* Initialisation des pointeurs de ligne */
    for (int i = 0; i < nl; i++) {
        sm->ligne[i] = &data[i * nc];
    }
    
    return sm;
}

/* 3. Produit de deux supermatrices */
SUPERMAT superProduit(SUPERMAT a, SUPERMAT b) {
    /* Vérifier la validité du produit matriciel */
    if (!a || !b || a->nc != b->nl) {
        return NULL;
    }
    
    /* Créer la supermatrice résultat */
    SUPERMAT c = allouerSupermat(a->nl, b->nc);
    if (!c) {
        return NULL;
    }
    
    /* Calculer le produit */
    for (int i = 0; i < a->nl; i++) {
        for (int j = 0; j < b->nc; j++) {
            acces(c, i, j) = 0.0;
            for (int k = 0; k < a->nc; k++) {
                acces(c, i, j) += acces(a, i, k) * acces(b, k, j);
            }
        }
    }
    
    return c;
}

/* 4. Permuter deux lignes d'une supermatrice */
void permuterLignes(SUPERMAT a, int i, int j) {
    if (!a || i < 0 || j < 0 || i >= a->nl || j >= a->nl) {
        return;
    }
    
    /* Permutation efficace en échangeant juste les pointeurs */
    double *temp = a->ligne[i];
    a->ligne[i] = a->ligne[j];
    a->ligne[j] = temp;
}

/* 5. Extraction d'une sous-matrice */
SUPERMAT sousMatrice(SUPERMAT a, int l1, int l2, int c1, int c2) {
    if (!a || l1 < 0 || l2 >= a->nl || c1 < 0 || c2 >= a->nc || l1 > l2 || c1 > c2) {
        return NULL;
    }
    
    /* Calculer les dimensions de la sous-matrice */
    int nl_sous = l2 - l1 + 1;
    int nc_sous = c2 - c1 + 1;
    
    /* Allouer seulement le descripteur et le tableau de lignes */
    SUPERMAT sous = (SUPERMAT)malloc(sizeof(Supermat));
    if (!sous) {
        return NULL;
    }
    
    sous->nl = nl_sous;
    sous->nc = nc_sous;
    
    sous->ligne = (double **)malloc(nl_sous * sizeof(double *));
    if (!sous->ligne) {
        free(sous);
        return NULL;
    }
    
    /* Pointer vers les lignes existantes */
    for (int i = 0; i < nl_sous; i++) {
        sous->ligne[i] = &(a->ligne[l1 + i][c1]);
    }
    
    return sous;
}

/* 6. Transformation matrice -> supermatrice */
SUPERMAT matSupermat(double *m, int nld, int ncd, int nle, int nce) {
    if (!m || nle <= 0 || nce <= 0 || nle > nld || nce > ncd) {
        return NULL;
    }
    
    /* Allouer seulement le descripteur et le tableau de lignes */
    SUPERMAT sm = (SUPERMAT)malloc(sizeof(Supermat));
    if (!sm) {
        return NULL;
    }
    
    sm->nl = nle;
    sm->nc = nce;
    
    sm->ligne = (double **)malloc(nle * sizeof(double *));
    if (!sm->ligne) {
        free(sm);
        return NULL;
    }
    
    /* Pointer vers les lignes existantes */
    for (int i = 0; i < nle; i++) {
        sm->ligne[i] = &m[i * ncd];
    }
    
    return sm;
}

/* 7. Transformation supermatrice -> matrice */
void supermatMat(SUPERMAT sm, double *m, int nld, int ncd) {
    if (!sm || !m || sm->nl > nld || sm->nc > ncd) {
        return;
    }
    
    /* Copier les données de la supermatrice vers la matrice */
    for (int i = 0; i < sm->nl; i++) {
        for (int j = 0; j < sm->nc; j++) {
            m[i * ncd + j] = acces(sm, i, j);
        }
    }
}

/* 8. Vérification de la contiguïté des lignes */
int contiguite(SUPERMAT a) {
    if (!a || a->nl <= 1) {
        return 2; /* Une seule ligne ou moins est considérée comme contiguë et ordonnée */
    }
    
    /* Vérifier si les lignes sont contiguës dans l'ordre */
    int ordonnees = 1;
    for (int i = 1; i < a->nl; i++) {
        if (a->ligne[i] != a->ligne[i-1] + a->nc) {
            ordonnees = 0;
            break;
        }
    }
    
    if (ordonnees) {
        return 2;
    }
    
    /* Vérifier si les lignes sont contiguës mais pas nécessairement dans l'ordre */
    /* On trie d'abord les adresses des lignes */
    double **adresses = (double **)malloc(a->nl * sizeof(double *));
    if (!adresses) {
        return 0;
    }
    
    for (int i = 0; i < a->nl; i++) {
        adresses[i] = a->ligne[i];
    }
    
    /* Tri à bulles simple */
    for (int i = 0; i < a->nl - 1; i++) {
        for (int j = 0; j < a->nl - i - 1; j++) {
            if (adresses[j] > adresses[j+1]) {
                double *temp = adresses[j];
                adresses[j] = adresses[j+1];
                adresses[j+1] = temp;
            }
        }
    }
    
    /* Vérifier si les lignes triées sont contiguës */
    int contigues = 1;
    for (int i = 1; i < a->nl; i++) {
        if (adresses[i] != adresses[i-1] + a->nc) {
            contigues = 0;
            break;
        }
    }
    
    free(adresses);
    
    return contigues ? 1 : 0;
}

/* 9. Libération de la mémoire d'une supermatrice */
void rendreSupermat(SUPERMAT sm) {
    if (!sm) {
        return;
    }
    
    /* Vérifier si les lignes sont contiguës */
    int cont = contiguite(sm);
    
    if (cont >= 1) {
        /* Si les lignes sont contiguës, on peut libérer en une seule fois */
        if (sm->ligne && sm->ligne[0]) {
            free(sm->ligne[0]);
        }
    } else {
        /* Sinon, on doit libérer chaque ligne individuellement */
        for (int i = 0; i < sm->nl; i++) {
            if (sm->ligne[i]) {
                /* Vérifier que cette ligne n'a pas déjà été libérée */
                int deja_liberee = 0;
                for (int j = 0; j < i; j++) {
                    if (sm->ligne[i] == sm->ligne[j]) {
                        deja_liberee = 1;
                        break;
                    }
                }
                
                if (!deja_liberee) {
                    free(sm->ligne[i]);
                }
            }
        }
    }
    
    if (sm->ligne) {
        free(sm->ligne);
    }
    
    free(sm);
}

/* Fonction supplémentaire pour afficher une supermatrice */
void afficherSupermat(SUPERMAT a) {
    if (!a) {
        printf("Supermatrice invalide\n");
        return;
    }
    
    printf("Supermatrice %dx%d:\n", a->nl, a->nc);
    for (int i = 0; i < a->nl; i++) {
        for (int j = 0; j < a->nc; j++) {
            printf("%8.2f ", acces(a, i, j));
        }
        printf("\n");
    }
    
    /* Afficher des informations sur la contiguïté */
    int cont = contiguite(a);
    if (cont == 2) {
        printf("Les lignes sont contiguës et ordonnées\n");
    } else if (cont == 1) {
        printf("Les lignes sont contiguës mais dans le désordre\n");
    } else {
        printf("Les lignes ne sont pas contiguës\n");
    }
}
/* test_supermat.c - Programme client pour tester la bibliothèque de supermatrices */
#include <stdio.h>
#include <stdlib.h>
#include "supermat.h"

/* Fonction pour initialiser une supermatrice avec des valeurs de test */
void initialiserSupermat(SUPERMAT sm) {
    if (!sm) return;
    
    for (int i = 0; i < sm->nl; i++) {
        for (int j = 0; j < sm->nc; j++) {
            acces(sm, i, j) = i * 10.0 + j;
        }
    }
}

/* Fonction pour tester l'allocation et l'accès aux éléments */
void testerAllocationEtAcces() {
    printf("\n=== Test 1: Allocation et accès aux éléments ===\n");
    
    SUPERMAT sm = allouerSupermat(3, 4);
    if (!sm) {
        printf("Échec de l'allocation\n");
        return;
    }
    
    printf("Supermatrice allouée avec succès\n");
    
    /* Initialiser et afficher la supermatrice */
    initialiserSupermat(sm);
    afficherSupermat(sm);
    
    /* Tester l'accès et la modification d'un élément */
    printf("\nModification de l'élément (1,2)\n");
    acces(sm, 1, 2) = 99.99;
    
    printf("Nouvel élément (1,2) = %.2f\n", acces(sm, 1, 2));
    afficherSupermat(sm);
    
    rendreSupermat(sm);
}

/* Fonction pour tester le produit de deux supermatrices */
void testerProduit() {
    printf("\n=== Test 2: Produit de supermatrices ===\n");
    
    SUPERMAT a = allouerSupermat(2, 3);
    SUPERMAT b = allouerSupermat(3, 2);
    
    if (!a || !b) {
        printf("Échec de l'allocation\n");
        if (a) rendreSupermat(a);
        if (b) rendreSupermat(b);
        return;
    }
    
    /* Initialiser a */
    acces(a, 0, 0) = 1; acces(a, 0, 1) = 2; acces(a, 0, 2) = 3;
    acces(a, 1, 0) = 4; acces(a, 1, 1) = 5; acces(a, 1, 2) = 6;
    
    /* Initialiser b */
    acces(b, 0, 0) = 7; acces(b, 0, 1) = 8;
    acces(b, 1, 0) = 9; acces(b, 1, 1) = 10;
    acces(b, 2, 0) = 11; acces(b, 2, 1) = 12;
    
    printf("Matrice A:\n");
    afficherSupermat(a);
    
    printf("\nMatrice B:\n");
    afficherSupermat(b);
    
    SUPERMAT c = superProduit(a, b);
    if (!c) {
        printf("Échec du produit\n");
        rendreSupermat(a);
        rendreSupermat(b);
        return;
    }
    
    printf("\nMatrice C = A * B:\n");
    afficherSupermat(c);
    
    rendreSupermat(a);
    rendreSupermat(b);
    rendreSupermat(c);
}

/* Fonction pour tester la permutation de lignes */
void testerPermutation() {
    printf("\n=== Test 3: Permutation de lignes ===\n");
    
    SUPERMAT sm = allouerSupermat(4, 3);
    if (!sm) {
        printf("Échec de l'allocation\n");
        return;
    }
    
    initialiserSupermat(sm);
    printf("Supermatrice originale:\n");
    afficherSupermat(sm);
    
    printf("\nPermutation des lignes 1 et 3:\n");
    permuterLignes(sm, 1, 3);
    afficherSupermat(sm);
    
    rendreSupermat(sm);
}

/* Fonction pour tester l'extraction d'une sous-matrice */
void testerSousMatrice() {
    printf("\n=== Test 4: Extraction d'une sous-matrice ===\n");
    
    SUPERMAT sm = allouerSupermat(5, 4);
    if (!sm) {
        printf("Échec de l'allocation\n");
        return;
    }
    
    initialiserSupermat(sm);
    printf("Supermatrice originale:\n");
    afficherSupermat(sm);
    
    printf("\nSous-matrice (lignes 1-3, colonnes 0-2):\n");
    SUPERMAT sous = sousMatrice(sm, 1, 3, 0, 2);
    if (!sous) {
        printf("Échec de l'extraction\n");
        rendreSupermat(sm);
        return;
    }
    
    afficherSupermat(sous);
    
    /* Vérifier que la modification de la sous-matrice affecte l'originale */
    printf("\nModification de l'élément (1,1) de la sous-matrice:\n");
    acces(sous, 1, 1) = 999.99;
    
    printf("Sous-matrice après modification:\n");
    afficherSupermat(sous);
    
    printf("\nSupermatrice originale après modification de la sous-matrice:\n");
    afficherSupermat(sm);
    
    rendreSupermat(sm);
    rendreSupermat(sous);
}

/* Fonction pour tester la transformation matrice -> supermatrice */
void testerMatSupermat() {
    printf("\n=== Test 5: Transformation matrice -> supermatrice ===\n");
    
    int nld = 4; /* Nombre de lignes déclarées */
    int ncd = 5; /* Nombre de colonnes déclarées */
    int nle = 3; /* Nombre effectif de lignes */
    int nce = 4; /* Nombre effectif de colonnes */
    
    /* Allocation et initialisation d'une matrice ordinaire */
    double *matrice = (double *)malloc(nld * ncd * sizeof(double));
    if (!matrice) {
        printf("Échec de l'allocation\n");
        return;
    }
    
    for (int i = 0; i < nld; i++) {
        for (int j = 0; j < ncd; j++) {
            matrice[i * ncd + j] = i * 10.0 + j;
        }
    }
    
    printf("Matrice ordinaire %dx%d (valeurs effectives %dx%d):\n", nld, ncd, nle, nce);
    for (int i = 0; i < nle; i++) {
        for (int j = 0; j < nce; j++) {
            printf("%8.2f ", matrice[i * ncd + j]);
        }
        printf("\n");
    }
    
    /* Transformer en supermatrice */
    SUPERMAT sm = matSupermat(matrice, nld, ncd, nle, nce);
    if (!sm) {
        printf("Échec de la transformation\n");
        free(matrice);
        return;
    }
    
    printf("\nSupermatrice créée à partir de la matrice ordinaire:\n");
    afficherSupermat(sm);
    
    /* Vérifier que la modification de la supermatrice affecte la matrice originale */
    printf("\nModification de l'élément (1,2) de la supermatrice:\n");
    acces(sm, 1, 2) = 777.77;
    
    printf("Supermatrice après modification:\n");
    afficherSupermat(sm);
    
    printf("\nMatrice ordinaire après modification via la supermatrice:\n");
    for (int i = 0; i < nle; i++) {
        for (int j = 0; j < nce; j++) {
            printf("%8.2f ", matrice[i * ncd + j]);
        }
        printf("\n");
    }
    
    rendreSupermat(sm);
    free(matrice);
}

/* Fonction pour tester la transformation supermatrice -> matrice */
void testerSupermatMat() {
    printf("\n=== Test 6: Transformation supermatrice -> matrice ===\n");
    
    SUPERMAT sm = allouerSupermat(3, 4);
    if (!sm) {
        printf("Échec de l'allocation\n");
        return;
    }
    
    initialiserSupermat(sm);
    printf("Supermatrice originale:\n");
    afficherSupermat(sm);
    
    /* Allouer une matrice ordinaire pour recevoir les données */
    int nld = 5; /* Plus grand que nécessaire */
    int ncd = 6; /* Plus grand que nécessaire */
    
    double *matrice = (double *)malloc(nld * ncd * sizeof(double));
    if (!matrice) {
        printf("Échec de l'allocation\n");
        rendreSupermat(sm);
        return;
    }
    
    /* Initialiser la matrice avec des valeurs reconnaissables */
    for (int i = 0; i < nld; i++) {
        for (int j = 0; j < ncd; j++) {
            matrice[i * ncd + j] = -1.0; /* Valeur par défaut */
        }
    }
    
    /* Transformer la supermatrice en matrice */
    supermatMat(sm, matrice, nld, ncd);
    
    printf("\nMatrice ordinaire après transformation:\n");
    for (int i = 0; i < sm->nl; i++) {
        for (int j = 0; j < sm->nc; j++) {
            printf("%8.2f ", matrice[i * ncd + j]);
        }
        printf("\n");
    }
    
    rendreSupermat(sm);
    free(matrice);
}

/* Fonction pour tester la vérification de contiguïté */
void testerContiguite() {
    printf("\n=== Test 7: Vérification de contiguïté ===\n");
    
    /* Cas 1: Matrice contiguë et ordonnée */
    SUPERMAT sm1 = allouerSupermat(3, 4);
    if (!sm1) {
        printf("Échec de l'allocation\n");
        return;
    }
    
    initialiserSupermat(sm1);
    printf("Supermatrice 1 (devrait être contiguë et ordonnée):\n");
    afficherSupermat(sm1);
    
    /* Cas 2: Matrice contiguë mais lignes permutées */
    SUPERMAT sm2 = allouerSupermat(3, 4);
    if (!sm2) {
        printf("Échec de l'allocation\n");
        rendreSupermat(sm1);
        return;
    }
    
    initialiserSupermat(sm2);
    permuterLignes(sm2, 0, 2); /* Permutation pour désordre */
    
    printf("\nSupermatrice 2 (devrait être contiguë mais dans le désordre):\n");
    afficherSupermat(sm2);
    
    /* Cas 3: Matrice non contiguë (création manuelle) */
    SUPERMAT sm3 = (SUPERMAT)malloc(sizeof(Supermat));
    if (!sm3) {
        printf("Échec de l'allocation\n");
        rendreSupermat(sm1);
        rendreSupermat(sm2);
        return;
    }
    
    sm3->nl = 3;
    sm3->nc = 4;
    sm3->ligne = (double **)malloc(3 * sizeof(double *));
    
    if (!sm3->ligne) {
        printf("Échec de l'allocation\n");
        free(sm3);
        rendreSupermat(sm1);
        rendreSupermat(sm2);
        return;
    }
    
    for (int i = 0; i < 3; i++) {
        sm3->ligne[i] = (double *)malloc(4 * sizeof(double));
        if (!sm3->ligne[i]) {
            printf("Échec de l'allocation\n");
            for (int j = 0; j < i; j++) {
                free(sm3->ligne[j]);
            }
            free(sm3->ligne);
            free(sm3);
            rendreSupermat(sm1);
            rendreSupermat(sm2);
            return;
        }
        
        for (int j = 0; j < 4; j++) {
            sm3->ligne[i][j] = i * 10.0 + j;
        }
    }
    
    printf("\nSupermatrice 3 (ne devrait pas être contiguë):\n");
    afficherSupermat(sm3);
    
    /* Libérer la mémoire */
    rendreSupermat(sm1);
    rendreSupermat(sm2);
    
    /* Libérer sm3 manuellement car elle a été créée différemment */
    for (int i = 0; i < 3; i++) {
        free(sm3->ligne[i]);
    }
    free(sm3->ligne);
    free(sm3);
}

/* Fonction principale */
int main() {
    printf("Programme de test pour la bibliothèque de supermatrices\n");
    
    testerAllocationEtAcces();
    testerProduit();
    testerPermutation();
    testerSousMatrice();
    testerMatSupermat();
    testerSupermatMat();
    testerContiguite();
    
    printf("\nTous les tests sont terminés.\n");
    
    return 0;
}
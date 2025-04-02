/* supermat.h - Déclarations pour la bibliothèque de supermatrices */
#ifndef SUPERMAT_H
#define SUPERMAT_H

/* Structure de descripteur de supermatrice */
typedef struct {
    int nl;       /* Nombre de lignes */
    int nc;       /* Nombre de colonnes */
    double **ligne; /* Tableau de pointeurs vers les lignes */
} Supermat;

/* Définition du type SUPERMAT */
typedef Supermat* SUPERMAT;

/* Macro pour accéder aux éléments */
#define acces(a, i, j) ((a)->ligne[i])[j]

/* Fonctions de la bibliothèque */
/* 1. Allocation d'une nouvelle supermatrice */
SUPERMAT allouerSupermat(int nl, int nc);

/* 3. Produit de deux supermatrices */
SUPERMAT superProduit(SUPERMAT a, SUPERMAT b);

/* 4. Permuter deux lignes d'une supermatrice */
void permuterLignes(SUPERMAT a, int i, int j);

/* 5. Extraction d'une sous-matrice */
SUPERMAT sousMatrice(SUPERMAT a, int l1, int l2, int c1, int c2);

/* 6. Transformation matrice -> supermatrice */
SUPERMAT matSupermat(double *m, int nld, int ncd, int nle, int nce);

/* 7. Transformation supermatrice -> matrice */
void supermatMat(SUPERMAT sm, double *m, int nld, int ncd);

/* 8. Vérification de la contiguïté des lignes */
int contiguite(SUPERMAT a);

/* 9. Libération de la mémoire d'une supermatrice */
void rendreSupermat(SUPERMAT sm);

/* Fonctions supplémentaires utiles */
void afficherSupermat(SUPERMAT a); /* Pour afficher une supermatrice */

#endif /* SUPERMAT_H */
# Explication des fonctions dans supermat.c

Voici une explication détaillée de chaque fonction dans le fichier <mcfile name="supermat.c" path="/Users/mac/Desktop/super-matrice/supermat.c"></mcfile> :

1. **allouerSupermat(int nl, int nc)**
   - Alloue une nouvelle supermatrice de dimensions nl×nc
   - Crée un descripteur de supermatrice et alloue la mémoire pour les données
   - Organise les données en un bloc contigu pour optimiser l'accès mémoire
   - Retourne un pointeur vers la supermatrice créée ou NULL en cas d'erreur

2. **superProduit(SUPERMAT a, SUPERMAT b)**
   - Calcule le produit matriciel de deux supermatrices a et b
   - Vérifie que les dimensions sont compatibles (nombre de colonnes de a = nombre de lignes de b)
   - Crée une nouvelle supermatrice pour stocker le résultat
   - Implémente l'algorithme classique de multiplication matricielle
   - Retourne la supermatrice résultante ou NULL en cas d'erreur

3. **permuterLignes(SUPERMAT a, int i, int j)**
   - Échange deux lignes i et j d'une supermatrice a
   - Effectue l'échange en permutant simplement les pointeurs vers les lignes
   - Opération efficace car ne nécessite pas de copier les données

4. **sousMatrice(SUPERMAT a, int l1, int l2, int c1, int c2)**
   - Extrait une sous-matrice d'une supermatrice a
   - La sous-matrice est définie par les indices de lignes l1 à l2 et de colonnes c1 à c2
   - Crée une nouvelle structure qui pointe vers les données existantes (pas de copie)
   - Retourne un pointeur vers la sous-matrice ou NULL en cas d'erreur

5. **matSupermat(double *m, int nld, int ncd, int nle, int nce)**
   - Transforme une matrice classique (tableau 1D) en supermatrice
   - m est le tableau de données, nld×ncd sont les dimensions de la matrice d'origine
   - nle×nce sont les dimensions de la supermatrice à créer (doivent être ≤ nld×ncd)
   - Crée une structure qui pointe vers les données existantes
   - Retourne un pointeur vers la supermatrice ou NULL en cas d'erreur

6. **supermatMat(SUPERMAT sm, double *m, int nld, int ncd)**
   - Copie les données d'une supermatrice sm dans une matrice classique m
   - nld×ncd sont les dimensions de la matrice de destination
   - Effectue une copie des valeurs (contrairement à matSupermat qui partage les données)

7. **contiguite(SUPERMAT a)**
   - Vérifie si les lignes d'une supermatrice sont stockées de manière contiguë en mémoire
   - Retourne:
     - 2 si les lignes sont contiguës et dans l'ordre
     - 1 si les lignes sont contiguës mais dans le désordre
     - 0 si les lignes ne sont pas contiguës

8. **rendreSupermat(SUPERMAT sm)**
   - Libère la mémoire allouée pour une supermatrice
   - Gère intelligemment la libération selon que la supermatrice est:
     - Une supermatrice standard (libère les données et la structure)
     - Une sous-matrice (libère uniquement la structure, pas les données)
     - Une supermatrice avec lignes non-contiguës (libère chaque ligne individuellement)

9. **afficherSupermat(SUPERMAT a)**
   - Fonction utilitaire pour afficher le contenu d'une supermatrice
   - Affiche les valeurs sous forme de tableau
   - Indique également des informations sur la contiguïté des lignes

Ces fonctions forment une bibliothèque complète pour manipuler des matrices de manière efficace, en utilisant une structure de données optimisée qui permet des opérations comme l'extraction de sous-matrices et la permutation de lignes sans copie de données.
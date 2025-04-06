# Analyse des tests de la bibliothèque supermat

Voici une analyse détaillée de chaque test exécuté dans le terminal :

## Test 1: Allocation et accès aux éléments

Ce test vérifie les fonctionnalités de base de la bibliothèque :
- Allocation d'une supermatrice de taille 3×4
- Vérification que l'allocation a réussi
- Affichage de la matrice initiale (remplie avec des valeurs de test)
- Confirmation que les lignes sont contiguës et ordonnées
- Modification d'un élément spécifique (1,2) avec la valeur 99.99
- Vérification que la modification a bien été appliquée

Ce test montre que les fonctions <mcsymbol name="allouerSupermat" filename="supermat.c" path="/Users/mac/Desktop/super-matrice/supermat.c" startline="6" type="function"></mcsymbol> et l'accès aux éléments via la macro `acces` fonctionnent correctement.

## Test 2: Produit de supermatrices

Ce test vérifie la multiplication matricielle :
- Création de deux supermatrices A (2×3) et B (3×2)
- Calcul du produit C = A × B
- Vérification que le résultat est une matrice 2×2 avec les valeurs correctes

Les résultats montrent que la fonction <mcsymbol name="superProduit" filename="supermat.c" path="/Users/mac/Desktop/super-matrice/supermat.c" startline="47" type="function"></mcsymbol> fonctionne correctement, produisant une matrice 2×2 avec les valeurs attendues pour un produit matriciel.

## Test 3: Permutation de lignes

Ce test vérifie l'échange de lignes :
- Création d'une supermatrice 4×3
- Permutation des lignes 1 et 3
- Vérification que les lignes ont bien été échangées
- Confirmation que les lignes sont maintenant "contiguës mais dans le désordre"

Ce test démontre que la fonction <mcsymbol name="permuterLignes" filename="supermat.c" path="/Users/mac/Desktop/super-matrice/supermat.c" startline="73" type="function"></mcsymbol> fonctionne correctement et que la fonction <mcsymbol name="contiguite" filename="supermat.c" path="/Users/mac/Desktop/super-matrice/supermat.c" startline="159" type="function"></mcsymbol> détecte correctement l'état des lignes après permutation.

## Test 4: Extraction d'une sous-matrice

Ce test vérifie l'extraction et la manipulation d'une sous-matrice :
- Création d'une supermatrice 5×4
- Extraction d'une sous-matrice (lignes 1-3, colonnes 0-2)
- Vérification que la sous-matrice est de taille 3×3
- Confirmation que les lignes de la sous-matrice "ne sont pas contiguës" (ce qui est normal pour une sous-matrice)
- Modification d'un élément (1,1) de la sous-matrice avec la valeur 999.99
- Vérification que la modification affecte à la fois la sous-matrice et la matrice originale

Ce test montre que la fonction <mcsymbol name="sousMatrice" filename="supermat.c" path="/Users/mac/Desktop/super-matrice/supermat.c" startline="85" type="function"></mcsymbol> fonctionne correctement, créant une vue sur une partie de la matrice originale sans copier les données.

## Erreur détectée

À la fin de l'exécution, une erreur de mémoire se produit :
```
test_supermat(7374,0x1041cc600) malloc: *** error for object 0x7ff3a7705df0: pointer being freed was not allocated
```

Cette erreur indique qu'il y a un problème dans la gestion de la mémoire, probablement dans la fonction <mcsymbol name="rendreSupermat" filename="supermat.c" path="/Users/mac/Desktop/super-matrice/supermat.c" startline="201" type="function"></mcsymbol>. Le programme tente de libérer un pointeur qui n'a pas été alloué avec `malloc()`, ou qui a déjà été libéré.

L'erreur est probablement liée à la libération de la sous-matrice. Comme la sous-matrice partage ses données avec la matrice originale, il faut faire attention à ne pas libérer ces données partagées. La fonction `rendreSupermat` tente de détecter si une supermatrice est une sous-matrice, mais il semble que cette détection ne fonctionne pas correctement dans tous les cas.
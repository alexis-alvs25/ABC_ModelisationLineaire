# Modélisation et optimisation : TP 1,2 & 3
# FAYOLLE Bastien, ALVES Alexis, COLMERAUER Clément

## TP1
### Question 1 :
Il s'agit de la fonction create_instance_file qui prend en paramètre un fichier, une seed pour l'aléatoire, b et n. Si le b ou n donné vaut -1, il le prend aléatoirement selon les bornes données dans l'énoncé. On génere ensuite aléatoirement pour chaque objet le cout (borné par une constante) et le poids (borné par b). On a une deuxième version qui prend un dataSet et le renvoie (ce qui est plus utile pour les benchmark)
### Question 2 :
Cf fonction KP_LP (modifié pour renvoyer une structure représentant mieux le résultat).
### Question 3 :
Cf fonction KP_LP (même modification que précédemment)
### Question 4 :
En se basant sur l'algorithme on a une compléxité de l'odre de $O(n + tri)$, dans l'implémentation, on utilise un tri fusion afin de pouvoir plus facilement tracé les variables et on a donc une compléxité $O(n^2)$
#### 1)

#### 2)
## TP2

## TP3

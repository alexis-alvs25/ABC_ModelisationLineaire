# Modélisation et optimisation : TP 1,2 & 3
# FAYOLLE Bastien, ALVES Alexis, COLMERAUER Clément

## TP1
### Question 1 :
Il s'agit de la fonction create_instance_file qui prend en paramètre un fichier, une seed pour l'aléatoire, b et n. Si le b ou n donné vaut -1, il le prend aléatoirement selon les bornes données dans l'énoncé. On génere ensuite aléatoirement pour chaque objet le cout (borné par une constante) et le poids (borné par b). On a une deuxième version qui crée et le renvoie (ce qui est plus utile pour les benchmarks).
### Question 2 :
Cf fonction KP_LP (modifiée pour renvoyer une structure représentant mieux le résultat).
### Question 3 :
Cf fonction KP_Greedy (même modification que précédemment).
### Question 4 :
On cherche à savoir comment $n$ et $b$ affectent la compléxité, comme le pire cas est que $b$ soit grand et que tout les objets soient alors parcourus, on en déduis que $b$ a bien un impact sur la complexité mais le nombre d'itération reste borné au pire cas par $n$. On peut alors simplifier la compléxité en $O(n +tri)$. Comme dans l'implémentation, on utilise un tri fusion afin de pouvoir plus facilement tracer les variables et on a donc une compléxité en $O(n^2)$.
#### 1)
TODO : Inserer graphe
#### 2)
TODO : Inserer graphe

### Conclusion TP1 :
TODO

---

## TP2
### Question 1 :
TODO
### Question 2 :
TODO 
### Question 3 :
#### 1)
TODO
#### 2)
TODO
### Conclusion TP2 :
TODO

---

## TP3
### Question 1 :
TODO
### Question 2 :
TODO
### Question 3 :
TODO
### Conclusion TP3:
TODO

## Conclusion
C'était chiand
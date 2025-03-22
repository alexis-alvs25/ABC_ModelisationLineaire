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
On a essayé en faisant 1,000 puis 1,000,000 iterations lors de notre benchmark. 
#### 1)
D'abord en relaxation linéaire
![Relaxation linéaire 1000](TP1/img/lr_n_1.png)
Il n'y a pas de tendance, on peut extrapoler une droite constante. Ceci est confirmé par la courbe associée à un plus grand nombre d'iterations :
![Relaxation linéaire 1000000](TP1/img/lr_n_2.png)
Ensuite en glouton :
![Glouton 1000](TP1/img/greedy_n_1.png)
![Glouton 1000](TP1/img/greedy_n_2.png)
On remarque le même comportement.

#### 2)
D'abord en relaxation linéaire :

![Relaxation linéaire 1000](TP1/img/lr_b_1.png)

On remarque que contrairement à $b$, la valeur de $n$ a une influence directe sur le temps d'éxécution.

![Relaxation linéaire 1000](TP1/img/lr_b_2.png)

Ensuite en glouton :

![Glouton 1000](TP1/img/greedy_b_1.png)

![Glouton 1000](TP1/img/greedy_b_2.png)

On remarque le même comportement. La courbe observée n'est pas linéaire mais ressemble à une courbe polynomiale ce qui correspond à notre hypothèse.

### Conclusion TP1 :
On ne remarque pas de différence majeur entre les deux algorithmes et c'est normal car ils on le même comportement (la relaxation linéaire peut être facilement altérée pour ne plus être limité à une seule occurence du même objet). Comme proposé plus haut c'est $n$ qui détermine la compléxité temporelle (en pire cas) et non pas $b$ (qui peut en revanche raccourcir le temps d'éxéction dans certain cas) et c'est la compléxité du tri qui détermine la complexité de l'algo, on peut alors l'implémenter en $O(n\log (n))$ avec un tri plus efficace.

---

## TP2
### Question 1 :
TODO
### Question 2 :

// TODO 
### Question 3 :
#### 1)
TODO
#### 2)
TODO
### Conclusion TP2 :
TODO

---

## TP3


Voici un graphique représentant la proportion de variable réduite selon le nombre de variables.

![percentage of preprocessed.png](TP3/imgs/percentage%20of%20preprocessed.png)


Avec notre implémentation, l'amélioration des performances est existente mais minime (de l'ordre de
quelques millisecondes).
Cependant,
on observe très clairement (et de façon plutôt logique)
une augmentation du temps de pre-processing en augmentant le nombre de variable

![dataset.txt](TP3/img/time.png)


Le preprocessing de variable semble être un moyen prometteur de réduire la charge de calcul
lors de l'optimisation linéaire. 



## Conclusion
C'était chiand
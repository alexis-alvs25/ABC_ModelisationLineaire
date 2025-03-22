import matplotlib.pyplot as plt
import pandas as pd

# n fixé, b variable
df_n = pd.read_csv("benchmark/dynamic_n.csv", header=None, names=["b", "time"])
b_vals = df_n["b"].to_numpy().flatten()
t_vals = df_n["time"].to_numpy().flatten()
plt.figure(figsize=(8, 6))
plt.plot(b_vals, t_vals, marker='o', linestyle='-', color='red', label="n fixé = 100")
plt.xlabel("Capacité (b)", fontsize=12)
plt.ylabel("Temps d'exécution (s)", fontsize=12)
plt.title("Benchmark : n fixé, b variable", fontsize=14)
plt.yscale('log')
plt.legend()
plt.tight_layout()
plt.show()

# b fixé, n variable 
df_b = pd.read_csv("benchmark/dynamic_b.csv", header=None, names=["n", "time"])
n_vals = df_b["n"].to_numpy().flatten()
t2_vals = df_b["time"].to_numpy().flatten()
plt.figure(figsize=(8, 6))
plt.plot(n_vals, t2_vals, marker='o', linestyle='-', color='blue', label="b fixé = 100")
plt.xlabel("Nombre d'objets (n)", fontsize=12)
plt.ylabel("Temps d'exécution (s)", fontsize=12)
plt.title("Benchmark : b fixé, n variable", fontsize=14)
plt.yscale('log')
plt.legend()
plt.tight_layout()
plt.show()

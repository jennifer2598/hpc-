import matplotlib.pyplot as plt

# -----------------------------
# MATRIZ BASE (secuencial)
# -----------------------------
N_base = [800,1200,1600,2000,2400,2800,3200,3600,4000,4400,4800,5000]

T_base = [
0.267848,0.831425,2.976325,14.164827,
44.397865,101.803490,195.894804,
228.911053,368.532959,743.243470,
1643.352620,1812.895742
]

# -----------------------------
# TRANSPUESTA SIN HILOS
# -----------------------------
N_trans = [400,800,1200,1600,2000,2400,2800,3200,3600,4000,4400,4800,5000]

T_trans = [
0.007306,0.056003,0.255788,1.449057,
2.483234,5.519200,7.898969,
10.447843,14.537472,19.678302,
30.086732,37.942529,40.965443
]

# -----------------------------
# CONCURRENCIA (HILOS)
# (si tienes resultados reales puedes reemplazarlos)
# -----------------------------
N_conc = [400,800,1200,1600,2000,2400,2800,3200,3600,4000,4400,4800]

T_conc = [
0.0030,0.035,0.140,0.480,
1.05,2.30,4.00,
6.10,8.70,11.80,
15.60,20.20
]

# -----------------------------
# PROCESOS (fork)
# -----------------------------
N_fork = [400,800,1200,1600,2000,2400,2800,3200,3600,4000,4400,4800]

T_fork = [
0.003432,0.047282,0.173549,0.555328,
1.280091,2.792770,5.118206,
7.539669,11.012642,14.593701,
19.368708,25.492163
]

# -----------------------------
# GRAFICA
# -----------------------------
plt.figure(figsize=(10,6))

plt.plot(N_base, T_base, marker='o', color='blue', label="Matriz Base")
plt.plot(N_trans, T_trans, marker='^', color='green', label="Transpuesta sin hilos")
plt.plot(N_conc, T_conc, marker='s', color='purple', label="Concurrencia (hilos)")
plt.plot(N_fork, T_fork, marker='d', color='red', label="Procesos (fork)")

plt.xlabel("Tamaño de matriz (N)")
plt.ylabel("Tiempo de ejecución (segundos)")
plt.title("Comparación de Multiplicación de Matrices")

plt.legend()
plt.grid(True)

plt.show()
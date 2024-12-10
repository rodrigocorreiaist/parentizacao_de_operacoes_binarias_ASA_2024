import time
import subprocess
import random
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.ticker import ScalarFormatter

# Caminho para o executável a ser testado
executavel = "./a.out"

# Função para gerar o ficheiro de input
def gerar_input(n, m, nome_ficheiro="test.in"):
    with open(nome_ficheiro, "w") as ficheiro:
        ficheiro.write(f"{n} {m}\n")
        
        # Geração da matriz n x n
        for _ in range(n):
            linha = " ".join(str(random.randint(1, n)) for _ in range(n))
            ficheiro.write(linha + "\n")
        
        # Geração de m inteiros (expressao)
        ficheiro.write(" ".join(str(random.randint(1, n)) for _ in range(m)) + "\n")
        
        # Geração do resultado desejado
        ficheiro.write(str(random.randint(1, n)) + "\n")

# Função para medir o tempo de execução do programa com um dado input
def testar_velocidade(n, m):
    gerar_input(n, m)
    
    # Mede o tempo antes da execução
    inicio = time.time()
    
    # Executa o programa com o input do ficheiro
    with open("test.in", "r") as entrada:
        subprocess.run([executavel], stdin=entrada, stdout=subprocess.DEVNULL)
    
    # Mede o tempo após a execução
    fim = time.time()
    
    # Calcula o tempo total
    tempo_execucao = fim - inicio
    
    # Retorna o tempo
    return tempo_execucao

# Listas para guardar as entradas e os tempos de execução
entradas = []
tempos = []

# Geração de 20 combinações de n e m com incremento linear
n_values = np.arange(4, 84, 4)  # 20 valores de n começando em 4, incrementando de 4
m_values = np.arange(10, 1010, 50)  # 20 valores de m começando em 10, incrementando de 50

# Testando as combinações
for n, m in zip(n_values, m_values):
    tempo = testar_velocidade(n, m)
    entradas.append((n, m))
    tempos.append(tempo)
    print(f"n={n}, m={m} -> Tempo de execução: {tempo:.4f} segundos")

# Exibindo os resultados
for (n, m), tempo in zip(entradas, tempos):
    print(f"n={n}, m={m} -> Tempo de execução: {tempo:.4f} segundos")

# Visualização dos dados
f_values = [n**2 * m**3 for n, m in entradas]

# Gráfico de dispersão
plt.scatter(f_values, tempos, color="blue", alpha=0.7)

# Plotando a curva normal (linha conectando os pontos)
sorted_indices = np.argsort(f_values)
sorted_f_values = np.array(f_values)[sorted_indices]
sorted_tempos = np.array(tempos)[sorted_indices]
plt.plot(sorted_f_values, sorted_tempos, '-', color="red")

# Configurando o eixo x para notação científica
plt.gca().xaxis.set_major_formatter(ScalarFormatter(useMathText=True))
plt.gca().ticklabel_format(style='sci', axis='x', scilimits=(0,0))

# Ajustando a escala para dar um pequeno zoom
plt.xlim(min(f_values) * 0.9, max(f_values) * 1.1)
plt.ylim(min(tempos) * 0.9, max(tempos) * 1.1)

plt.xlabel("f(n, m) = m³*n²")
plt.ylabel("Tempo de execução (segundos)")
plt.title("Análise de Tempo de Execução")
plt.show()
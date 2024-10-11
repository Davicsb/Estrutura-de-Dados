import matplotlib.pyplot as plt

# Inicializa o gráfico e suas configurações
plt.ion()  # Ativa o modo interativo para atualizações em tempo real
fig, ax = plt.subplots()

# Inicializa listas vazias para os dados
tam_list, sh_list, ch_list = [], [], []

# Cria as linhas (sem dados) que serão atualizadas
line_sh, = ax.plot([], [], label='Fila de prioridade sem heaps', color='blue', marker='o', linestyle='None')
line_ch, = ax.plot([], [], label='Fila de prioridade com heaps', color='red', marker='o', linestyle='None')

# Configurações do gráfico
ax.set_ylabel('Número de comparações', fontsize=12)
ax.set_xlabel('Tamanho da fila', fontsize=12)
ax.set_title('COMPARAÇÃO: Fila de prioridade com heaps x Fila de prioridade sem heaps', fontsize=20)
ax.legend()

# Limita o eixo y e x para garantir que o gráfico seja sempre visível
ax.set_xlim(0, 100)  # Valor inicial, ajuste de acordo com o tamanho da fila
ax.set_ylim(0, 1000)  # Valor inicial, ajuste de acordo com o número de comparações

# 'with' abre e fecha o arquivo automaticamente após a leitura
with open('saida_fila.txt', 'r') as file:
    lines = file.readlines()

# Percorre o arquivo e atualiza os dados em tempo real
for line in lines[1:]:  # [1:] ignora o cabeçalho
    sh_data, ch_data, tam_data = map(int, line.split())  # Lê os 3 valores e transforma em inteiros

    # Atualiza as listas com novos dados
    tam_list.append(tam_data)
    sh_list.append(sh_data)
    ch_list.append(ch_data)

    # Atualiza os dados do gráfico
    line_sh.set_data(tam_list, sh_list)  # Atualiza a linha para sem heaps
    line_ch.set_data(tam_list, ch_list)  # Atualiza a linha para com heaps

    # Atualiza os limites dos eixos dinamicamente
    ax.set_xlim(0, max(tam_list) + 10)
    ax.set_ylim(0, max(max(sh_list), max(ch_list)) + 100)

    plt.draw()  # Atualiza o gráfico
    plt.pause(0.01)  # Pausa para permitir que o gráfico seja renderizado

plt.ioff()  # Desativa o modo interativo após o loop
plt.show()  # Exibe o gráfico final

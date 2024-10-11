import matplotlib.pyplot as plt #Importa a função para plotar o gráfico

# 'with' abre e fecha o arquivo automaticamente após a leitura
with open('saida_fila.txt', 'r') as file:
    # Lê as linhas do arquivo e armazena em uma lista
    lines = file.readlines()

# Dicionário com as chaves para armazenar dados de filas sem heaps (sh), com heaps (ch), e o tamanho da fila (tam)
data = {'sh': [], 'ch': [], 'tam': []}

#For do começo para o final do arquivo
for line in lines[1:]: #[1:] Porque lê a partir da primeira linha (cabeçalho)
    sh_data, ch_data, tam_data = map(int, line.split())  # Lê cada um dos 3 valores da linha e transforma em inteiro
    data['sh'].append(sh_data)  # Adiciona o primeiro valor à lista 'sh' no dicionário - SEM HEAPS
    data['ch'].append(ch_data)  # Adiciona o segundo valor à lista 'ch' no dicionário - COM HEAPS
    data['tam'].append(tam_data)  # Adiciona o terceiro valor à lista 'tam' no dicionário - TAMANHO DA FILA

# Cria o gráfico de dispersão para fila de prioridade sem heaps, eixo x como tamanho, eixo y o valor do sem heaps
plt.scatter(data['tam'], data['sh'], label='Fila de prioridade sem heaps', color = 'blue')

# Cria o gráfico de dispersão para fila de prioridade com heaps eixo x como tamanho, eixo y o valor do com heaps
plt.scatter(data['tam'], data['ch'], label='Fila de prioridade com heaps', color = 'red')

# Adiciona legendas e título
plt.ylabel('Número de comparações', fontsize=12) # Legenda para o eixo y
plt.xlabel('Tamanho da fila', fontsize=12) # Legenda para o eixo x
plt.title('COMPARAÇÃO: Fila de prioridade com heaps x Fila de prioridade sem heaps', fontsize=20) # Título do gráfico
plt.legend()  # Exibe a legenda
plt.show()  # Exibe o gráfico

# 5: Cria um gráfico de dispersão (scatter) para fila sem heaps, com eixo x como tamanho e eixo y como comparações.
# 6: Cria um gráfico de dispersão para fila com heaps.
# 7: plt.xlabel() adiciona legenda ao eixo x.
# 8: plt.ylabel() adiciona legenda ao eixo y.

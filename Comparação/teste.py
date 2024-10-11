import matplotlib.pyplot as plt

#LEITURA DO ARQUIVO
#with abre e fecha o arquivo após a leitura
with open('saida_fila.txt', 'r') as file:
    #lê as linhas do arquivo e adiciona em uma lista 
    lines = file.readlines()

#dicionario com as chaves lista, arvore e o tamanho, que sao listas
data = {'sh': [], 'tam': []}

#percorre as linhas do arquivo, menos a primeira, pois é so um 'cabeçalho'
#lines[1:] começa a leitura a partir do indíce 1, ou seja, segunda linha 
for line in lines[1:]:
    sh_data, tam_data = map(int, line.split()) #1
    data['sh'].append(sh_data) #3
    data['tam'].append(tam_data) #4


plt.scatter(data['tam'], data['sh'], label='Fila de prioridade sem heap', linewidth=1) #5
plt.xlabel('TAMANHO DA FILA', fontsize=12) #7 - bota legenda no eixo X
plt.ylabel('NUMERO DE COMPARAÇÕES', fontsize=12) #8 - bota legenda no eixo X
plt.title('Comparação Fila de Prioridade com e sem heaps', fontsize=20) #titulo
plt.legend() #adiciona as legendas
plt.grid(True) #adiciona um grade
plt.show() #exibe o grafico
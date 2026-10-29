# Metodologia experimental

## Pergunta de pesquisa

Como Bubble Sort, Selection Sort e Insertion Sort diferem em comparações entre chaves, escritas de elementos e tempo para ordenar os mesmos dados?

## Variáveis controladas

- Mesmo vetor-base para os três algoritmos em cada repetição e cenário.
- Mesmo tamanho `n`, semente e compilação.
- Cópia do vetor antes da cronometragem.
- Validação fora da região cronometrada.
- Mesma definição de comparação e movimentação em todo o código.

## Variáveis observadas

1. `comparisons`: comparações entre valores do vetor.
2. `movements`: escritas de elementos em posições do vetor; troca conta como três.
3. `seconds`: tempo de CPU reportado por `clock()`.
4. `sorted` e `permutation_preserved`: verificações de correção.

## Cenários

- `random`: inteiros pseudoaleatórios.
- `sorted`: vetor crescente.
- `reversed`: vetor decrescente.
- `duplicates`: valores de um domínio pequeno, útil para testar duplicatas.

## Repetições e ordem

O programa executa várias repetições e rotaciona a ordem dos algoritmos. Isso reduz parcialmente o viés de ordem, mas não elimina efeitos de cache, escalonamento ou processos em segundo plano. A mediana e a dispersão são preferíveis a uma única medição.

## Limitações

A instrumentação adiciona incrementos aos laços e pode alterar o tempo medido. Por isso, comparações e movimentações são métricas estruturais, enquanto o tempo é uma medição empírica separada e deve ser interpretado com cautela. `clock()` não é um relógio monotônico universal e sua resolução depende da plataforma.

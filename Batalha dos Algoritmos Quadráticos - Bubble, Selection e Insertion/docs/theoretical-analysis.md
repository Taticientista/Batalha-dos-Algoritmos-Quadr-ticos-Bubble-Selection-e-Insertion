# Análise teórica

## Modelo RAM

No modelo RAM, operações elementares sobre valores e índices são consideradas de custo constante. Para comparação prática, o número de comparações entre chaves e o número de escritas ajudam a explicar parte do trabalho, mas não representam todos os custos de hardware.

## Bubble Sort otimizado

No pior caso, executa um número quadrático de comparações e pode executar um número quadrático de trocas. Cada troca é contabilizada como três escritas de elementos. A saída antecipada melhora o melhor caso para `O(n)` quando o vetor já está ordenado.

## Selection Sort

Executa `n(n-1)/2` comparações entre chaves independentemente da ordem dos dados, pois procura o mínimo em cada sufixo. Realiza no máximo `n-1` trocas, ignorando trocas quando o mínimo já está na posição correta. É atraente quando escritas são caras, mas continua quadrático em comparações.

## Insertion Sort

No melhor caso, com vetor ordenado, executa `n-1` comparações aproximadamente e uma escrita da chave por iteração; seu tempo é `O(n)`. No caso médio e no pior caso, desloca elementos repetidamente e tem tempo `O(n²)`. Ainda assim, pode vencer em implementações práticas por ter um laço interno simples, acesso sequencial a posições contíguas e menos sobrecarga que trocas adjacentes repetidas.

## Por que Insertion pode ser mais rápido?

A classe `O(n²)` descreve a ordem de crescimento, não a constante multiplicativa. Dois algoritmos quadráticos podem executar números diferentes de operações, e operações semelhantes também podem ter custos diferentes na CPU. Insertion Sort desloca valores e grava a chave; Bubble Sort frequentemente faz várias trocas, cada uma com três escritas. Selection Sort reduz escritas, mas sempre faz aproximadamente `n²/2` comparações.

Não é correto afirmar que Insertion Sort sempre vence: a implementação, a entrada, a compilação, a CPU e a métrica podem alterar o resultado. O experimento deve sustentar a conclusão, não pressupô-la.

## Ordem de crescimento para n = 5000

`n² = 25.000.000`, enquanto `n(n-1)/2 = 12.497.500`. Esses valores ilustram a escala dos termos quadráticos, mas não são resultados de tempo e não substituem as contagens medidas pelo programa.

## Conclusão experimental incluída

Na execução de referência registrada em `results/benchmark.csv` (GCC 14.2.0, `-O2`, Linux x86_64, `n = 5000`, sete repetições), Insertion Sort teve a menor mediana no cenário aleatório (0,003390 s), seguido por Selection Sort (0,030567 s) e Bubble Sort (0,061135 s). Bubble Sort registrou 18.933.033 movimentações médias, contra 14.985 do Selection Sort; Insertion Sort registrou 6.316.010.

No cenário reverso, Bubble Sort chegou a 37.492.500 movimentações médias, enquanto Selection Sort fez 7.500. No vetor ordenado, Bubble e Insertion aproveitaram o melhor caso linear, mas Selection continuou com 12.497.500 comparações.

Esses dados sustentam a importância das constantes e das escritas, mas não provam que Insertion Sort sempre será o mais rápido. Os tempos dependem da máquina, do compilador, da instrumentação e da resolução do relógio; medições muito pequenas devem ser interpretadas com cautela.

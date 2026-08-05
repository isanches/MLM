# Especificação Técnica

## MLM – Mini Language Model (Bigram Edition)

**Versão:** 1.0

---

# 1. Objetivo

Este documento define a implementação de referência do projeto **MLM – Mini Language Model (Bigram Edition)**.

A implementação deverá privilegiar:

* simplicidade;
* legibilidade;
* boas práticas em C99;
* facilidade de compreensão pelos alunos.

O desempenho computacional não é prioridade.

---

# 2. Arquitetura geral

O fluxo completo do programa será:

```
                +----------------+
                |  Arquivo texto |
                +-------+--------+
                        |
                        v
                 Leitura do corpus
                        |
                        v
                   Tokenização
                        |
                        v
              Construção do vocabulário
                (word <-> integer ID)
                        |
                        v
            Sequência de identificadores
                        |
                        v
              Contagem dos bigramas
                        |
                        v
           Modelo estatístico treinado
                        |
                        v
               Geração de novo texto
```

Todo o processamento ocorrerá em memória.

---

# 3. Estruturas de dados

A implementação utilizará apenas vetores e estruturas (`struct`).

Não serão utilizadas:

* listas ligadas;
* árvores;
* tabelas hash;
* bibliotecas externas.

Essa escolha mantém o projeto acessível aos alunos.

---

## 3.1 Vocabulário

Representa todas as palavras distintas.

```c
typedef struct
{
    char word[MAX_WORD_LENGTH];
} VocabularyEntry;
```

O vocabulário será armazenado na estrutura:

```c
typedef struct {
    VocabularyEntry entries[MAX_VOCAB_SIZE];
    int size;
} Vocabulary;
```

A posição no vetor `entries` corresponde diretamente ao ID da palavra.

Exemplo:

```
ID    Palavra

0     the
1     cat
2     sat
3     on
4     mat
```

---

## 3.2 Sequência de tokens

Após a tokenização:

```
the cat sat on the mat
```

será obtido:

```
0 1 2 3 0 4
```

Essa sequência será armazenada em `ids`:

```c
typedef struct {
    int ids[MAX_TOKENS];
    int size;
} TokenSequence;
```

---

## 3.3 Modelo de bigramas

Cada bigrama será armazenado como:

```c
typedef struct
{
    int current;
    int next;
    int count;
} Bigram;
```

Todos os bigramas observados serão armazenados no vetor da estrutura:

```c
typedef struct {
    Bigram entries[MAX_BIGRAMS];
    int size;
} BigramModel;
```

Não será utilizada matriz de adjacência.

A busca será linear.

Essa decisão simplifica bastante o código.

---

# 4. Limites do projeto

Para evitar alocação dinâmica, a implementação utilizará limites fixos.

Valores sugeridos:

```c
MAX_WORD_LENGTH = 32

MAX_VOCAB_SIZE = 5000

MAX_TOKENS = 50000

MAX_BIGRAMS = 50000

MAX_GENERATED_WORDS = 100
```

Esses limites são suficientes para os corpora utilizados na disciplina.

---

# 5. Tokenização

Separadores aceitos:

* espaço
* tabulação
* quebra de linha

Opcionalmente:

```
.,;:!?()"'
```

podem ser removidos.

Todas as letras deverão ser convertidas para minúsculas.

Assim,

```
Dog
DOG
dog
```

serão tratados como a mesma palavra.

---

# 6. Construção do vocabulário

Para cada token:

1. procurar a palavra no vocabulário;
2. se existir, retornar o ID;
3. caso contrário:

   * inserir nova palavra;
   * atribuir novo ID.

Busca:

```
O(V)
```

onde

```
V = tamanho do vocabulário
```

Essa busca linear é intencionalmente simples.

---

# 7. Treinamento

Após gerar

```
tokens[]
```

percorrer

```
tokens[i]
tokens[i+1]
```

para todo

```
0 <= i < N-1
```

Para cada par:

```
(current,next)
```

* procurar o bigrama;
* se existir:

```
count++
```

caso contrário:

```
criar novo bigrama
```

---

# 8. Geração de texto

Algoritmo:

Escolher uma palavra inicial.

Enquanto:

```
n_palavras < MAX_GENERATED_WORDS
```

executar:

1. localizar todos os bigramas cujo

```
current == palavra_atual
```

2. calcular a soma das frequências

```
total
```

3. gerar

```
r = rand() % total
```

4. selecionar o próximo token proporcionalmente às frequências

5. imprimir palavra

6. repetir.

---

# 9. Funções previstas

A implementação deverá permanecer modular.

## Entrada

```c
bool load_corpus(...)
```

Lê o arquivo texto.

---

## Tokenização

```c
void tokenize(...)
```

Extrai palavras.

---

## Vocabulário

```c
int vocabulary_find(...)
```

Procura palavra.

```c
int vocabulary_add(...)
```

Insere nova palavra.

```c
int vocabulary_get_id(...)
```

Retorna ID.

---

## Bigramas

```c
void train_bigram(...)
```

Treina modelo.

```c
int find_bigram(...)
```

Busca bigrama.

---

## Geração

```c
int choose_next(...)
```

Escolhe próxima palavra.

```c
void generate_text(...)
```

Produz sequência.

---

# 10. Complexidade esperada

## Construção do vocabulário

```
O(N·V)
```

onde

```
N = número de tokens
V = tamanho do vocabulário
```

---

## Treinamento

```
O(N·B)
```

onde

```
B = número de bigramas
```

---

## Geração

Cada palavra gerada exige uma busca linear nos bigramas.

Complexidade aproximada:

```
O(G·B)
```

onde

```
G = número de palavras geradas
```

Como os conjuntos de dados são pequenos, esse custo é aceitável.

---

# 11. Estratégia de implementação

A solução oficial será construída incrementalmente na seguinte ordem:

1. leitura do arquivo;
2. tokenização;
3. vocabulário;
4. sequência de IDs;
5. treinamento;
6. geração.

Cada etapa deverá ser validada antes da implementação seguinte.

---

# 12. Boas práticas exigidas

A solução oficial deverá demonstrar:

* nomes de variáveis autoexplicativos;
* comentários apenas quando agregarem informação;
* funções curtas;
* ausência de variáveis globais desnecessárias;
* verificação de erros de abertura de arquivo;
* tratamento de limites dos vetores;
* compilação sem *warnings* com:

```bash
gcc -std=c99 -Wall -Wextra -pedantic
```

---

# 13. Possíveis extensões futuras

A arquitetura foi planejada para permitir evolução gradual, incluindo:

* trigramas;
* n-gramas genéricos;
* tabela hash para o vocabulário;
* tabela hash para bigramas;
* alocação dinâmica de memória;
* suavização (*smoothing*);
* serialização do modelo;
* interface interativa;
* token especial `<START>` e `<END>`;
* integração futura com modelos neurais.

Essas funcionalidades **não** fazem parte da implementação de referência, mas a organização do código deverá facilitar sua incorporação em versões posteriores.

# Projeto da Disciplina

# MLM – Mini Language Model (Bigram Edition)

## 1. Objetivo

Os Grandes Modelos de Linguagem (Large Language Models – LLMs) revolucionaram diversas áreas da Inteligência Artificial. Embora os modelos atuais utilizem arquiteturas neurais complexas, como Transformers, os primeiros modelos estatísticos de linguagem já eram capazes de capturar relações entre palavras e gerar texto de maneira probabilística.

O objetivo deste projeto é implementar, em linguagem **C99**, um **Mini Language Model (MLM)** baseado em **bigramas**, permitindo compreender os princípios fundamentais de representação de texto, construção de vocabulário, modelagem estatística da linguagem e geração automática de texto.

Ao final deste projeto, o aluno terá desenvolvido um sistema capaz de:

* ler um corpus textual;
* converter palavras em identificadores inteiros;
* aprender probabilidades de transição entre palavras;
* gerar automaticamente novas sequências de texto.

Este projeto tem caráter didático e busca apresentar, em pequena escala, alguns dos conceitos utilizados em modelos modernos de linguagem.

---

# 2. Objetivos de aprendizagem

Ao concluir este projeto, o aluno deverá ser capaz de:

* manipular arquivos texto em C;
* implementar estruturas de dados utilizando vetores e estruturas (`struct`);
* realizar tokenização de texto;
* construir um vocabulário baseado em identificadores inteiros;
* implementar um modelo estatístico baseado em bigramas;
* utilizar números aleatórios para amostragem probabilística;
* desenvolver software organizado e documentado.

---

# 3. Descrição do problema

Um modelo de linguagem estima a probabilidade de ocorrência de uma palavra conhecendo as palavras anteriores.

Neste projeto será utilizado o caso mais simples possível:

> A próxima palavra depende apenas da palavra imediatamente anterior.

Ou seja,

P(próxima palavra | palavra atual)

Esse modelo é conhecido como **modelo de bigramas**.

Durante o treinamento, o programa deverá observar todas as transições existentes no corpus.

Exemplo:

```
the cat sat on the mat
```

gera as transições

```
the → cat
cat → sat
sat → on
on  → the
the → mat
```

Após analisar todo o corpus, o programa deverá utilizar essas estatísticas para gerar novos textos.

---

# 4. Escopo do projeto

O projeto será composto pelas seguintes etapas.

## Etapa 1 – Leitura do corpus

O programa deverá:

* abrir um arquivo texto;
* ler seu conteúdo;
* armazenar as linhas para processamento.

O corpus será fornecido pelo professor.

---

## Etapa 2 – Tokenização

Cada linha deverá ser dividida em palavras.

Considere como separadores:

* espaço;
* tabulação;
* quebra de linha.

Não será exigido tratamento sofisticado de pontuação.

---

## Etapa 3 – Construção do vocabulário

Cada palavra distinta deverá receber um identificador inteiro.

Exemplo:

| Palavra | ID |
| ------- | -: |
| the     |  0 |
| cat     |  1 |
| sat     |  2 |
| on      |  3 |
| mat     |  4 |

Durante o restante do processamento, o programa deverá trabalhar preferencialmente com esses identificadores.

---

## Etapa 4 – Treinamento do modelo

O programa deverá percorrer toda a sequência de palavras e contabilizar a frequência de ocorrência de cada bigrama.

Exemplo:

```
(cat → sat)
```

pode ocorrer diversas vezes ao longo do corpus.

Essas frequências constituem o modelo de linguagem.

---

## Etapa 5 – Geração de texto

Após o treinamento, o programa deverá gerar automaticamente uma sequência de palavras.

O algoritmo deverá:

1. escolher uma palavra inicial;
2. selecionar probabilisticamente uma palavra seguinte;
3. repetir o processo até atingir o número desejado de palavras.

---

# 5. Estrutura do projeto

A estrutura do repositório será:

```
MLM/
│
├── docs/
├── include/
├── source/
├── tests/
├── Makefile
└── README.md
```

O aluno deverá manter essa organização durante todo o desenvolvimento.

---

# 6. Requisitos obrigatórios

O projeto deverá ser desenvolvido utilizando exclusivamente:

* linguagem C99;
* biblioteca padrão da linguagem C.

Não será permitido utilizar bibliotecas externas para processamento de texto, estruturas de dados ou Inteligência Artificial.

O programa deverá ser compilado utilizando:

```
gcc -std=c99
```

---

# 7. Funcionalidades mínimas

O programa deverá ser capaz de:

* carregar um corpus;
* construir o vocabulário;
* converter palavras em IDs;
* contabilizar os bigramas;
* gerar texto automaticamente.

---

# 8. Restrições

Este projeto **não** deverá implementar:

* trigramas;
* modelos de ordem superior;
* redes neurais;
* embeddings;
* Transformers;
* atenção (Attention);
* BPE (Byte Pair Encoding);
* WordPiece;
* SentencePiece;
* algoritmos de treinamento profundo.

O objetivo é compreender os fundamentos estatísticos dos modelos de linguagem.

---

# 9. Entregáveis

Cada grupo deverá entregar:

* código-fonte completo;
* Makefile funcional;
* README contendo instruções de compilação e execução;
* documentação do código;
* relatório técnico (máximo de cinco páginas).

---

# 10. Critérios de avaliação

Serão considerados os seguintes aspectos:

| Critério                          | Peso |
| --------------------------------- | ---: |
| Correção da implementação         |  40% |
| Organização do código             |  20% |
| Clareza e documentação            |  15% |
| Qualidade das estruturas de dados |  15% |
| Boas práticas de programação      |  10% |

---

# 11. Desafios opcionais (não obrigatórios)

Os alunos interessados poderão implementar funcionalidades adicionais, como:

* remoção de pontuação;
* conversão para letras minúsculas;
* escolha ponderada por probabilidades;
* suporte a múltiplos arquivos de treinamento;
* salvamento e carregamento do modelo treinado;
* geração de texto iniciando por uma palavra escolhida pelo usuário.

Essas funcionalidades não substituem os requisitos obrigatórios e poderão ser consideradas como atividades de enriquecimento.

---

# 12. Considerações finais

O foco deste projeto é compreender como um modelo estatístico de linguagem pode ser construído a partir de um corpus textual utilizando apenas conceitos fundamentais de programação e estruturas de dados.

Embora bastante simplificado em relação aos modelos atuais, este projeto reproduz várias etapas presentes em sistemas reais de Processamento de Linguagem Natural (PLN), incluindo a construção de vocabulário, a representação de palavras por identificadores inteiros e a modelagem probabilística das transições entre palavras.

#ifndef MLM_H
#define MLM_H

/*
 * ================================================================
 *  MLM - Mini Language Model (Bigram Edition)
 *
 *  Arquivo: mlm.h
 *
 *  Interface pública do projeto.
 *
 *  Todas as estruturas de dados e protótipos de funções são
 *  definidos neste arquivo.
 *
 *  Linguagem: C99
 * ================================================================
 */

#include <stdbool.h>

#define MLM_VERSION_MAJOR 1
#define MLM_VERSION_MINOR 0
#define MLM_VERSION_PATCH 0

/* ================================================================
 * Constantes do projeto
 * ================================================================ */

/* Número máximo de caracteres de uma palavra (incluindo '\0'). */
#define MAX_WORD_LENGTH      32

/* Número máximo de palavras distintas. */
#define MAX_VOCAB_SIZE       5000

/* Número máximo de tokens do corpus. */
#define MAX_TOKENS           50000

/* Número máximo de bigramas armazenados. */
#define MAX_BIGRAMS          50000

/* Número máximo de palavras geradas. */
#define MAX_GENERATED_WORDS  100

/* Tamanho máximo de uma linha do arquivo texto. */
#define MAX_LINE_LENGTH      1024


/* ================================================================
 * Estruturas de dados
 * ================================================================ */

/*
 * Uma entrada do vocabulário.
 *
 * O índice desta estrutura dentro do vetor corresponde
 * diretamente ao ID da palavra.
 */
typedef struct
{
    char word[MAX_WORD_LENGTH];

} VocabularyEntry;


/*
 * Vocabulário completo.
 */
typedef struct
{
    VocabularyEntry entries[MAX_VOCAB_SIZE];

    int size;

} Vocabulary;


/*
 * Sequência de IDs correspondente ao corpus tokenizado.
 */
typedef struct
{
    int ids[MAX_TOKENS];

    int size;

} TokenSequence;


/*
 * Um único bigrama observado.
 *
 * Exemplo:
 *
 * current = 15
 * next    = 22
 * count   = 31
 *
 * significa que:
 *
 * palavra 15 -> palavra 22
 *
 * ocorreu 31 vezes.
 */
typedef struct
{
    int current;

    int next;

    int count;

} Bigram;


/*
 * Modelo estatístico composto por todos os bigramas.
 */
typedef struct
{
    Bigram entries[MAX_BIGRAMS];

    int size;

} BigramModel;


/*
 * Estrutura principal do Mini Language Model.
 *
 * Reúne todos os componentes do sistema.
 */
typedef struct
{
    Vocabulary vocabulary;

    TokenSequence tokens;

    BigramModel model;

    bool trained;

} MLM;


/* ================================================================
 * Inicialização
 * ================================================================ */

/*
 * Inicializa toda a estrutura MLM.
 */
void mlm_init(MLM *mlm);


/* ================================================================
 * Leitura do corpus e Tokenização
 * ================================================================ */

/*
 * Converte o corpus em sequência de IDs.
 *
 * Este tokenizador foi mantido simples propositalmente.
 *
 * Modelos reais utilizam tokenizadores muito mais sofisticados.
 */
void tokenize(MLM *mlm, const char *filename);


/* ================================================================
 * Vocabulário
 * ================================================================ */

/*
 * Procura uma palavra no vocabulário.
 *
 * Retorna:
 *
 * ID >= 0  -> encontrada
 * -1       -> inexistente
 */
int vocabulary_find(const Vocabulary *vocabulary,
                    const char *word);


/*
 * Adiciona uma nova palavra ao vocabulário.
 *
 * Retorna o ID atribuído.
int vocabulary_add(Vocabulary *vocabulary,
                   const char *word);
 *
 * Note: movida para main.c
 */


/*
 * Obtém o ID de uma palavra.
 *
 * Caso a palavra ainda não exista,
 * ela será inserida automaticamente.
 */
int vocabulary_get_id(Vocabulary *vocabulary,
                      const char *word);


/* ================================================================
 * Treinamento
 * ================================================================ */

/*
 * Treina o modelo de bigramas.
 */
void train_bigram_model(MLM *mlm);


/*
 * Procura um bigrama existente.
 *
 * Retorna:
 *
 * índice >= 0
 * -1 caso inexistente
 */
int bigram_find(const BigramModel *model,
                int current,
                int next);


/* ================================================================
 * Geração
 * ================================================================ */

/*
 * Escolhe probabilisticamente a próxima palavra.
 *
 * Retorna o ID escolhido.
 */
int choose_next(const BigramModel *model,
                int current);


/*
 * Gera texto.
 */
void generate_text(const MLM *mlm,
                   int first_word,
                   int number_of_words);


/* ================================================================
 * Depuração
 * ================================================================ */

/*
 * Imprime o vocabulário.
 */
void print_vocabulary(const MLM *mlm);


/*
 * Imprime a sequência de IDs.
 */
void print_tokens(const MLM *mlm);


/*
 * Imprime todos os bigramas.
 */
void print_bigrams(const MLM *mlm);


#endif

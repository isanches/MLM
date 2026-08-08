/*
 * ================================================================
 *  MLM - Mini Language Model (Bigram Edition)
 *
 *  Arquivo:
 *      main.c
 *
 *  Implementação de referência do professor.
 *
 *  Linguagem:
 *      C99
 * ================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "../include/mlm.h"


/* ================================================================
 * Inicialização
 * ================================================================ */

/*
 * Inicializa todas as estruturas do modelo.
 */
void mlm_init(MLM *mlm)
{
    mlm->vocabulary.size = 0;
    mlm->tokens.size = 0;
    mlm->model.size = 0;
    mlm->trained = false;
}


/* ================================================================
 * Vocabulário
 * ================================================================ */

/*
 * Procura uma palavra no vocabulário.
 *
 * Retorna:
 *      ID da palavra
 *      -1 caso não exista
 */
int vocabulary_find(const Vocabulary *vocabulary,
                    const char *word)
{
    int i;

    for (i = 0; i < vocabulary->size; i++)
    {
        if (strcmp(vocabulary->entries[i].word, word) == 0)
        {
            return i;
        }
    }

    return -1;
}


/*
 * Insere uma nova palavra.
 *
 * Retorna o ID criado.
 */
int vocabulary_add(Vocabulary *vocabulary,
                   const char *word)
{
    int id;

    if (vocabulary->size >= MAX_VOCAB_SIZE)
    {
        fprintf(stderr,
                "Erro: vocabulário cheio.\n");

        exit(EXIT_FAILURE);
    }

    id = vocabulary->size;

    strncpy(vocabulary->entries[id].word,
            word,
            MAX_WORD_LENGTH - 1);

    vocabulary->entries[id].word[MAX_WORD_LENGTH - 1] = '\0';

    vocabulary->size++;

    return id;
}


/*
 * Obtém o ID de uma palavra.
 *
 * Caso ainda não exista,
 * ela será automaticamente adicionada.
 */
int vocabulary_get_id(Vocabulary *vocabulary,
                      const char *word)
{
    int id;

    id = vocabulary_find(vocabulary, word);

    if (id >= 0)
    {
        return id;
    }

    return vocabulary_add(vocabulary, word);
}


/* ================================================================
 * Impressão (depuração)
 * ================================================================ */

/*
 * Imprime todo o vocabulário.
 */
void print_vocabulary(const MLM *mlm)
{
    int i;

    printf("\n");
    printf("========== VOCABULARY ==========\n");

    printf("Size: %d\n\n",
           mlm->vocabulary.size);

    for (i = 0; i < mlm->vocabulary.size; i++)
    {
        printf("%4d : %s\n",
               i,
               mlm->vocabulary.entries[i].word);
    }

    printf("\n");
}

/* ================================================================
 * Leitura do corpus
 * ================================================================ */

/*
 * Abre o arquivo apenas para verificar sua existência.
 *
 * O arquivo será posteriormente reaberto durante a tokenização.
 */
bool load_corpus(MLM *mlm, const char *filename)
{
    FILE *fp;

    (void) mlm;

    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        return false;
    }

    fclose(fp);

    return true;
}

/* ================================================================
 * Funções auxiliares (invisíveis foram de main.c)
 * ================================================================ */

/*
 * Converte uma palavra para letras minúsculas.
 */
static void normalize_word(char *word)
{
    while (*word)
    {
        *word = (char)tolower((unsigned char)*word);
        word++;
    }
}


/*
 * Caracteres utilizados como separadores.
 */
static int is_delimiter(char c)
{
    switch (c)
    {
        case ' ':
        case '\t':
        case '\n':  // fim de linha Unix, Linux, Mac (LF) \n
        case '\r':  // fim de linha Windows (CRLF) \r\n
        case '.':
        case ',':
        case ';':
        case ':':
        case '!':
        case '?':
        case '(':
        case ')':
        case '"':
        case '*':
        case '+':
        case '_':
        case '-':
        case '[':
        case ']':
        case '{':
        case '}':
        case '&':
        case '$':
        case '#':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        case '\'':
            return 1;

        default:
            return 0;
    }
}

/* ================================================================
 * Tokenização
 * ================================================================ */

static void process_word(MLM *mlm,
                         char *word,
                         int index)
{
    word[index] = '\0';

    normalize_word(word);

    if (mlm->tokens.size >= MAX_TOKENS)
    {
        fprintf(stderr,
                "Número máximo de tokens excedido.\n");

        exit(EXIT_FAILURE);
    }

    mlm->tokens.ids[mlm->tokens.size++] =
        vocabulary_get_id(&mlm->vocabulary,
                          word);
}

/*
 * Lê o corpus e gera:
 *
 * - vocabulário
 * - sequência de IDs
 *
 */
void tokenize(MLM *mlm,
              const char *filename)
{
    FILE *fp;

    char line[MAX_LINE_LENGTH];

    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        fprintf(stderr,
                "Erro ao abrir %s\n",
                filename);

        exit(EXIT_FAILURE);
    }

    while (fgets(line,
                 sizeof(line),
                 fp) != NULL)
    {
        char word[MAX_WORD_LENGTH];

        int index = 0;

        int i = 0;

        while (1)
        {
            char c = line[i];

            if (c == '\0')
            {
                if (index > 0)
                {
                    process_word(mlm, word, index);
                }

                break;
            }

            if (is_delimiter(c))
            {
                if (index > 0)
                {
                    process_word(mlm, word, index);
                    index = 0;
                }
            }
            else
            {
                if (index < MAX_WORD_LENGTH - 1)
                {
                    word[index++] = c;
                }
            }

            i++;
        }
    }

    fclose(fp);
}

/*
 * Imprime a sequência de tokens.
 */
void print_tokens(const MLM *mlm)
{
    int i, id;

    printf("\n");
    printf("========== TOKENS ==========\n");

    printf("Total: %d\n\n",
           mlm->tokens.size);

    for (i = 0; i < mlm->tokens.size; i++)
    {
        id = mlm->tokens.ids[i];
        printf("%4d -> %s\n",
               id,
               mlm->vocabulary.entries[id].word);

    }

    printf("\n\n");
}

/* ================================================================
 * Bigramas
 * ================================================================ */

/*
 * Adiciona um novo bigrama ao modelo.
 *
 * Retorna o índice criado.
 */
static int bigram_add(BigramModel *model,
                      int current,
                      int next)
{
    int index;

    if (model->size >= MAX_BIGRAMS)
    {
        fprintf(stderr,
                "Erro: limite de bigramas excedido.\n");

        exit(EXIT_FAILURE);
    }

    index = model->size;

    model->entries[index].current = current;
    model->entries[index].next    = next;
    model->entries[index].count   = 1;

    model->size++;

    return index;
}

/*
 * Procura um bigrama.
 *
 * Retorna:
 *
 * índice >= 0
 * -1 caso inexistente
 */
int bigram_find(const BigramModel *model,
                int current,
                int next)
{
    int i;

    for (i = 0; i < model->size; i++)
    {
        if (model->entries[i].current == current &&
            model->entries[i].next    == next)
        {
            return i;
        }
    }

    return -1;
}

/*
 * Constrói o modelo estatístico de bigramas.
 */
void train_bigram_model(MLM *mlm)
{
    int i;

    for (i = 0; i < mlm->tokens.size - 1; i++)
    {
        int current;
        int next;

        int index;

        current = mlm->tokens.ids[i];
        next    = mlm->tokens.ids[i + 1];

        index = bigram_find(&mlm->model,
                            current,
                            next);

        if (index >= 0)
        {
            mlm->model.entries[index].count++;
        }
        else
        {
            bigram_add(&mlm->model,
                       current,
                       next);
        }
    }

    mlm->trained = true;
}

/*
 * Imprime todos os bigramas.
 */
void print_bigrams(const MLM *mlm)
{
    int i;

    printf("\n");
    printf("========== BIGRAMS ==========\n");

    printf("Total: %d\n\n",
           mlm->model.size);

    printf("%-20s %-20s %s\n",
           "Current",
           "Next",
           "Count");

    printf("----------------------------------------------------------\n");

    for (i = 0; i < mlm->model.size; i++)
    {
        const Bigram *b = &mlm->model.entries[i];

        printf("%-20s %-20s %5d\n",
               mlm->vocabulary.entries[b->current].word,
               mlm->vocabulary.entries[b->next].word,
               b->count);
    }

    printf("\n");
}

/*
 * Escolhe a próxima palavra utilizando
 * amostragem proporcional às frequências.
 */
int choose_next(const BigramModel *model,
                int current)
{
    int i;
    int total = 0;
    int accumulated = 0;
    int r;

    /* Soma todas as frequências possíveis */

    for (i = 0; i < model->size; i++)
    {
        if (model->entries[i].current == current)
        {
            total += model->entries[i].count;
        }
    }

    /*
     * Palavra sem sucessores.
     */

    if (total == 0)
    {
        return -1;
    }

    r = rand() % total;

    for (i = 0; i < model->size; i++)
    {
        if (model->entries[i].current != current)
        {
            continue;
        }

        accumulated += model->entries[i].count;

        if (r < accumulated)
        {
            return model->entries[i].next;
        }
    }

    return -1;
}

/*
 * Gera uma sequência de palavras.
 */
void generate_text(const MLM *mlm,
              int first_word,
              int number_of_words)
{
    int current;
    int next;
    int i;

    if (!mlm->trained)
    {
        fprintf(stderr,
                "Erro: o modelo ainda não foi treinado.\n");
        return;
    }

    if (mlm->vocabulary.size == 0)
    {
        return;
    }

    if (first_word >= 0 &&
        first_word < mlm->vocabulary.size)
    {
        current = first_word;
    }
    else
    {
        current = rand() % mlm->vocabulary.size;
    }

    printf("\n========== GENERATED TEXT ==========\n\n");

    printf("%s ",
           mlm->vocabulary.entries[current].word);

    for (i = 1; i < number_of_words; i++)
    {
        next = choose_next(&mlm->model,
                           current);

        if (next < 0)
        {
            break;
        }

        printf("%s ",
               mlm->vocabulary.entries[next].word);

        current = next;
    }

    printf("\n\n");
}

/*
 * Exibe um resumo do modelo treinado.
 */
void print_model_summary(const MLM *mlm)
{
    int i;
    int max_count = 0;
    int max_index = -1;

    for (i = 0; i < mlm->model.size; i++)
    {
        if (mlm->model.entries[i].count > max_count)
        {
            max_count = mlm->model.entries[i].count;
            max_index = i;
        }
    }

    printf("\n========== MODEL SUMMARY ==========\n\n");

    printf("Vocabulary size : %d\n",
           mlm->vocabulary.size);

    printf("Tokens          : %d\n",
           mlm->tokens.size);

    printf("Bigrams         : %d\n",
           mlm->model.size);

    if (max_index >= 0)
    {
        const Bigram *b = &mlm->model.entries[max_index];

        printf("Most frequent   : %s -> %s (%d)\n",
               mlm->vocabulary.entries[b->current].word,
               mlm->vocabulary.entries[b->next].word,
               b->count);
    }

    printf("\n");
}

int main(void)
{
    MLM mlm;
    char corpus[MAX_LINE_LENGTH];

    printf("\nMLM, version %d.%d.%d\n\n",
           MLM_VERSION_MAJOR,
           MLM_VERSION_MINOR,
           MLM_VERSION_PATCH);

    /* Inicialização */

    srand((unsigned) time(NULL));

    mlm_init(&mlm);

    /* Construção do modelo */

    printf("Corpus file name: ");
    gets(corpus);
    tokenize(&mlm, corpus);

    train_bigram_model(&mlm);

    /* Depuração */

    //print_model_summary(&mlm);

    //print_vocabulary(&mlm);

    //print_bigrams(&mlm);

    /* Geração */

    for(int i = 1; i <= 10; i++)
    {
        printf("%2d:", i);
        generate_text(&mlm, -1, 30);
    }

    return EXIT_SUCCESS;
}

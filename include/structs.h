#ifndef STRUCTS_H
#define STRUCTS_H

#define TAM_BIBLIOTECA 100
#define TAM_TEXTO 100

typedef struct
{
    int dia;
    int mes;
    int ano;
} Data;

typedef struct
{
    char nome[TAM_TEXTO];
    Data data_nascimento;
} Autor;

typedef struct
{
    int id;
    char titulo[TAM_TEXTO];
    Autor autor;
    Data data_escrita;
} LivroMagico;

#endif

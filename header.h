#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100
#define NUM_LOCAIS 20
#define NUM_LOCAIS_PESSOA 3

/*-----------------Structs-----------------------*/
typedef struct{
  int dia,mes,ano;
}Data;

typedef struct{
  int hora, minuto;
}Hora;

/*Lista dos Pontos de Interesse*/
typedef struct lnodepinteresse* lista_p_interesse;
typedef struct lnodepinteresse{
  int id;
  int popularidade;
  char nome[MAX];
  char descricao[MAX];
  Hora entrada, saida;
  lista_p_interesse next;
}list_p_interesse;

/*Lista dos Locais*/
typedef struct lnodelocais* lista_locais;
typedef struct lnodelocais{
  int id;
  int popularidade;
  char cidade[MAX];
  lista_p_interesse p_interesse;
  char pais[MAX];
  lista_locais next;
}list_locais;

/*Lista das pessoas*/
typedef struct lnodepessoas* lista_pessoas;
typedef struct lnodepessoas{
  char hot[MAX];
  int hasHot;
  int id;
  char nome[MAX];
  char morada[MAX];
  Data data_nascimento;
  int telefone;
  lista_locais locais;
  lista_p_interesse p_interesse;
  lista_pessoas next;
}list_pessoas;
/*----------------------------------------------------*/
/*-------------------------------Funcoes------------------------------------------*/
lista_p_interesse insere_ordenado_lista_pi(int id, lista_p_interesse lista, char descricao[], char nome[], Hora entrada, Hora saida, int pop);
lista_p_interesse insere_fim_lista_pi(int id, lista_p_interesse lista, char descricao[], char nome[], Hora entrada, Hora saida, int pop);
lista_locais insere_fim_lista_local(lista_locais lista, int id, char cidade[], char pais[], lista_p_interesse listaPI, int pop);
lista_pessoas insere_fim_lista_pessoas(lista_pessoas lista, char nome[], char morada[], Data data, int telefone, int id);
lista_locais ler_fich_locais(lista_locais lista);
void imprime_lista_locais(lista_locais lista);
void imprime_lista_pi(lista_p_interesse lista);
void menu(lista_locais, lista_pessoas);
lista_pessoas registar_user();
int verificarData(int dia, int mes, int ano);
void imprime_lista_pessoas(lista_pessoas lista);
lista_pessoas ler_fich_pessoas(lista_pessoas lista, lista_locais lista_locais);
void guarda_fich_pessoas(lista_pessoas lista);
lista_pessoas altera_nome(lista_pessoas lista, char nome[], int id);
lista_pessoas alterar_dados(lista_pessoas lista, int id);
lista_pessoas altera_morada(lista_pessoas lista, char morada[], int id);
lista_pessoas altera_telefone(lista_pessoas lista, int telefone, int id);
int procura_na_lista(lista_pessoas lista, int id);
int tamanho_lista(lista_pessoas lista);
lista_p_interesse apaga_lista(lista_p_interesse lista);
lista_pessoas escolher_preferencia(lista_pessoas lista_pessoas, lista_locais lista_locais, int id);
lista_locais aumenta_pop(lista_locais lista, int id);
lista_pessoas escolher_preferencia_local(lista_pessoas lista_pessoas, int id, lista_locais lista_locais);
void imprime_locais_pessoa(lista_locais lista);
int procura_na_lista_locais(lista_locais lista, int id);
lista_pessoas escolher_preferencia_ponto_interesse(lista_pessoas lista_pessoas, int id, lista_locais lista_locais);
lista_locais escolher_hot(lista_pessoas lista, int id, lista_locais lista_locais);
lista_p_interesse insere_inicio_lista_pi(int id, lista_p_interesse lista, char descricao[], char nome[], Hora entrada, Hora saida, int pop);
void imprime_pi_pessoa(lista_p_interesse lista);
lista_locais aumenta_pop_pi(lista_locais lista, int id_pi);
int procura_lista(lista_pessoas lista, int id_local, int id_pessoa);
lista_locais insere_ordenado_lista(lista_locais lista, int id, char cidade[], char pais[], lista_p_interesse listaPI, int pop);
lista_locais insere_inicio_lista(lista_locais lista, int id, char cidade[], char pais[], lista_p_interesse listaPI, int pop);
lista_locais ordena_lista_alf(lista_locais lista);
lista_locais ordena_lista_pop(lista_locais lista);
void troca(lista_locais a, lista_locais b);
lista_locais escolher_pi(lista_pessoas lista, int id, lista_locais lista_locais);
int procura_lista_pi(lista_pessoas lista, int id_pi, int id_pessoa);
int tamanho_lista_pi(lista_p_interesse lista);
void fazer_viagem(lista_pessoas lista_pessoa, int id, lista_locais lista_locais);
int pdi_in_local(int id_pdi, lista_p_interesse lista_pdis);
lista_p_interesse ordena_lista_alf_pdi(lista_p_interesse lista);
lista_p_interesse ordena_lista_pop_pdi(lista_p_interesse lista);
void troca_pdis(lista_p_interesse a, lista_p_interesse b);
lista_pessoas apaga_lista_pessoa(lista_pessoas lista);
lista_locais apaga_lista_locais(lista_locais lista);
int tamanho_lista_locais(lista_locais lista);
lista_pessoas remover_preferencia(lista_pessoas lista_pessoas, lista_locais lista_locais, int id);
lista_pessoas remover_preferencia_local(lista_pessoas lista_pessoas, int id, lista_locais lista_locais);
void deleteNodelocal(struct lnodelocais** head_ref, int id);
void deleteNodepdi(struct lnodepinteresse** head_ref, int id);
lista_pessoas remover_preferencia_pi(lista_pessoas lista_pessoas, int id, lista_locais lista_locais);
lista_locais diminui_pop_pi(lista_locais lista, int id_pi);
lista_locais diminui_pop(lista_locais lista, int id);
void deleteNodehot(struct lnodepinteresse** head_ref, int id);

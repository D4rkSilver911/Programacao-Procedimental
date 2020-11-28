#include "header.h"
/*---------------------------------------------------------------------------------*/

int main(int argc, char const *argv[]) {
  lista_locais lista_local = NULL; //Criar as listas que vao ser usadas
  lista_pessoas lista_pessoa = NULL;
  lista_local = ler_fich_locais(lista_local); //Importar dos files
  lista_pessoa = ler_fich_pessoas(lista_pessoa, lista_local);
  menu(lista_local, lista_pessoa); //Programa em si
  apaga_lista_locais(lista_local); //Libertar as listas
  apaga_lista_pessoa(lista_pessoa);

}

void menu(lista_locais lista_locais, lista_pessoas lista_pessoas){
  int n, a, i, id;

  i = tamanho_lista(lista_pessoas); //Numero de users

  do {
    system("cls");
    printf("\n\n");
    printf("                                    MENU VIAGEM                          \n");
    printf("            ______________________________________________________________\n");
    printf("            |         1. Registar Novo User                              |\n");
    printf("            |         2. Listar Users Registados                         |\n");
    printf("            |         3. Alterar Dados Pessoais                          |\n");
    printf("            |         4. Listar Todos os Locais e Pontos de Interesse    |\n");
    printf("            |         5. Escolher Preferencia                            |\n");
    printf("            |         6. Remover Preferencia                             |\n");
    printf("            |         7. Listar Locais e Pontos de Interesse + Populares |\n");
    printf("            |         8. Construir Viagem                                |\n");
    printf("            |         0. Sair                                            |\n");
    printf("            ______________________________________________________________\n");

    do {
      printf("\n");
      printf("\t    Que opcao deseja realizar: ");
      scanf("%d",&n);
    } while((n<0 || n>8));

    switch (n) {
      case 1:
      lista_pessoas = registar_user(lista_pessoas, i);
      i++;
      break;
      case 2:
      imprime_lista_pessoas(lista_pessoas);
      break;
      case 3:
      if(tamanho_lista(lista_pessoas) == 0){
        printf("\n\tNao ha users registados!\n");
        break;
      }
      imprime_lista_pessoas(lista_pessoas);
      do{
        printf("\n\tEscolha o User que quer alterar os dados: ");
        scanf("%d", &id);
      }while(procura_na_lista(lista_pessoas, id) == 0);
      lista_pessoas = alterar_dados(lista_pessoas, id);

      break;
      case 4:
      printf("\n\tLISTA DE LOCAIS: \n\n");
      lista_locais = ordena_lista_alf(lista_locais);
      imprime_lista_locais(lista_locais);
      break;
      case 5:
      if(tamanho_lista(lista_pessoas) == 0){
        printf("\n\tNao ha users registados!\n");
        break;
      }
      imprime_lista_pessoas(lista_pessoas);
      do{
        printf("\n\tInsira o id do user pretendido: ");
        scanf("%d", &id);
      }while(procura_na_lista(lista_pessoas, id)==0);

      lista_pessoas = escolher_preferencia(lista_pessoas, lista_locais, id);
      break;

      case 6:
      if(tamanho_lista(lista_pessoas) == 0){
        printf("\n\tNao ha users registados!\n");
        break;
      }
      imprime_lista_pessoas(lista_pessoas);
      do{
        printf("\n\tInsira o id do user pretendido: ");
        scanf("%d", &id);
      }while(procura_na_lista(lista_pessoas, id) == 0);
      lista_pessoas = remover_preferencia(lista_pessoas, lista_locais, id);
      break;
      case 7:
      printf("\n\t\tLISTA DE LOCAIS: \n\n");
      lista_locais = ordena_lista_alf(lista_locais);
      lista_locais = ordena_lista_pop(lista_locais);
      imprime_lista_locais(lista_locais);
      break;
      case 8:
      if(tamanho_lista(lista_pessoas) == 0){
        printf("\n\tNao ha users registados!\n");
        break;
      }
      printf("\n\tFAZER VIAGEM!\n\n");
      imprime_lista_pessoas(lista_pessoas);
      do{
        printf("\n\tEscolha o User que quer fazer a viagem: ");
        scanf("%d", &id);
      }while(procura_na_lista(lista_pessoas, id) == 0);

      fazer_viagem(lista_pessoas, id, lista_locais);
      break;
      case 0:
      guarda_fich_pessoas(lista_pessoas);
      exit(0);
      break;

    }
    printf("\n\tDeseja realizar mais alguma operacao? [1,0]: ");
    printf("\n\t1 - Sim\n\t0 - Nao\n");
    do{
      printf("\n\tEscolha: ");
      scanf("%d",&a);
    }while(a!=0 && a!=1 && (a>'a' && a<'Z'));
    if(a==0){
      guarda_fich_pessoas(lista_pessoas);
    }
  }while(a==1);
}

/*Ler ficheiro pessoas (quando inicia o programa)*/
lista_pessoas ler_fich_pessoas(lista_pessoas lista, lista_locais lista_locais){

  FILE *fp;
  int i, j;
  fp = fopen("users.txt", "r");
  char linha[MAX];
  int id;
  char nome[MAX];
  char morada[MAX];
  Data nascimento;
  int telefone;
  char data[MAX];
  int id_local, id_pdi;
  int n_locais, n_pdis;
  int ehot = 0;
  struct lnodepinteresse* pdis_local;
  struct lnodelocais* aux;
  struct lnodepinteresse* temp_interesse;
  struct lnodelocais* temp_l;
  struct lnodepinteresse* temp_p;
  struct lnodepessoas* temp_pessoas = lista;
  char c;
  char help[MAX];

  if(fp == NULL){
    return 0;
  }
  else{
    fgets(linha, MAX, fp);
    while(!feof(fp)){
      fgets(linha, MAX, fp);
      if(strlen(linha) == 0){
        break;
      }
      id = atoi(strtok(linha, "|"));
      strcpy(nome, strtok(NULL, "|"));
      strcpy(morada, strtok(NULL, "|"));
      strcpy(data, strtok(NULL, "|"));
      telefone = atoi(strtok(NULL, "|"));
      n_locais = atoi(strtok(NULL, "|"));
      n_pdis = atoi(strtok(NULL, "|"));
      nascimento.dia = atoi(strtok(data, "/"));
      nascimento.mes = atoi(strtok(NULL, "/"));
      nascimento.ano = atoi(strtok(NULL, "\n"));

      lista = insere_fim_lista_pessoas(lista, nome, morada, nascimento, telefone, id);
      for(i = 0; i < n_locais; i++){
        fgets(linha, MAX, fp);

        id_local = atoi(strtok(linha, "\n"));
        aux = lista_locais;
        //Procurar o local
        while(aux){
          if(aux -> id == id_local){
            temp_l = aux;
            break;
          }
          else{
            aux = aux -> next;
          }
        }
        temp_pessoas = lista;

        //Procurar a pessoa
        while(temp_pessoas){
          if(temp_pessoas->id == id){
              temp_pessoas-> locais =insere_ordenado_lista(temp_pessoas -> locais, temp_l->id, temp_l->cidade, temp_l->pais, temp_l->p_interesse, temp_l->popularidade+1);
              aumenta_pop(lista_locais, id_local);
              break;
            }
          else{
            temp_pessoas = temp_pessoas -> next;
          }
        }

      }
      fgets(linha, MAX, fp);

      for(j = 0; j < n_pdis; j++){
        fgets(linha, MAX, fp);
        c = linha[0];

        if(c == 'H'){
          ehot = 1;
          strcpy(help,strtok(linha, "-"));
          id_pdi = atoi(strtok(NULL, "\n"));


        }
        else{
          id_pdi = atoi(strtok(linha, "\n"));
        }

        aux = lista_locais;
        while(aux){
          temp_interesse = aux -> p_interesse;
          while(temp_interesse){
            if(temp_interesse->id == id_pdi){
              temp_p = temp_interesse;

              break;
            }
            else{
              temp_interesse = temp_interesse->next;
          }

          }
          aux = aux -> next;
        }
        temp_pessoas = lista;
        while(temp_pessoas){
          if(temp_pessoas->id == id){
              temp_pessoas-> p_interesse = insere_fim_lista_pi(id_pdi, temp_pessoas -> p_interesse, temp_p->descricao, temp_p->nome, temp_p->entrada, temp_p->saida, temp_p->popularidade+1);
              //printf("%s\n", temp_p->nome);
              if(ehot == 1){
                strcpy(temp_pessoas->hot, temp_p->nome);
                temp_pessoas->hasHot = 1;
                ehot = 0;
              }
              aumenta_pop_pi(lista_locais, id_pdi);
              break;
            }
          else{
            temp_pessoas = temp_pessoas -> next;
          }
        }


      }


    }
  }

  return lista;
}

/*Ler ficheiro dos locais (quando inicia o programa)*/
lista_locais ler_fich_locais(lista_locais lista){
  int j, i, ind = 0, k = 1;
  lista_p_interesse listaPI[NUM_LOCAIS];
  FILE *fp = fopen("locais.txt", "r");
  char linha[MAX]; //Auxiliar para ler uma linha do ficheiro
  char linha2[MAX];
  int id, aux, tipo; //Id do loca, aux (nº PIs) , tipo (bar, parque, uni)
  char cidade[MAX]; //Local
  char pais[MAX]; //Local
  Hora entrada, saida; //Pi
  char nome[MAX]; //pi
  char descricao[MAX]; //pi
  for(i = 0; i < NUM_LOCAIS; i++){
    listaPI[i] = NULL;
  }
  if(!fp){
    return 0;
  }
  else{
    while(!feof(fp)){
      fgets(linha, MAX, fp);
      //Ler a primeira linha do local e meter tudo nas suas variaveis
      id = atoi(strtok(linha, ":"));
      aux = atoi(strtok(NULL, ":"));
      strcpy(cidade, strtok(NULL, ":"));
      strcpy(pais, strtok(NULL, ":"));
      //Percorrer o numero de PIs do local em questão
      for(j = 0; j < aux; j++){
        fgets(linha, MAX, fp);
        tipo = atoi(strtok(linha, ":")); //Tipo do local
        switch (tipo) {
          case 1:
          strcpy(descricao, "Bar");
          break;
          case 2:
          strcpy(descricao, "Parque");
          break;
          case 3:
          strcpy(descricao, "Museu");
          break;
          case 4:
          strcpy(descricao, "Universidade");
          break;
          default:
          break;
        } //Descricao do PI
        strcpy(nome, strtok(NULL, ":"));
        entrada.hora = atoi(strtok(NULL, ":"));
        entrada.minuto = atoi(strtok(NULL, ":"));
        saida.hora = atoi(strtok(NULL, ":"));
        saida.minuto = atoi(strtok(NULL, ":"));
        listaPI[ind] = insere_ordenado_lista_pi(k,listaPI[ind], descricao, nome, entrada, saida, 0);
        k++;
      }
    lista = insere_ordenado_lista(lista, id, cidade, pais, listaPI[ind], 0);
    ind++;
    fgets(linha, MAX, fp);
    }
  }
  fclose(fp);
  return lista;
}

/*Quando fecha, atualiza o ficheiro com novos users*/
void guarda_fich_pessoas(lista_pessoas lista){
  FILE *fp;
  lista_pessoas aux = lista;
  struct lnodelocais* temp;
  struct lnodepinteresse* temp_pdi;
  int n_locais;
  int n_pdis;

  fp = fopen("users.txt", "w");
  while(aux){
    n_locais = tamanho_lista_locais(aux->locais);
    n_pdis = tamanho_lista_pi(aux->p_interesse);
    temp = aux->locais;
    temp_pdi = aux->p_interesse;
    fprintf(fp,"\n%d|%s|%s|%d/%d/%d|%d|%d|%d",aux->id, aux->nome, aux->morada, aux->data_nascimento.dia, aux->data_nascimento.mes, aux->data_nascimento.ano, aux->telefone, n_locais, n_pdis);
    while(temp){
      fprintf(fp, "\n%d", temp->id);
      temp = temp->next;
    }
    fprintf(fp,"\n");
    while(temp_pdi){
      if(strcmp(temp_pdi->nome, aux->hot)==0){
        fprintf(fp, "\nH-%d", temp_pdi->id);
      }
      else{
        fprintf(fp, "\n%d", temp_pdi->id);
      }

      temp_pdi = temp_pdi -> next;
    }
    aux = aux->next;
  }
  fclose(fp);
}

/*Registar um novo utilizador */
lista_pessoas registar_user(lista_pessoas lista, int id){
  FILE *fp = fopen("users.txt", "a");

  char nome[MAX];
  char morada[MAX];
  Data nascimento;
  int telefone;

  printf("\n\t\tREGISTAR USER!\n\n");
  printf("\tInsira o nome: ");
  scanf(" %[^\n]s", nome);


  printf("\tInsira a morada: ");
  scanf(" %[^\n]s", morada);

  do{
    printf("\tInsira a data de nascimento: ");
    scanf("%d/%d/%d", &nascimento.dia, &nascimento.mes, &nascimento.ano);
  }while(verificarData(nascimento.dia, nascimento.mes, nascimento.ano)==1);
  printf("\tInsira o telefone: ");
  scanf("%d",&telefone);

  printf("\n\tUSER REGISTADO COM SUCESSO!\n");
  lista = insere_fim_lista_pessoas(lista, nome, morada, nascimento, telefone, id);

  fclose(fp);
  return lista;
}

/*Funções para alterar os dados*/
lista_pessoas alterar_dados(lista_pessoas lista, int id){
  int n, telefone;
  char nome[MAX];
  char morada[MAX];
  printf("\n\n\t\tALTERAR DADOS\n");
  printf("\tEscolha que dado deseja alterar: \n\t1. Nome\n\t2. Morada\n\t3. Telefone\n");
  printf("\tOpcao: ");
  scanf("%d", &n);
  switch(n){
    case 1:
    printf("\tInsira o novo nome: ");
    scanf(" %[^\n]s", nome);
    lista = altera_nome(lista, nome, id);
    break;
    case 2:
    printf("\tInsira a nova morada: ");
    scanf(" %[^\n]s", morada);
    lista = altera_morada(lista, morada, id);
    break;
    case 3:
    printf("\tInsira o novo telefone: ");
    scanf("%d", &telefone);
    lista = altera_telefone(lista, telefone, id);
    break;

  }
  return lista;
}
lista_pessoas altera_nome(lista_pessoas lista, char nome[], int id){
  lista_pessoas atual;
  atual = lista;
  while(atual){
    if(atual->id == id){
      strcpy(atual->nome, nome);
      break;
    }
    else{
      atual = atual->next;
    }
  }
  return lista;
}
lista_pessoas altera_morada(lista_pessoas lista, char morada[], int id){
  lista_pessoas atual;
  atual = lista;
  while(atual){
    if(atual->id == id){
      strcpy(atual->morada, morada);
      break;
    }
    else{
      atual = atual->next;
    }
  }
  return lista;
}
lista_pessoas altera_telefone(lista_pessoas lista, int telefone, int id){
  lista_pessoas atual;
  atual = lista;
  while(atual){
    if(atual->id == id){
      atual->telefone = telefone;
      break;
    }
    else{
      atual = atual->next;
    }
  }
  return lista;
}

/*Escolher/Remover preferencia do user*/
lista_pessoas escolher_preferencia(lista_pessoas lista_pessoas, lista_locais lista_locais, int id){
  int opcao;
  printf("\n\tESCOLHER PREFERENCIA: \n");

  printf("\tQuer escolher: \n ");
  printf("\t1 - Local\n");
  printf("\t2 - Ponto de Interesse\n");
  printf("\tOpcao: ");

  opcao:
  do{
    scanf("%d", &opcao);
  }while(opcao != 1 && opcao != 2);

  switch(opcao){
    case 1:
    if(escolher_preferencia_local(lista_pessoas, id, lista_locais)==0){
      goto opcao;
    }
    break;

    case 2: escolher_preferencia_ponto_interesse(lista_pessoas, id, lista_locais);
    break;

  }

  return lista_pessoas;
}
lista_pessoas escolher_preferencia_local(lista_pessoas lista_pessoas, int id, lista_locais lista_locais){
  int id_local;
  struct lnodelocais* temp;
  local:
  printf("\t Insira o ID do local a preferir: ");
  scanf("%d", &id_local);
  if(procura_lista(lista_pessoas, id_local, id)==0){
    printf("\n\tLocal ja escolhido!\n");
    goto local;
  }
  if(id_local > 20 || id_local < 0){
    printf("\n\tLocal nao existe!\n");
    goto local;
  }
  //Procurar o local que o user escolheu e armazená-lo numa variavel temp
  while(lista_locais){
    if(lista_locais -> id == id_local){
      temp = lista_locais;
      break;
    }
    else{
      lista_locais = lista_locais -> next;
    }
  }
  //Percorrer a lista de pessoas e dps adicionar na sua lista de locais o local armazenado em temp
  while(lista_pessoas){
    if(lista_pessoas->id == id){
      if(tamanho_lista_locais(lista_pessoas -> locais)>2){
        printf("\tJa escolheu 3 locais. Nao pode ter mais!\n");
        goto end;
      }
      else{
        lista_pessoas -> locais = insere_ordenado_lista(lista_pessoas -> locais, temp->id, temp->cidade, temp->pais, temp->p_interesse, temp->popularidade+1);
        aumenta_pop(lista_locais, id_local);
        break;
      }

    }
    else{
      lista_pessoas = lista_pessoas -> next;
    }
  }
  end:
  return lista_pessoas;
}
lista_pessoas escolher_preferencia_ponto_interesse(lista_pessoas lista_pessoas, int id, lista_locais lista_locais){
  int id_pi, hot, hasHot;
  struct lnodelocais* temp;
  struct lnodepessoas* aux = lista_pessoas;
  escolha:
  printf("\tQuer escolher o Ponto de Interesse Hot? \n");
  printf("\t1- Sim\n");
  printf("\t2- Nao");
  do{
    printf("\n\tOpcao: ");
    scanf("%d", &hot);
  }while(hot!=1 && hot!=2);

  //Encontrar a pessoa correta e ver se ela já escolheu algum hot
  while(aux){
    if(aux->id == id){
      hasHot = aux->hasHot;
      break;
    }
    else{
      aux = aux -> next;
    }
  }
  switch(hot){
    case 1:if(hasHot == 1){
      printf("\n\tNao pode escolher mais que um ponto Hot.\n");
      goto escolha;
    }
    else{
      lista_locais = escolher_hot(lista_pessoas, id, lista_locais);
      aux -> hasHot = 1;
    }
    break;

    case 2:
    lista_locais = escolher_pi(lista_pessoas, id, lista_locais);
    break;

  }
}
lista_locais escolher_hot(lista_pessoas lista, int id, lista_locais lista_locais){
  int id_hot;
  struct lnodepinteresse* temp;
  lista_p_interesse temp_interesse;
  struct lnodelocais* aux = lista_locais;


  local:
  printf("\n\t Insira o ID do Ponto Interesse Hot: ");
  scanf("%d", &id_hot);
  if(procura_lista_pi(lista, id_hot, id)==0){
    printf("\n\tPonto de Interesse ja escolhido!\n");
    goto local;
  }
  //Percorrer os locais e os seus pdis e ver qual pdi corresponde ao que o user escolheu e armazená-lo em temp
  while(aux){
    temp_interesse = aux -> p_interesse;
    while(temp_interesse){
      if(temp_interesse->id == id_hot){
        temp = temp_interesse;
        break;
      }
      else{
        temp_interesse = temp_interesse->next;
    }

    }
    aux = aux -> next;
  }
  //Percorrer a lista de pessoas e adicionar na sua lista de pdis o pdi anteriormente armazenado
  while(lista){
    if(lista->id == id){
        strcpy(lista->hot, temp->nome);
        lista-> p_interesse = insere_inicio_lista_pi(id_hot, lista -> p_interesse, temp->descricao, temp->nome, temp->entrada, temp->saida, temp->popularidade+1);
        aumenta_pop_pi(lista_locais, id_hot);
        break;
      }
    else{
      lista = lista -> next;
    }
  }
  return lista_locais;
}
lista_locais escolher_pi(lista_pessoas lista, int id, lista_locais lista_locais){

  //Funcao igual ao escolher_hot
  int id_pi;
  struct lnodepinteresse* temp;
  lista_p_interesse temp_interesse;
  struct lnodelocais* aux = lista_locais;

  local:
  printf("\n\t Insira o ID do Ponto Interesse: ");
  scanf("%d", &id_pi);
  if(procura_lista_pi(lista, id_pi, id)==0){
    printf("\n\tPonto de Interesse ja escolhido!\n");
    goto local;
  }

  while(aux){
    temp_interesse = aux -> p_interesse;
    while(temp_interesse){
      if(temp_interesse->id == id_pi){
        temp = temp_interesse;
        break;
      }
      else{
        temp_interesse = temp_interesse->next;
    }

    }
    aux = aux -> next;
  }

  while(lista){
    if(lista->id == id){
        lista-> p_interesse = insere_fim_lista_pi(id_pi, lista -> p_interesse, temp->descricao, temp->nome, temp->entrada, temp->saida, temp->popularidade+1);
        aumenta_pop_pi(lista_locais, id_pi);
        break;
      }
    else{
      lista = lista -> next;
    }
  }
  return lista_locais;
}
lista_pessoas remover_preferencia(lista_pessoas lista_pessoas, lista_locais lista_locais, int id){
  int opcao;
  printf("\n\tREMOVER PREFERENCIA: \n");

  printf("\tQuer remover: \n ");
  printf("\t1 - Local\n");
  printf("\t2 - Ponto de Interesse\n");
  printf("\tOpcao: ");

  opcao:
  do{
    scanf("%d", &opcao);
  }while(opcao != 1 && opcao != 2);

  switch(opcao){
    case 1:
    if(remover_preferencia_local(lista_pessoas, id, lista_locais)==0){
      goto opcao;
    }
    break;

    case 2: remover_preferencia_pi(lista_pessoas, id, lista_locais);
    break;

  }

  return lista_pessoas;
}
lista_pessoas remover_preferencia_local(lista_pessoas lista_pessoas, int id, lista_locais lista_locais){
  int id_local;
  struct lnodelocais* temp;
  struct lnodepessoas* aux = lista_pessoas;

  printf("\n\tInsira o ID do local que quer remover: ");
  scanf("%d", &id_local);

  while(aux){
    if(aux->id == id){
      deleteNodelocal(&aux->locais, id_local);
      diminui_pop(lista_locais, id_local);
      break;
    }
    else{
      aux = aux->next;
    }
  }

  return lista_pessoas;

}
lista_pessoas remover_preferencia_pi(lista_pessoas lista_pessoas, int id, lista_locais lista_locais){
  int id_pdi;
  struct lnodepessoas* aux = lista_pessoas;
  struct lnodepinteresse* temp;


  printf("\n\tInsira o ID do Ponto de Interesse que quer remover: ");
  scanf("%d", &id_pdi);

  while(aux){
    temp = aux->p_interesse;
    if(aux->id == id){
      deleteNodepdi(&aux->p_interesse, id_pdi);
      diminui_pop_pi(lista_locais, id_pdi);
      while(temp){


          if(temp->id == id_pdi){

            if(aux->hasHot == 1){
              if(strcmp(temp->nome, aux->hot)==0){

                aux->hot[0] = '\0';

                aux->hasHot = 0;
                break;
              }
            }
            else{
              break;
            }

          }
          else{
              temp = temp->next;
          }

        }
        break;
      }

    else{
      aux = aux->next;
    }

  }
}
void deleteNodelocal(struct lnodelocais** head_ref, int id){
  struct lnodelocais* temp = *head_ref, *prev;

  if(temp != NULL && temp->id == id){
    *head_ref = temp->next;
    free(temp);
    return;
  }

  while(temp!=NULL && temp->id != id){
    prev = temp;
    temp = temp->next;
  }
  if(temp == NULL) return;

  prev->next = temp->next;
  free(temp);

}
void deleteNodepdi(struct lnodepinteresse** head_ref, int id){
  struct lnodepinteresse* temp = *head_ref, *prev;

  if(temp != NULL && temp->id == id){
    *head_ref = temp->next;
    free(temp);
    return;
  }

  while(temp!=NULL && temp->id != id){
    prev = temp;
    temp = temp->next;
  }
  if(temp == NULL) return;

  prev->next = temp->next;
  free(temp);

}

/*Funções de imprimir as listas*/
void imprime_lista_pessoas(lista_pessoas lista){
  lista_pessoas temp = lista;
  printf("\n\t\tUSERS REGISTADOS:\n\n");
  while(temp){
    printf("\tUser[%d]: %s | %s | %d/%d/%d | %d\n", temp->id, temp->nome, temp->morada, temp->data_nascimento.dia, temp->data_nascimento.mes, temp->data_nascimento.ano, temp->telefone);
    printf("\t\tLista de Locais preferidos: ");
    imprime_locais_pessoa(temp->locais);
    printf("\n\t\tLista de PIs preferidos: ");
    imprime_pi_pessoa(temp->p_interesse);
    printf("\n\n");
    temp = temp->next;

  }
}
void imprime_lista_pi(lista_p_interesse lista){
  lista_p_interesse act = lista;
  while(act){
    printf("\t%d: %s | %s | Popularidade:%d | Horario:%d:%d-%d:%d\n ",act->id, act->descricao,act->nome,act->popularidade,act->entrada.hora, act->entrada.minuto,act->saida.hora, act->saida.minuto);
    act = act -> next;
  }
}
void imprime_lista_locais(lista_locais lista){
  lista_locais act = lista;
  int i;
  char str[MAX];
  while(act){
    for(i=0;act->pais[i]!='\0';i++)
       {
       if(act->pais[i]=='\n')
           {
            act->pais[i]=' ';

           }
        }
        act = act->next;
  }
  act = lista;
  while(act){
    printf("\tLocal %d: Cidade: %s | Pais: %s | Popularidade: %d\n", act->id, act->cidade, act->pais, act->popularidade);
    printf("\tPontos de Interesse: \n");
    imprime_lista_pi(act->p_interesse);
    act = act -> next;
    printf("\n");
  }

}
void imprime_locais_pessoa(lista_locais lista){
  lista_locais temp = lista;
  while(temp){
    printf("\t%d. %s  ", temp->id, temp->cidade);
    temp = temp->next;
  }
}
void imprime_pi_pessoa(lista_p_interesse lista){
  lista_p_interesse temp = lista;
  while(temp){
    printf("\t%d. %s ", temp->id, temp->nome);
    temp = temp->next;
  }

}

/*Funções de inserir nas listas (fim, inicio ou ordenado)*/
lista_pessoas insere_fim_lista_pessoas(lista_pessoas lista, char nome[], char morada[], Data data, int telefone, int id){
  lista_pessoas novo, atual;

  novo = (lista_pessoas) malloc(sizeof(struct lnodepessoas));

  strcpy(novo->nome, nome);
  strcpy(novo->morada, morada);
  novo->data_nascimento = data;
  novo->telefone = telefone;
  novo->id = id;
  novo->locais = NULL;
  novo->next = NULL;
  novo->hasHot = 0;

  if(lista == NULL){ //Se a lista estiver vazia, basta por a lista a apontar para o novo nó
    lista = novo;
  }
  else{ //Se a lista nao estiver vazia, temos que percorrer a lista toda à procura do ultimo, para depois pormos o campo next desse nó a null
    atual = lista; //Começamos no primeiro nó
    while(atual->next){ //Enquanto nao chegarmos ao fim,
      atual = atual->next; //Vamos passando de nó em nó
    }
    atual -> next = novo; //Metemos no fim o novo nó , como "atual" e "lista" estão a apontar para o mesmo, ao alterar o atual, altera o lista, por isso é que podemos dar return em lista

  }
  return lista;
}
lista_p_interesse insere_fim_lista_pi(int id, lista_p_interesse lista, char descricao[], char nome[], Hora entrada, Hora saida, int pop){
  lista_p_interesse atual, novo;
  novo = (lista_p_interesse) malloc(sizeof(struct lnodepinteresse));
  novo->id = id;
  novo->popularidade = pop;
  strcpy(novo->nome, nome);
  strcpy(novo->descricao, descricao);
  novo->entrada = entrada;
  novo->saida = saida;
  novo->next = NULL;

  if(lista == NULL){ //Se a lista estiver vazia, basta por a lista a apontar para o novo nó
    lista = novo;
  }
  else{ //Se a lista nao estiver vazia, temos que percorrer a lista toda à procura do ultimo, para depois pormos o campo next desse nó a null
    atual = lista; //Começamos no primeiro nó
    while(atual->next){ //Enquanto nao chegarmos ao fim,
      atual = atual->next; //Vamos passando de nó em nó
    }
    atual -> next = novo; //Metemos no fim o novo nó , como "atual" e "lista" estão a apontar para o mesmo, ao alterar o atual, altera o lista, por isso é que podemos dar return em lista

  }
  return lista;
}
lista_locais insere_fim_lista_local(lista_locais lista, int id, char cidade[], char pais[], lista_p_interesse listaPI, int pop){

  lista_locais atual, novo;
  novo = (lista_locais) malloc(sizeof(struct lnodelocais));
  novo->popularidade = pop;
  novo->id = id;
  strcpy(novo->cidade, cidade);
  strcpy(novo->pais, pais);
  novo->p_interesse = listaPI;
  novo->next = NULL;

  if(lista == NULL){ //Se a lista estiver vazia, basta por a lista a apontar para o novo nó
    lista = novo;
  }
  else{ //Se a lista nao estiver vazia, temos que percorrer a lista toda à procura do ultimo, para depois pormos o campo next desse nó a null
    atual = lista; //Começamos no primeiro nó
    while(atual->next){ //Enquanto nao chegarmos ao fim,
      atual = atual->next; //Vamos passando de nó em nó
    }
    atual -> next = novo; //Metemos no fim o novo nó , como "atual" e "lista" estão a apontar para o mesmo, ao alterar o atual, altera o lista, por isso é que podemos dar return em lista

  }
  return lista;
}
lista_p_interesse insere_ordenado_lista_pi(int id, lista_p_interesse lista, char descricao[], char nome[], Hora entrada, Hora saida, int pop){
  lista_p_interesse atual, novo, ant;
  novo = (lista_p_interesse) malloc(sizeof(struct lnodepinteresse));
  novo->popularidade = pop;
  novo->id = id;
  strcpy(novo->nome, nome);
  strcpy(novo->descricao, descricao);
  novo->entrada = entrada;
  novo->saida = saida;
  novo->next = NULL;

  if(lista == NULL || strcmp(lista->nome, novo->nome)>0){ //Se invertermos a ordem das condições, nao vai dar, porque ele iria verificar a primeira condicao (lista->info) e como ela estaria vazia
    //no inicio, nao teriamos o valor lista->info, logo iria dar segmentation fault
    novo->next = lista;
    lista = novo;
  }
  else{
    atual = lista->next; //Vai percorrer a lista ate chegar ao fim ou até encontrar o valor maior do que o que queremos inserir
    ant = lista; //Vai percorrer juntamente com o atual, mas um nó atrás, para dps podermos guardar o endereço do nó anterior.
    while(atual && strcmp(atual->nome, novo->nome)<0){
      atual = atual ->next; //Ou: ant = atual; atual = atual->next;
      ant = ant->next;
    }
    novo->next = atual; //Quando acaba o ciclo While, o valor novo a inserir vai estar sempre entre o ant e o atual, logo basta alterar as cenas.
    ant->next = novo;

  }
  return lista;
}
lista_locais insere_ordenado_lista(lista_locais lista, int id, char cidade[], char pais[], lista_p_interesse listaPI, int pop){
  lista_locais atual, novo, ant;
  novo = (lista_locais) malloc(sizeof(struct lnodelocais));
  novo->popularidade = pop;
  novo->id = id;
  strcpy(novo->cidade, cidade);
  strcpy(novo->pais, pais);
  novo->p_interesse = listaPI;
  novo->next = NULL;

  if(lista == NULL || strcmp(lista->cidade, novo->cidade)>0){ //Se invertermos a ordem das condições, nao vai dar, porque ele iria verificar a primeira condicao (lista->info) e como ela estaria vazia
    //no inicio, nao teriamos o valor lista->info, logo iria dar segmentation fault
    novo->next = lista;
    lista = novo;
  }
  else{
    atual = lista->next; //Vai percorrer a lista ate chegar ao fim ou até encontrar o valor maior do que o que queremos inserir
    ant = lista; //Vai percorrer juntamente com o atual, mas um nó atrás, para dps podermos guardar o endereço do nó anterior.
    while(atual && strcmp(atual->cidade, novo->cidade)<0){
      atual = atual ->next; //Ou: ant = atual; atual = atual->next;
      ant = ant->next;
    }
    novo->next = atual; //Quando acaba o ciclo While, o valor novo a inserir vai estar sempre entre o ant e o atual, logo basta alterar as cenas.
    ant->next = novo;

  }
  return lista;
}
lista_locais insere_inicio_lista(lista_locais lista, int id, char cidade[], char pais[], lista_p_interesse listaPI, int pop){
  lista_locais novo, temp;

  novo = (lista_locais) malloc(sizeof(struct lnodelocais));
  novo->popularidade = pop;
  novo->id = id;
  strcpy(novo->cidade, cidade);
  strcpy(novo->pais, pais);
  novo->p_interesse = listaPI;
  novo->next = NULL;

  novo->next = lista; //Novo aponta para o primeiro valor
  lista = novo; //Lista aponta para o novo
}
lista_p_interesse insere_inicio_lista_pi(int id, lista_p_interesse lista, char descricao[], char nome[], Hora entrada, Hora saida, int pop){
  lista_p_interesse atual, novo;
  novo = (lista_p_interesse) malloc(sizeof(struct lnodepinteresse));
  novo->id = id;
  novo->popularidade = pop;
  strcpy(novo->nome, nome);
  strcpy(novo->descricao, descricao);
  novo->entrada = entrada;
  novo->saida = saida;
  novo->next = NULL;



  novo->next = lista; //Novo aponta para o primeiro valor
  lista = novo; //Lista aponta para o novo

  return lista;
}

/*Verificar a data*/
int verificarData(int dia, int mes, int ano){
  int z = 1;
  if (mes<1 || mes>12){
    return z;
  }
  if (mes==1 || mes==3 || mes==5 || mes==7 || mes==8 || mes==10 || mes==12){
    if(dia>31 || dia <1){
      return z;
    }
  }
  if (mes==4 || mes==6 || mes==9 || mes==11){
    if(dia>30 || dia <1){
      return z;
    }
  }
  if (mes==2){
    if(dia>28 || dia<1){
      return z;
    }
  }
}

/*Funções de procurar um elemento na lista*/
int procura_na_lista(lista_pessoas lista, int id){
  lista_pessoas temp = lista;
  while(temp){
    if(temp->id == id){
      return 1;
    }
    temp = temp->next;
  }
  return 0;
}
int procura_na_lista_locais(lista_locais lista, int id){
  lista_locais temp = lista;
  while(temp){
    if(temp->id == id){
      return 1;
    }
    temp = temp->next;
  }
  return 0;
}
int procura_na_lista_pi(lista_locais lista,int id){
  lista_locais temp = lista;
  while(temp){
    while(temp->p_interesse){
      if(temp->p_interesse->id == id){
        return 1;
      }
      else{
        temp->p_interesse = temp->p_interesse->next;
      }
    }
    temp = temp->next;
  }
  return 0;

}
int procura_lista(lista_pessoas lista, int id_local, int id_pessoa){
  lista_pessoas temp = lista;
  lista_locais temp2;
  int conta = 1;
  if(!temp){
    return 1;
  }
  else{
    while(temp){
      if(temp->id == id_pessoa){
        temp2 = temp->locais;
        break;
      }
      else{
        temp = temp->next;
      }
    }
  }
  if(!temp2){
    return conta;
  }
  else{
    while(temp2){
      if(temp2->id == id_local){
        conta = 0;
        break;
      }
      else{
        temp2 = temp2->next;
      }
    }
  }

  return conta;
}
int procura_lista_pi(lista_pessoas lista, int id_pi, int id_pessoa){
  lista_pessoas temp = lista;
  lista_p_interesse temp2;
  int conta = 1;
  if(!temp){
    return 1;
  }
  else{
    while(temp){
      if(temp->id == id_pessoa){
        temp2 = temp->p_interesse;
        break;
      }
      else{
        temp = temp->next;
      }
    }
  }
  if(!temp2){
    return conta;
  }
  else{
    while(temp2){
      if(temp2->id == id_pi){
        conta = 0;
        break;
      }
      else{
        temp2 = temp2->next;
      }
    }
  }

  return conta;
}

/*Funções para calcular o tamanho da lista*/
int tamanho_lista(lista_pessoas lista){
  lista_pessoas temp = lista;
  int i = 0;
  if(!lista){
    return 0;
  }
  else{
    while(temp){
      i++;
      temp = temp -> next;
    }
  }
  return i;
}
int tamanho_lista_locais(lista_locais lista){
  lista_locais temp = lista;
  int i = 0;
  if(!lista){
    return 0;
  }
  else{
    while(temp){
      i++;
      temp = temp -> next;
    }
  }
  return i;
}
int tamanho_lista_pi(lista_p_interesse lista){
  lista_p_interesse temp = lista;
  int i = 0;
  if(!lista){
    return 0;
  }
  else{
    while(temp){
      i++;
      temp = temp -> next;
    }
  }
  return i;
}

/*Funções de ordenar as listas*/
lista_locais ordena_lista_pop(lista_locais lista){
  lista_locais temp = lista;
  int swapped, i;
  lista_locais ptr1;
  lista_locais lptr = NULL;
  if(lista == NULL){
    return NULL;
  }
  do{
    swapped = 0;
    ptr1 = temp;
    while(ptr1 -> next != lptr){
      ordena_lista_pop_pdi(ptr1->p_interesse);
      if(ptr1->popularidade < ptr1->next->popularidade){
        troca(ptr1, ptr1->next);
        swapped = 1;
      }
      ptr1 = ptr1->next;
    }
    lptr = ptr1;
  }while(swapped);

  return lista;
}
lista_locais ordena_lista_alf(lista_locais lista){
  lista_locais temp = lista;
  int swapped, i;
  lista_locais ptr1;
  lista_locais lptr = NULL;
  if(lista == NULL){
    return NULL;
  }
  do{
    swapped = 0;
    ptr1 = temp;
    while(ptr1 -> next != lptr){
      ordena_lista_alf_pdi(ptr1->p_interesse);
      if(strcmp(ptr1->cidade,ptr1->next->cidade)>0){
        troca(ptr1, ptr1->next);
        swapped = 1;
      }
      ptr1 = ptr1->next;
    }
    lptr = ptr1;
  }while(swapped);

  return lista;
}
lista_p_interesse ordena_lista_pop_pdi(lista_p_interesse lista){
  lista_p_interesse temp = lista;
  int swapped, i;
  lista_p_interesse ptr1;
  lista_p_interesse lptr = NULL;
  if(lista == NULL){
    return NULL;
  }
  do{
    swapped = 0;
    ptr1 = temp;
    while(ptr1 -> next != lptr){
      if(ptr1->popularidade < ptr1->next->popularidade){
        troca_pdis(ptr1, ptr1->next);
        swapped = 1;
      }
      ptr1 = ptr1->next;
    }
    lptr = ptr1;
  }while(swapped);

  return lista;
}
lista_p_interesse ordena_lista_alf_pdi(lista_p_interesse lista){
  lista_p_interesse temp = lista;
  int swapped, i;
  lista_p_interesse ptr1;
  lista_p_interesse lptr = NULL;
  if(lista == NULL){
    return NULL;
  }
  do{
    swapped = 0;
    ptr1 = temp;
    while(ptr1 -> next != lptr){
      if(strcmp(ptr1->nome,ptr1->next->nome)>0){
        troca_pdis(ptr1, ptr1->next);
        swapped = 1;
      }
      ptr1 = ptr1->next;
    }
    lptr = ptr1;
  }while(swapped);

  return lista;
}
/*Funções para trocar dois elementos da lista*/
void troca(lista_locais a, lista_locais b){
  int id = a->id;
  int popularidade = a -> popularidade;
  char cidade[MAX];
  strcpy(cidade, a->cidade);
  lista_p_interesse p_interesse = a->p_interesse;
  char pais[MAX];
  strcpy(pais, a->pais);

  a->id = b->id;
  a->popularidade = b->popularidade;
  strcpy(a->cidade, b->cidade);
  a->p_interesse = b->p_interesse;
  strcpy(a->pais, b->pais);

  b->id = id;
  b->popularidade = popularidade;
  strcpy(b->cidade, cidade);
  b->p_interesse = p_interesse;
  strcpy(b->pais, pais);


}
void troca_pdis(lista_p_interesse a, lista_p_interesse b){

  int id = a->id;
  int popularidade = a -> popularidade;
  char nome[MAX];
  strcpy(nome, a->nome);
  char descricao[MAX];
  strcpy(descricao, a->descricao);
  Hora entrada = a->entrada;
  Hora saida = a->saida;


  a->id = b->id;
  a->popularidade = b->popularidade;
  strcpy(a->nome, b->nome);
  strcpy(a->descricao, b->descricao);
  a->entrada = b->entrada;
  a->saida = b->saida;

  b->id = id;
  b->popularidade = popularidade;
  strcpy(b->nome, nome);
  strcpy(b->descricao, descricao);
  b->entrada = entrada;
  b->saida = saida;


}

/*Funções de apagar listas*/
lista_p_interesse apaga_lista(lista_p_interesse lista){
  lista_p_interesse temp;
  if(lista == NULL){
    return 0;
  }
  else{
    while(lista){
      temp = lista;
      lista = lista->next;
      free(temp);

    }
  }

  return lista;
}
lista_locais apaga_lista_locais(lista_locais lista){
  lista_locais temp;
  if(lista == NULL){
    return 0;
  }
  else{
    while(lista){
      temp = lista;
      apaga_lista(temp->p_interesse);
      lista = lista->next;
      free(temp);

    }
  }

  return lista;
}
lista_pessoas apaga_lista_pessoa(lista_pessoas lista){
  lista_pessoas temp;
  if(lista == NULL){
    return 0;
  }
  else{
    while(lista){
      temp = lista;
      lista = lista->next;
      free(temp);

    }
  }

  return lista;
}

/*Funções para aumentar a popularidade de um local ou pdi*/
lista_locais aumenta_pop(lista_locais lista, int id){

  lista_locais temp = lista;
  while(temp){
    if(temp->id == id){
      temp->popularidade += 1;

      break;
    }
    else{
      temp = temp->next;
    }
  }
  return lista;
}
lista_locais aumenta_pop_pi(lista_locais lista, int id_pi){
  lista_locais temp = lista;
  lista_p_interesse temp_interesse;
  while(temp){
    temp_interesse = temp->p_interesse;
    while(temp_interesse){
      if(temp_interesse->id == id_pi){
        temp_interesse->popularidade += 1;

        break;
      }
      else{
        temp_interesse = temp_interesse->next;
      }
    }
    temp = temp->next;

  }
  return lista;
}
lista_locais diminui_pop_pi(lista_locais lista, int id_pi){
  lista_locais temp = lista;
  lista_p_interesse temp_interesse;
  while(temp){
    temp_interesse = temp->p_interesse;
    while(temp_interesse){
      if(temp_interesse->id == id_pi){
        temp_interesse->popularidade -= 1;

        break;
      }
      else{
        temp_interesse = temp_interesse->next;
      }
    }
    temp = temp->next;

  }
  return lista;
}
lista_locais diminui_pop(lista_locais lista, int id){

  lista_locais temp = lista;
  while(temp){
    if(temp->id == id){
      temp->popularidade -= 1;

      break;
    }
    else{
      temp = temp->next;
    }
  }
  return lista;
}
/*Ver se um PDI está num local*/
int pdi_in_local(int id_pdi, lista_p_interesse lista_pdis){
  int conta = 0;
  struct lnodepinteresse* temp = lista_pdis;
  while(temp){


    if(temp->id==id_pdi){
      conta = 1;
      goto end;
    }
    else{
      temp = temp->next;
    }
  }


  end:
  return conta;
}

/*fazer a viagem final*/
void fazer_viagem(lista_pessoas lista_pessoa, int id, lista_locais lista_locais){
  struct lnodelocais* temp_locais;
  struct lnodelocais* temp2_locais = lista_locais; //Usada para dps colocar os Pdis que faltam.
  lista_pessoas temp_pessoas = lista_pessoa;
  struct lnodepinteresse* temp_pdis;
  struct lnodepinteresse* temp2_pdis; //Usada para os pdis que faltam
  char locais[MAX][MAX];
  char pdis[MAX][MAX];
  int n_local = 0, n_pdis = 0; //Variaveis que vao ser os indices do char acima
  int aux;
  int aux2 = 0; //Numero de PIs no local. Isto vai servir para p.e: Se no primeiro local, o user tiver escolhido menos que 3 locais, o resto vou preencher com pdis do local.
  int j, k, aux3 = 0, n = 0, i;

  /*variaveis para calcular a taxa de popularidade da viagem*/
  double taxa_pop;
  int total_users = 0;
  int local_igual = 0; //nº de users com locais iguais aos da viagem
  int hot_igual = 0; //Nº de users com hot incluido na viagem
  int total_pdis_users = 0; //Numero total de pdis nos users
  int pdis_users_na_viagem = 0; //Numero de pdis iguais aos da viagem
  double perc_pdi;
  double perc_local;
  double perc_hot;


  while(temp_pessoas){ //Encontar a pessoa
    if(temp_pessoas -> id == id){
      temp_locais = temp_pessoas->locais; //Locais escolhidos pela pessoa
      temp_pdis = temp_pessoas->p_interesse; //PIs escolhidos pela pessoa
      if(tamanho_lista_locais(temp_locais) < 3){ //Ver se escolhei 3 locais
        printf("\n\tAinda nao escolheu 3 lugares!\n");
        return;
      }
      break;
    }
    else{
      temp_pessoas = temp_pessoas ->next;
    }
  }

  while(temp_locais){ //Percorrer os locais da pessoa
    strcpy(locais[n_local], temp_locais->cidade);
    n_local++;
    while(temp_pdis){//Percorrer os pdis da pessoa e ver se estão dentro dos locais acima (local a local)
      aux = pdi_in_local(temp_pdis->id, temp_locais->p_interesse);
      if(aux==1){
        strcpy(pdis[n_pdis], temp_pdis->nome);

        n_pdis++;

        aux2++;
        if(aux2 >= 3){
          goto label;
        }

      }

      temp_pdis = temp_pdis -> next;


    }

    if(aux2<3){ //Se naquele local nao houver ainda 3 pdis (q foram escolhidos pelo user) vamos adicionar os que faltam da lista original de locais
      while(temp2_locais){

        if(temp2_locais->id==temp_locais->id){

          temp2_pdis = temp2_locais->p_interesse;
          while(temp2_pdis){
              for(j = 0; j < n_pdis; j++){
                if(strcmp(pdis[j], temp2_pdis->nome)==0){
                  aux3 = 1;

                }
              }
              if(aux3!=1){
                strcpy(pdis[n_pdis], temp2_pdis->nome);
                n_pdis++;

                aux2++;
              }

              if(aux2 == 3){
              goto label;
            }
            aux3 = 0;
            temp2_pdis = temp2_pdis -> next;
          }

        }
        else{

          temp2_locais = temp2_locais -> next;
        }
      }
    }
    label:
    aux3 = 0;
    aux2 = 0;
    temp_pdis = temp_pessoas -> p_interesse;
    temp_locais = temp_locais ->next;
  }

  temp_pessoas = lista_pessoa;

  while(temp_pessoas){ //Taxa de popularidade

      total_users++; //Total de users


      if(temp_pessoas->id != id){

        for(j = 0; j < n_pdis; j++){

          if(strcmp(temp_pessoas->hot, pdis[j]) == 0){ //Se o hot da pessoa tiver na viagem

            hot_igual++;

          }
        }
        temp_locais = temp_pessoas->locais;
        temp_pdis = temp_pessoas->p_interesse;
        while(temp_locais){

          for(i = 0; i < 3; i++){

            if(strcmp(locais[i], temp_locais->cidade)==0){ //Se o local tiver na viagem
              local_igual++;
              goto p;
            }
          }
          temp_locais = temp_locais -> next;
        }
        p:
        while(temp_pdis){

          total_pdis_users++; //Numero de pdis

          for(k = 0; k < n_pdis; k++){
            if(strcmp(pdis[k], temp_pdis->nome)==0){ //Se o PDI tiver na viagem


              pdis_users_na_viagem++;

            }
          }
          temp_pdis = temp_pdis -> next;
        }
      }

      temp_pessoas = temp_pessoas->next;

    }

  if(total_pdis_users == 0){
    perc_pdi = 0;
  }
  else{
    perc_pdi = pdis_users_na_viagem / (float)total_pdis_users;
  }



  if(total_users == 0){
    perc_local = 0;
  }
  else if(total_users == 1){
    perc_local = local_igual / ((float)total_users);
  }
  else{
    perc_local = local_igual / ((float)total_users-1);
  }

  if(total_users == 0){
    perc_hot = 0;
  }
  else if(total_users == 1){
    perc_hot = hot_igual /( (float) total_users);
  }
  else{
    perc_hot = hot_igual / ((float) total_users-1);
  }



  taxa_pop = (perc_pdi + perc_local + perc_hot) / 3;


  printf("\n\tA viagem do User [%d] vai ser constituida por:\n\n", id);

  for(int i = 0 ; i < 3; i++){

    printf("\tLocal %d: %s\t  |  ",i+1, locais[i]);
    printf("Pontos de Interesse: ");
    for(int j = n; j < n+3; j++){
      printf("%s - ", pdis[j]);

    }
    n+=3;
    printf("\n");
  }
  printf("\n\n\t%.2f%c dos Pontos de Interesse favoritos dos outros utilizadores estao nesta viagem!", perc_pdi*100, '%');
  printf("\n\t%.2f%c dos utilizadores tem como seu ponto Hot um ponto de interesse incluido nesta viagem!",perc_hot*100, '%');
  printf("\n\t%.2f%c dos utilizadores escolheram 1 ou mais locais desta viagem!", perc_local*100, '%');
  printf("\n\tA taxa de popularidade desta viagem e de: %.2f%c\n", taxa_pop*100, '%');


}

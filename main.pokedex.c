#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>   /* necessário para strcasecmp em modo estrito (-std=c11) */
#include <locale.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif


/* Remove '\r' e '\n' do final de uma string (usada em nomes/linhas lidas) */
void ajustar(char n[]) {
    n[strcspn(n, "\r\n")] = '\0';
}


/* Lê um inteiro de forma segura, sem travar em loop infinito se o
   usuário digitar algo que não seja número. */
int lerInteiro() {

    int valor;

    while (scanf("%d", &valor) != 1) {

        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        printf("Entrada inválida! Digite um número: ");
    }

    return valor;
}


void removerAcentos(char *str) {

    char buffer[256];
    int j = 0;

    for (int i = 0; str[i] != '\0' && j < (int)sizeof(buffer) - 2; i++) {

        unsigned char c = str[i];

        if ((c & 0xE0) == 0xC0 && str[i + 1]) {

            unsigned char c2 = str[i + 1];

            if ((c == 0xC3 && c2 >= 0x80 && c2 <= 0x87) ||
                (c == 0xC3 && c2 >= 0xA0 && c2 <= 0xA7)) {

                buffer[j++] = 'a';

            } else if ((c == 0xC3 && c2 >= 0x88 && c2 <= 0x8B) ||
                       (c == 0xC3 && c2 >= 0xA8 && c2 <= 0xAB)) {

                buffer[j++] = 'e';

            } else if ((c == 0xC3 && c2 >= 0x8C && c2 <= 0x8F) ||
                       (c == 0xC3 && c2 >= 0xAC && c2 <= 0xAF)) {

                buffer[j++] = 'i';

            } else if ((c == 0xC3 && c2 >= 0x92 && c2 <= 0x96) ||
                       (c == 0xC3 && c2 >= 0xB2 && c2 <= 0xB6)) {

                buffer[j++] = 'o';

            } else if ((c == 0xC3 && c2 >= 0x99 && c2 <= 0x9C) ||
                       (c == 0xC3 && c2 >= 0xB9 && c2 <= 0xBC)) {

                buffer[j++] = 'u';

            } else if ((c == 0xC3 && c2 == 0x87) ||
                       (c == 0xC3 && c2 == 0xA7)) {

                buffer[j++] = 'c';

            } else {

                buffer[j++] = c;
                if (j < (int)sizeof(buffer) - 1) buffer[j++] = c2;
            }

            i++;

        } else {

            buffer[j++] = c;
        }
    }

    buffer[j] = '\0';

    strcpy(str, buffer);
}


void pedirEnter() {

    printf("\nPressione ENTER para prosseguir.\n");

    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}



typedef struct {

    int entrada;
    int hp;
    int attack;
    int defense;
    int spatk;
    int spdef;
    int speed;

    char nome[50];
    char tipo1[20];
    char tipo2[20];

    int maxHP;

} Pokemon;



void mostrarPokemon(Pokemon p) {

    printf("-------------------------------------\n");

    printf("Entrada: %d\n", p.entrada);
    printf("Nome: %s\n", p.nome);
    printf("Tipo 1: %s\n", p.tipo1);
    printf("Tipo 2: %s\n", p.tipo2);

}



void mostrarTime(Pokemon equipe[], int tam, char *nomeTime) {

    printf("\n%s:\n", nomeTime);


    for(int i = 0; i < tam; i++) {


        if(strcmp(equipe[i].tipo2, "Nenhum") == 0) {


            printf("%d - %s (%s) HP: %d\n",
                   equipe[i].entrada,
                   equipe[i].nome,
                   equipe[i].tipo1,
                   equipe[i].hp);


        } else {


            printf("%d - %s (%s / %s) HP: %d\n",
                   equipe[i].entrada,
                   equipe[i].nome,
                   equipe[i].tipo1,
                   equipe[i].tipo2,
                   equipe[i].hp);

        }
    }
}



int pokemonVivo(Pokemon equipe[], int tam) {


    for(int i = 0; i < tam; i++) {

        if(equipe[i].hp > 0) {

            return 1;
        }
    }

    return 0;
}



int escolherProximoPokemon(Pokemon equipe[], int tam) {


    for(int i = 0; i < tam; i++) {

        if(equipe[i].hp > 0) {

            return i;
        }
    }

    return -1;
}



int calcularDano(Pokemon atacante, Pokemon defensor) {


    int ataque = atacante.attack;

    int defesa = defensor.defense;

    if (defesa <= 0) defesa = 1;   /* evita divisão por zero */


    float stab = 1.5;

    float aleatorio = 0.85 + ((rand() % 16) / 100.0);



    int dano = (int)(((float)ataque * 25 / defesa)
                     * stab
                     * aleatorio);

    if (dano < 1) dano = 1;   /* todo ataque causa ao menos 1 de dano */


    return dano;
}
void atacar(Pokemon *atacante, Pokemon *defensor) {

    int dano = calcularDano(*atacante, *defensor);

    printf("-------------------------------------\n");
    printf("O ataque de %s deu %d de dano!\n",
           atacante->nome, dano);


    defensor->hp -= dano;


    if(defensor->hp < 0) {
        defensor->hp = 0;
    }


    printf("-------------------------------------\n");
    printf("%s HP: %d\n", atacante->nome, atacante->hp);
    printf("%s HP: %d\n", defensor->nome, defensor->hp);


    if(defensor->hp == 0) {

        printf("-------------------------------------\n");
        printf("%s morreu!\n", defensor->nome);
    }

}



void curar(Pokemon *p, int *pocoes) {


    int cura = 30;


    if(*pocoes > 0) {


        (*pocoes)--;


        p->hp += cura;


        if(p->hp > p->maxHP) {

            p->hp = p->maxHP;
        }


        printf("-------------------------------------\n");
        printf("%s usou uma poção!\n", p->nome);
        printf("HP atual: %d | Poções restantes: %d\n",
               p->hp,
               *pocoes);


    } else {


        printf("-------------------------------------\n");
        printf("Você não possui mais poções!\n");

    }

}



void trocar(Pokemon equipe[], int tam, int *ativo) {


    int possuiPokemon = 0;


    for(int i = 0; i < tam; i++) {

        if(equipe[i].hp > 0) {

            possuiPokemon = 1;
            break;
        }
    }



    if(!possuiPokemon) {

        printf("Nenhum Pokémon disponível!\n");
        return;
    }



    printf("-------------------------------------\n");
    printf("Escolha um Pokémon:\n");


    for(int i = 0; i < tam; i++) {


        printf("%d - %s HP: %d\n",
               i,
               equipe[i].nome,
               equipe[i].hp);

    }



    int escolha;


    while(1) {


        printf("Escolha: ");
        escolha = lerInteiro();



        if(escolha >= 0 &&
           escolha < tam &&
           equipe[escolha].hp > 0) {


            *ativo = escolha;


            printf("-------------------------------------\n");
            printf("%s entrou em campo!\n",
                   equipe[*ativo].nome);


            break;


        } else {


            printf("Escolha inválida!\n");

        }
    }

}




void turno(Pokemon timeAtacante[],
           int tamAtacante,
           Pokemon timeDefensor[],
           int tamDefensor,
           int *pocoes,
           int *pocoesDefensor) {


    int ativoAtacante = 0;
    int ativoDefensor = 0;

    int escolha;



    while(1) {


        printf("\n------------- STATUS --------------\n");


        mostrarTime(timeAtacante,
                    tamAtacante,
                    "Seu time");


        mostrarTime(timeDefensor,
                    tamDefensor,
                    "Time adversário");



        printf("-------------------------------------\n");

        printf("Seu Pokémon: %s HP:%d\n",
               timeAtacante[ativoAtacante].nome,
               timeAtacante[ativoAtacante].hp);



        printf("\n1 - Atacar");
        printf("\n2 - Usar poção");
        printf("\n3 - Trocar Pokémon");
        printf("\nEscolha: ");

        escolha = lerInteiro();



        if(escolha == 1) {


            atacar(&timeAtacante[ativoAtacante],
                   &timeDefensor[ativoDefensor]);



            if(timeDefensor[ativoDefensor].hp == 0) {


                int prox = escolherProximoPokemon(
                    timeDefensor,
                    tamDefensor);



                if(prox != -1) {


                    ativoDefensor = prox;


                    printf("-------------------------------------\n");
                    printf("%s entrou em campo!\n",
                           timeDefensor[ativoDefensor].nome);


                }

            }



        } else if(escolha == 2) {


            curar(&timeAtacante[ativoAtacante],
                  pocoes);



        } else if(escolha == 3) {


            trocar(timeAtacante,
                   tamAtacante,
                   &ativoAtacante);


        } else {


            printf("Opção inválida!\n");
            continue;

        }



        if(!pokemonVivo(timeDefensor,tamDefensor)) {


            printf("-------------------------------------\n");
            printf("Você venceu a batalha!\n");
            break;

        }




        int acaoCPU = rand()%10;



        if(acaoCPU < 9) {


            pedirEnter();


            atacar(&timeDefensor[ativoDefensor],
                   &timeAtacante[ativoAtacante]);



        } else {


            pedirEnter();


            curar(&timeDefensor[ativoDefensor],
                  pocoesDefensor);

        }



        if(timeAtacante[ativoAtacante].hp <= 0) {


            printf("-------------------------------------\n");
            printf("%s desmaiou!\n",
                   timeAtacante[ativoAtacante].nome);



            if(pokemonVivo(timeAtacante,tamAtacante)) {


                trocar(timeAtacante,
                       tamAtacante,
                       &ativoAtacante);

            }

        }



        if(!pokemonVivo(timeAtacante,tamAtacante)) {


            printf("-------------------------------------\n");
            printf("Você perdeu a batalha!\n");

            break;
        }

    }


    printf("-------------------------------------\n");
    printf("Fim da batalha!\n");

}
void swap(Pokemon *a, Pokemon *b) {

    Pokemon temp = *a;
    *a = *b;
    *b = temp;

}



int partitionNumero(Pokemon lista[], int low, int high) {

    int pivot = lista[high].entrada;

    int i = low - 1;


    for(int j = low; j < high; j++) {

        if(lista[j].entrada < pivot) {

            i++;

            swap(&lista[i], &lista[j]);

        }

    }


    swap(&lista[i+1], &lista[high]);


    return i + 1;

}



void quicksortNumero(Pokemon lista[], int low, int high) {


    if(low < high) {


        int pi = partitionNumero(lista, low, high);


        quicksortNumero(lista, low, pi - 1);

        quicksortNumero(lista, pi + 1, high);

    }

}



int partitionNome(Pokemon lista[], int low, int high) {


    char pivot[50];


    strcpy(pivot, lista[high].nome);


    int i = low - 1;



    for(int j = low; j < high; j++) {


        if(strcasecmp(lista[j].nome, pivot) < 0) {


            i++;

            swap(&lista[i], &lista[j]);

        }

    }



    swap(&lista[i+1], &lista[high]);


    return i+1;

}




void quicksortNome(Pokemon lista[], int low, int high) {


    if(low < high) {


        int pi = partitionNome(lista, low, high);


        quicksortNome(lista, low, pi-1);

        quicksortNome(lista, pi+1, high);

    }

}




void pesquisarPorNome(Pokemon lista[], int qtd) {


    char nome[50];


    int c;
    while ((c = getchar()) != '\n' && c != EOF);


    printf("Digite o nome do Pokémon: ");

    if (fgets(nome, sizeof(nome), stdin) == NULL) {
        return;
    }


    ajustar(nome);

    removerAcentos(nome);



    int encontrado = 0;



    for(int i = 0; i < qtd; i++) {


        char nomePokemon[50];


        strcpy(nomePokemon, lista[i].nome);


        removerAcentos(nomePokemon);



        if(strcasecmp(nome,nomePokemon)==0) {


            mostrarPokemon(lista[i]);

            encontrado = 1;

        }

    }



    if(!encontrado) {

        printf("Pokémon não encontrado!\n");

    }

}




void pesquisarNumero(Pokemon lista[], int qtd) {


    int numero;


    printf("Digite a entrada: ");

    numero = lerInteiro();



    for(int i = 0; i < qtd; i++) {


        if(lista[i].entrada == numero) {


            mostrarPokemon(lista[i]);

            return;

        }

    }


    printf("Pokémon não encontrado!\n");

}




void pesquisarTipo(Pokemon lista[], int qtd) {


    char tipo[20];


    int c;
    while ((c = getchar()) != '\n' && c != EOF);


    printf("Digite o tipo: ");

    if (fgets(tipo, sizeof(tipo), stdin) == NULL) {
        return;
    }


    ajustar(tipo);

    removerAcentos(tipo);



    int encontrado = 0;



    for(int i = 0; i < qtd; i++) {


        char t1[20];

        char t2[20];


        strcpy(t1,lista[i].tipo1);

        strcpy(t2,lista[i].tipo2);



        removerAcentos(t1);

        removerAcentos(t2);



        if(strcasecmp(tipo,t1)==0 ||
           strcasecmp(tipo,t2)==0) {


            mostrarPokemon(lista[i]);

            encontrado = 1;

        }

    }



    if(!encontrado) {

        printf("Nenhum Pokémon encontrado!\n");

    }

}





void menuPesquisa(Pokemon lista[], int qtd) {


    int opc;


    printf("\n1 - Nome");

    printf("\n2 - Número");

    printf("\n3 - Tipo");

    printf("\nEscolha: ");

    opc = lerInteiro();



    if(opc==1) {


        pesquisarPorNome(lista,qtd);


    } else if(opc==2) {


        pesquisarNumero(lista,qtd);


    } else if(opc==3) {


        pesquisarTipo(lista,qtd);


    } else {

        printf("Opção inválida!\n");
    }

}





void opcoesExtras(Pokemon lista[], int qtd) {


    int opc;


    printf("\n1 - Ordenar por número");

    printf("\n2 - Ordenar por nome");

    printf("\nEscolha: ");

    opc = lerInteiro();



    if(opc==1) {


        quicksortNumero(lista,0,qtd-1);


    } else if(opc==2) {


        quicksortNome(lista,0,qtd-1);

    } else {

        printf("Opção inválida!\n");
        return;
    }



    mostrarTime(lista,qtd,"Pokédex");

}



/* Deixa o jogador escolher, pela Entrada (número da Pokédex),
   os Pokémon que vão compor o seu time de batalha. */
void escolherTime(Pokemon pokedex[], int qtd, Pokemon equipe[], int tamEquipe) {

    printf("\nMonte seu time (%d Pokémon). Escolha pela Entrada:\n", tamEquipe);

    mostrarTime(pokedex, qtd, "Pokédex disponível");

    for (int i = 0; i < tamEquipe; i++) {

        int idx = -1;

        while (idx == -1) {

            printf("\nPokémon %d de %d - Entrada: ", i + 1, tamEquipe);

            int entrada = lerInteiro();

            for (int j = 0; j < qtd; j++) {

                if (pokedex[j].entrada == entrada) {
                    idx = j;
                    break;
                }
            }

            if (idx == -1) {
                printf("Entrada inválida!\n");
            }
        }

        equipe[i] = pokedex[idx];
        equipe[i].hp = equipe[i].maxHP;   /* garante time cheio no início da batalha */
    }
}


/* Monta o time da CPU sorteando Pokémon da Pokédex. */
void montarTimeCPU(Pokemon pokedex[], int qtd, Pokemon equipe[], int tamEquipe) {

    for (int i = 0; i < tamEquipe; i++) {

        int idx = rand() % qtd;

        equipe[i] = pokedex[idx];
        equipe[i].hp = equipe[i].maxHP;
    }
}


/* Prepara os dois times e inicia a batalha. */
void iniciarBatalha(Pokemon pokedex[], int qtd) {

    int tamEquipe = 3;

    if (qtd < tamEquipe) {
        tamEquipe = qtd;
    }

    Pokemon *timeJogador = malloc(sizeof(Pokemon) * tamEquipe);
    Pokemon *timeCPU = malloc(sizeof(Pokemon) * tamEquipe);

    if (timeJogador == NULL || timeCPU == NULL) {
        printf("Erro ao alocar memória para a batalha!\n");
        free(timeJogador);
        free(timeCPU);
        return;
    }

    escolherTime(pokedex, qtd, timeJogador, tamEquipe);
    montarTimeCPU(pokedex, qtd, timeCPU, tamEquipe);

    int pocoesJogador = 3;
    int pocoesCPU = 3;

    printf("\nA batalha vai começar! Você tem %d poções.\n", pocoesJogador);

    turno(timeJogador, tamEquipe, timeCPU, tamEquipe, &pocoesJogador, &pocoesCPU);

    free(timeJogador);
    free(timeCPU);
}




int carregarPokemons(Pokemon **lista, const char *arquivoNome) {


    FILE *arquivo = fopen(arquivoNome,"r");



    if(arquivo == NULL) {


        printf("Erro ao abrir Pokemon.txt\n");

        return 0;

    }



    int capacidade = 50;

    int quantidade = 0;



    *lista = malloc(sizeof(Pokemon)*capacidade);

    if (*lista == NULL) {
        printf("Erro ao alocar memória para a Pokédex!\n");
        fclose(arquivo);
        return 0;
    }



    char linha[300];



    while(fgets(linha,sizeof(linha),arquivo)) {


        /* remove '\r'/'\n' do fim da linha ANTES de tokenizar; isso evita
           que o último campo (tipo1, quando não há tipo2) fique com uma
           quebra de linha grudada no valor. */
        ajustar(linha);

        if (linha[0] == '\0') {
            continue;   /* ignora linhas em branco */
        }


        if(quantidade >= capacidade) {


            capacidade *= 2;


            Pokemon *novaLista = realloc(*lista,
                    sizeof(Pokemon)*capacidade);

            if (novaLista == NULL) {
                printf("Erro ao realocar memória!\n");
                fclose(arquivo);
                return quantidade;
            }

            *lista = novaLista;

        }



        Pokemon *p = &(*lista)[quantidade];



        char *token;
        int linhaValida = 1;



        token = strtok(linha,";");
        if (!token) linhaValida = 0;
        else p->entrada = atoi(token);



        if (linhaValida) {
            token = strtok(NULL,";");
            if (!token) linhaValida = 0;
            else {
                strncpy(p->nome, token, sizeof(p->nome) - 1);
                p->nome[sizeof(p->nome) - 1] = '\0';
            }
        }



        if (linhaValida) {
            token = strtok(NULL,";");
            if (!token) linhaValida = 0;
            else {
                p->hp = atoi(token);
                p->maxHP = p->hp;
            }
        }



        if (linhaValida) {
            token = strtok(NULL,";");
            if (!token) linhaValida = 0;
            else p->attack = atoi(token);
        }



        if (linhaValida) {
            token = strtok(NULL,";");
            if (!token) linhaValida = 0;
            else p->defense = atoi(token);
        }



        if (linhaValida) {
            token = strtok(NULL,";");
            if (!token) linhaValida = 0;
            else p->spatk = atoi(token);
        }



        if (linhaValida) {
            token = strtok(NULL,";");
            if (!token) linhaValida = 0;
            else p->spdef = atoi(token);
        }



        if (linhaValida) {
            token = strtok(NULL,";");
            if (!token) linhaValida = 0;
            else p->speed = atoi(token);
        }



        if (linhaValida) {
            token = strtok(NULL,";");
            if (!token) linhaValida = 0;
            else {
                strncpy(p->tipo1, token, sizeof(p->tipo1) - 1);
                p->tipo1[sizeof(p->tipo1) - 1] = '\0';
            }
        }



        if (!linhaValida) {

            printf("Aviso: linha inválida ignorada na Pokedex (entrada %d).\n",
                   quantidade + 1);
            continue;
        }


        token = strtok(NULL,";");

        if(token) {
            strncpy(p->tipo2, token, sizeof(p->tipo2) - 1);
            p->tipo2[sizeof(p->tipo2) - 1] = '\0';
        } else {
            strcpy(p->tipo2,"Nenhum");
        }



        quantidade++;

    }



    fclose(arquivo);



    return quantidade;

}





int main() {


    setlocale(LC_ALL,"");


    srand(time(NULL));



    Pokemon *pokedex = NULL;



    int quantidade = carregarPokemons(
        &pokedex,
        "Pokemon.txt"
    );



    if(quantidade == 0) {


        printf("Nenhum Pokémon carregado.\n");

        return 1;

    }




    int opc;



    while(1) {


        printf("\n============================\n");

        printf(" BATALHA E PESQUISA POKEMON\n");

        printf("============================\n");



        printf("Pokémons carregados: %d\n",quantidade);



        printf("\n1 - Pesquisar");

        printf("\n2 - Ordenar Pokédex");

        printf("\n3 - Batalhar");

        printf("\n4 - Sair");

        printf("\nEscolha: ");



        opc = lerInteiro();



        if(opc==1) {


            menuPesquisa(pokedex,quantidade);


        } else if(opc==2) {


            opcoesExtras(pokedex,quantidade);



        } else if(opc==3) {


            iniciarBatalha(pokedex, quantidade);


        } else if(opc==4) {


            break;


        } else {


            printf("Opção inválida!\n");

        }

    }



    free(pokedex);



    return 0;

}

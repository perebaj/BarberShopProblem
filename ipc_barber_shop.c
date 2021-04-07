#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define False 0
#define True 1

int numeroCadeiras = 5;
int clientesAtuais = 0;
int clientesIniciais = 25;
int tempoDeCorte = 0;


void *cliente();
void corteDeCabelo();
void balk(int);
void *barbeiro();
void cortandoCabelo();

//semaforos
sem_t sem_cliente;     //Cliente querendo cortar cabelo
sem_t sem_mutex;    //Controla a chegada de clientes na barbearia
sem_t sem_barbeiro;   //Define se o barbeiro está ocupado ou n
sem_t sem_fimCliente; //Cliente terminou de cortar o cabelo
sem_t sem_fimBarbeiro; //Barbeiro terminou de cortar cabelo

int success = 0;

int main()
{
    pthread_t barber_id;
    pthread_t cliente_id[clientesIniciais];
    int cliente_num[clientesIniciais];

    sem_init(&sem_cliente, 0, 0);
    sem_init(&sem_mutex, 0, 1);
    sem_init(&sem_barbeiro, 0, 0);
    sem_init(&sem_fimCliente, 0, 0);
    sem_init(&sem_fimBarbeiro, 0, 0);

    //Iniciando o thread barbeiro
    pthread_create(&barber_id, NULL, *barbeiro, NULL);

    //Iniciando todos threads de consumidores
    for (int i = 0; i < clientesIniciais; i++)
    {
        // pthread_create(&cliente_id[iterator], NULL, cliente, (void *)&cliente_num[iterator]);
        printf("\t\tCliente %d chegou!\n", i + 1);
        pthread_create(&cliente_id[i], NULL, cliente, (void *) &i);
        int rNum = rand() % 2 + 1;

        sleep(rNum);
    }

    printf("\nFim dos clientes.\n");
    sleep(25);

    for (int i = 0; i < clientesIniciais; i++)
        pthread_join(cliente_id[i], NULL);
    //pthread_join(barber_id, NULL);

    sem_destroy(&sem_cliente);
    sem_destroy(&sem_mutex);
    sem_destroy(&sem_barbeiro);
    sem_destroy(&sem_fimCliente);
    sem_destroy(&sem_fimBarbeiro);

    return 0;
}

void *barbeiro() {
    while(1) {
        sem_wait(&sem_cliente);
        sem_post(&sem_barbeiro);

        printf("\t\t\t\tBarbeiro chama o cliente para o corte\n");
        cortandoCabelo();
        printf("\t\t\t\t\t\tBarbeiro está terminando o corte do cliente\n");
        success++;

        sem_post(&sem_fimBarbeiro);
        sem_wait(&sem_fimCliente);
    }
}

void *cliente(void * n) {
    int num = *(int *)n;
    sem_wait(&sem_mutex);
    if (clientesAtuais == numeroCadeiras) {
        //Se nao ha cadeiras disponiveis, sai do salao
        sem_post(&sem_mutex);
        balk(num);
        return NULL;
    }
    printf("\t\tCliente %d acha uma cadeira livre\n", num + 1);
    clientesAtuais++;
    printf("\t\tCliente %d está esperando\n", num + 1);
    sem_post(&sem_mutex);

    sem_post(&sem_cliente);
    sem_wait(&sem_barbeiro);

        printf("\t\t\t\tCliente %d está cortando o cabelo\n", num + 1);
        tempoDeCorte = rand() % 5 + 1;
        corteDeCabelo();
        printf("\t\t\t\t\t\tCliente %d terminou de cortar o cabelo\n", num + 1);

    sem_post(&sem_fimCliente);
    sem_wait(&sem_fimBarbeiro);

    sem_wait(&sem_mutex);
    clientesAtuais--;
    sem_post(&sem_mutex);

    return NULL;
}

void balk(int num) {
    printf("Cliente %d saiu sem cortar o cabelo\n", num + 1);
    return;
}

void cortandoCabelo() {
    sleep(tempoDeCorte);
}

void corteDeCabelo() {    
    sleep(tempoDeCorte);
}

// int cortandoCabelo()
// // fala se o cabeleireiro esta cortando cabelo ou n
// // Return:
// //     {False} se o número de consumidores é igual a zero. Barbeiro nao está
// //     cortando cabelo.
// //     {True} se o número de consimidores é diferente de zero. Barbeiro
// //     está cortando cabelo
// {

//     if (clientes_inside_barbershop == 0)
//     {
//         rand() % numeroCadeiras * 2; //numero randomico incial de clientes

//         return False;
//     }
//     return True;
// }

// int getHairCut()
// {
//     if (cortandoCabelo())
//     {
//         if (get_avaliable_chair() > 0 && get_avaliable_chair() < numeroCadeiras)
//         {

//             //sentar e esperar
//         }
//         else
//         {
//             //vazar
//             // balk()
//         }
//     }
//     else
//     {
//         //sentar e cortar
//         pthread_mutex_lock(&lock);
//         sleep(5);
//         printf("\n Cortou o cabelo!!!\n");
//         clientes_inside_barbershop -= 1;
//         pthread_mutex_unlock(&lock);
//     }
// }

// int get_avaliable_chair()
// {
//     // retorna o número de caideiras disponíveis
//     return 0
// }

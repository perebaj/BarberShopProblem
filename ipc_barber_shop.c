#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define False 0
#define True 1

int _NUMBER_CHAIRS = 5;
int current_customers = 0;
int initial_customers = 25;
int hair_cut_time = 0;


void *customer();
void get_hair_cut();
void balk(int);
void *barber();
void cut_hair();

//semaforos
sem_t sem_cust;     //Cliente querendo cortar cabelo
sem_t sem_mutex;    //Controla a chegada de clientes na barbearia
sem_t sem_barber;   //Define se o barbeiro está ocupado ou n
sem_t sem_custDone; //Cliente terminou de cortar o cabelo
sem_t sem_barbDone; //Barbeiro terminou de cortar cabelo

int success = 0;

int main()
{
    pthread_t barber_id;
    pthread_t customer_id[initial_customers];
    int customer_num[initial_customers];

    sem_init(&sem_cust, 0, 0);
    sem_init(&sem_mutex, 0, 1);
    sem_init(&sem_barber, 0, 0);
    sem_init(&sem_custDone, 0, 0);
    sem_init(&sem_barbDone, 0, 0);

    //Iniciando o thread barbeiro
    pthread_create(&barber_id, NULL, *barber, NULL);

    //Iniciando todos threads de consumidores
    for (int iterator = 0; iterator < initial_customers; iterator++)
    {
        // pthread_create(&customer_id[iterator], NULL, customer, (void *)&customer_num[iterator]);
        printf("\t\tCustomer %d arrives!\n", iterator);
        pthread_create(&customer_id[iterator], NULL, customer, (void *) &iterator);
        int rNum = rand() % 2 + 1;

        sleep(rNum);
    }

    printf("\nOffice hour ended. Doors Closed.\n");
    sleep(25);
    printf("\nClosing Barbershop\n");
    printf("Total gained today: $%d\n", success*5);

    for (int iterator = 0; iterator < initial_customers; iterator++)
        pthread_join(customer_id[iterator], NULL);
    //pthread_join(barber_id, NULL);

    sem_destroy(&sem_cust);
    sem_destroy(&sem_mutex);
    sem_destroy(&sem_barber);
    sem_destroy(&sem_custDone);
    sem_destroy(&sem_barbDone);

    return 0;
}

void *barber() {
    while(1) {
        sem_wait(&sem_cust);
        sem_post(&sem_barber);

        printf("\t\t\t\tBarber is preparing to cut\n");
        cut_hair();
        printf("\t\t\t\t\t\tBarber Finished Cutting\n");
        success++;

        sem_post(&sem_barbDone);
        sem_wait(&sem_custDone);
    }
}

void *customer(void * n) {
    int num = *(int *)n;
    sem_wait(&sem_mutex);
    if (current_customers == _NUMBER_CHAIRS) {
        //Se nao ha cadeiras disponiveis, sai do salao
        sem_post(&sem_mutex);
        balk(num);
        return NULL;
    }
    
    current_customers++;
    printf("\t\tCustomer %d is waiting\n", num);
    sem_post(&sem_mutex);

    sem_post(&sem_cust);
    sem_wait(&sem_barber);

        printf("\t\t\t\tCustomer %d is getting haircut\n", num);
        hair_cut_time = rand() % 5 + 1;
        get_hair_cut();
        printf("\t\t\t\t\t\tCustomer %d finished cutting\n", num);

    sem_post(&sem_custDone);
    sem_wait(&sem_barbDone);

    sem_wait(&sem_mutex);
    current_customers--;
    sem_post(&sem_mutex);

    return NULL;
}

void balk(int num) {
    printf("Customer %d exited without cutting\n", num);
    return;
}

void cut_hair() {
    sleep(hair_cut_time);
}

void get_hair_cut() {    
    sleep(hair_cut_time);
}

// int cut_hair()
// // fala se o cabeleireiro esta cortando cabelo ou n
// // Return:
// //     {False} se o número de consumidores é igual a zero. Barbeiro nao está
// //     cortando cabelo.
// //     {True} se o número de consimidores é diferente de zero. Barbeiro
// //     está cortando cabelo
// {

//     if (customers_inside_barbershop == 0)
//     {
//         rand() % _NUMBER_CHAIRS * 2; //numero randomico incial de clientes

//         return False;
//     }
//     return True;
// }

// int getHairCut()
// {
//     if (cut_hair())
//     {
//         if (get_avaliable_chair() > 0 && get_avaliable_chair() < _NUMBER_CHAIRS)
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
//         customers_inside_barbershop -= 1;
//         pthread_mutex_unlock(&lock);
//     }
// }

// int get_avaliable_chair()
// {
//     // retorna o número de caideiras disponíveis
//     return 0
// }

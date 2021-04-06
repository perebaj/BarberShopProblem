#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define False 0
#define True 1

int _NUMBER_CHAIRS = 100;
int current_customers = 0;
int initial_customers = 150;
int hair_cut_time = 0;

//semaforos
sem_t sem_cust;     // cliente querendo cortar cabelo
sem_t sem_mutex;    // controla a qnt de clientes na barbearia
sem_t sem_barber;   //Define se o barbeiro está dormindo ou n
sem_t sem_custDone; //cliente terminou de cortar o cabelo
sem_t sem_barbDone; //Barbeiro terminou de cortar cabelo

int main()
{
    pthread_t barber_id;
    pthread_t customer_id[initial_customers];

    sem_init(&sem_cust, 0, 0);
    sem_init(&sem_mutex, 0, 1);
    sem_init(&sem_barber, 0, 0);
    sem_init(&sem_custDone, 0, 0);
    sem_init(&sem_barbDone, 0, 0);

    pthread_create(&barber_id, NULL, *barber, NULL);

    //Iniciando todos threads de consumidores
    for (int iterator = 0; iterator < initial_customers; iterator++)
    {
        pthread_create(&customer_id[iterator], NULL, customer, (void *)&custsArr[iterator]);
        int rNum = rand() % 5 + 1;

        sleep(rNum);
    }

    return 0
}

int *barber()
{
    return 0;
}

int cut_hair()
// fala se o cabeleireiro esta cortando cabelo ou n
// Return:
//     {False} se o número de consumidores é igual a zero. Barbeiro nao está
//     cortando cabelo.
//     {True} se o número de consimidores é diferente de zero. Barbeiro
//     está cortando cabelo
{

    if (customers_inside_barbershop == 0)
    {
        rand() % _NUMBER_CHAIRS * 2; //numero randomico incial de clientes

        return False;
    }
    return True;
}

int getHairCut()
{
    if (cut_hair())
    {
        if (get_avaliable_chair() > 0 && get_avaliable_chair() < _NUMBER_CHAIRS)
        {

            //sentar e esperar
        }
        else
        {
            //vazar
            // balk()
        }
    }
    else
    {
        //sentar e cortar
        pthread_mutex_lock(&lock);
        sleep(5);
        printf("\n Cortou o cabelo!!!\n");
        customers_inside_barbershop -= 1;
        pthread_mutex_unlock(&lock);
    }
}

int get_avaliable_chair()
{
    // retorna o número de caideiras disponíveis
    return 0
}
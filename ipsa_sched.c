#include <stdio.h>
#include <pthread.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/* Local includes. dans posix_gcc */
#include "console.h"

/* Priorities at which the tasks are created. */
#define task1priority					  ( tskIDLE_PRIORITY + 5 ) 
#define task2priority					  ( tskIDLE_PRIORITY + 4 )
#define task3priority					  ( tskIDLE_PRIORITY + 3 )
#define task4priority					  ( tskIDLE_PRIORITY + 2 )
#define task5priority					  ( tskIDLE_PRIORITY + 1 )

/* The rate at which data is sent to the queue.  The times are converted from
 * milliseconds to ticks using the pdMS_TO_TICKS() macro. */
#define mainTASK_SEND_FREQUENCY_MS         pdMS_TO_TICKS( 200UL )
#define mainTIMER_SEND_FREQUENCY_MS        pdMS_TO_TICKS( 2000UL )
						
/* The values sent to the queue receive task from the queue send task and the
 * queue send software timer respectively. */
#define mainVALUE_SENT_FROM_TASK           ( 100UL )
#define mainVALUE_SENT_FROM_TIMER          ( 200UL )
/*
 * The tasks as described in the comments at the top of this file.
 */

static void task1(void * pvParameters);
static void task2(void * pvParameters);
static void task3(void * pvParameters);
static void task4(void * pvParameters);
static void task5(void * pvParameters);

/* A software timer that is started from the tick hook. */
//static TimerHandle_t xTimer = NULL;
void ipsa_sched(void)
{
xTaskCreate( task1, "Rx", configMINIMAL_STACK_SIZE, NULL, task1priority, NULL );                          
xTaskCreate( task2, "TX", configMINIMAL_STACK_SIZE, NULL, task2priority, NULL );
xTaskCreate( task3, "FX", configMINIMAL_STACK_SIZE, NULL, task3priority, NULL );
xTaskCreate( task4, "ZX", configMINIMAL_STACK_SIZE, NULL, task4priority, NULL );
xTaskCreate( task5, "GX", configMINIMAL_STACK_SIZE, NULL, task4priority, NULL );

/* Create the software timer, but don't start it yet. */
//xTimer = xTimerCreate( "Timer",xTimerPeriod,pdTRUE,NULL ); 

/*if( xTimer != NULL )
{
	xTimerStart( xTimer, 0 );
};*/

/* Start the tasks and timer running. */
vTaskStartScheduler();


/* If all is well, the scheduler will now be running, and the following
 * line will never be reached.  If the following line does execute, then
 * there was insufficient FreeRTOS heap memory available for the idle and/or
 * timer tasks	to be created.  See the memory management section on the
 * FreeRTOS web site for more details. */
for( ; ; )
{
}
};



static void task1(void * pvParameters){
	TickType_t xNextWakeTime;
    const TickType_t xBlockTime = mainTASK_SEND_FREQUENCY_MS;
    const uint32_t ulValueToSend = mainVALUE_SENT_FROM_TASK;
	xNextWakeTime = xTaskGetTickCount();
	for( ; ; )
    {
	console_print("Everything is working \n");
	vTaskDelayUntil( &xNextWakeTime, xBlockTime );
	}
};

static void task2(void * pvParameters){
	TickType_t xNextWakeTime;
    const TickType_t xBlockTime = mainTASK_SEND_FREQUENCY_MS;
    const uint32_t ulValueToSend = mainVALUE_SENT_FROM_TASK;
	xNextWakeTime = xTaskGetTickCount();
	for( ; ; )
    {
	int F=100;
	float C;
	C=(F-32)/1.8;
	console_print("%f \n",C);
	vTaskDelayUntil( &xNextWakeTime, xBlockTime );
	}
	
	
};

static void task3(void * pvParameters){
	TickType_t xNextWakeTime;
    const TickType_t xBlockTime = mainTASK_SEND_FREQUENCY_MS;
    const uint32_t ulValueToSend = mainVALUE_SENT_FROM_TASK;
	xNextWakeTime = xTaskGetTickCount();
	for( ; ; )
    {
	long int a=20355432453,b=25432543;
	long int result;
	result=a*b;
	console_print("%ld \n",result);
	vTaskDelayUntil( &xNextWakeTime, xBlockTime );
	}
};



static void task4(void * pvParameters){
	TickType_t xNextWakeTime;
    const TickType_t xBlockTime = mainTASK_SEND_FREQUENCY_MS;
    const uint32_t ulValueToSend = mainVALUE_SENT_FROM_TASK;
	xNextWakeTime = xTaskGetTickCount();

	for( ; ; )
    {
	int tab[50]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49};
	int low = 0;
	int high = 49;
	int key = 2;
	int mid = (low+high)/2;
	
	while(tab[mid]!=key){
		if(tab[mid]>key){
			high = mid-1;
			mid = (low+high)/2;   
		}
		else if(tab[mid]<key){
			low = mid+1;
			mid = (low+high)/2;
		}
		else{
			console_print("Le chiffre n'est pas dans le tableau \n");
		}
	}
	console_print("le chiffre %d est à l'index : %d \n",key, mid);
	vTaskDelayUntil( &xNextWakeTime, xBlockTime );
	}
};

// Fonction qui permet de vérifier si une touche a été pressée sans bloquer l'exécution
int hasKeyBeenPressed(void) {
    struct termios oldConfig, newConfig;
    int inputChar;
    int previousFileFlags;

    // Sauvegarde de l'état actuel du terminal
    tcgetattr(STDIN_FILENO, &oldConfig);

    // Modification des paramètres pour désactiver le mode canonique et l'écho des touches
    newConfig = oldConfig;
    newConfig.c_lflag &= ~(ICANON | ECHO);  // Désactive l'attente de retour à la ligne et l'écho
    tcsetattr(STDIN_FILENO, TCSANOW, &newConfig);  // Applique la nouvelle configuration au terminal

    // Passage en mode non-bloquant
    previousFileFlags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, previousFileFlags | O_NONBLOCK);

    // Tentative de lecture d'une touche sans suspendre l'exécution
    inputChar = getchar();

    // Restauration de la configuration d'origine du terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &oldConfig);
    fcntl(STDIN_FILENO, F_SETFL, previousFileFlags);  // Restaure les anciens paramètres de fichier

    // Si une touche a été lue, retourne 1 (vrai)
    if (inputChar != EOF) {
        ungetc(inputChar, stdin);  // Remet la touche dans le buffer pour une utilisation ultérieure
        return 1;
    }

    // Si aucune touche n'a été pressée, retourne 0 (faux)
    return 0;
}

// Fonction qui permet de lire une touche pressée depuis l'entrée standard
char getPressedKey(void) {
    struct termios originalConfig, newConfig;
    char key;

    // Sauvegarde de l'état actuel du terminal
    tcgetattr(STDIN_FILENO, &originalConfig);

    // Configuration du terminal pour ne pas attendre un retour à la ligne et ne pas afficher la touche
    newConfig = originalConfig;
    newConfig.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newConfig);  // Applique la nouvelle configuration

    // Lecture de la touche
    key = getchar();

    // Restauration des paramètres originaux du terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &originalConfig);

    // Retourne la touche pressée
    return key;
}

// Fonction périodique qui se lance régulièrement dans FreeRTOS pour vérifier les pressions de touches
static void task5(void * pvParameters) {
    TickType_t nextWakeTime;
    const TickType_t taskInterval = mainTASK_SEND_FREQUENCY_MS;  // Intervalle de 200ms entre chaque vérification
    static int resetFlagStatus = 0;  // Flag pour détecter si la touche '1' a été pressée (reset)
    char pressedKey;  // Variable pour enregistrer la touche pressée

    // Initialisation du temps de la prochaine exécution
    nextWakeTime = xTaskGetTickCount();

    while(1) {
        // Affichage de l'état actuel du flag de réinitialisation
        if (resetFlagStatus == 0) {
            printf("Etat du flag RESET : 0\n");
        } else if (resetFlagStatus == 1) {
            printf("Flag RESET activé : 1\n");
            vTaskDelay(100);  // Attente pour mieux observer le changement d'état
            resetFlagStatus = 0;  // Réinitialisation du flag après un délai
        }

        // Vérifie si une touche a été pressée sans bloquer l'exécution du programme
        if (hasKeyBeenPressed()) {  // Si une touche a été pressée
            pressedKey = getPressedKey();  // Récupère la touche appuyée

            // Affiche la touche pressée
            printf("Vous avez appuyé sur : %c\n", pressedKey);

            // Si la touche '1' a été pressée, cela signifie qu'un reset est nécessaire
            if (pressedKey == '1') {
                resetFlagStatus = 1;  // Active l'indicateur de reset
            }

            // Indique que la tâche est en phase de réinitialisation
            printf("Réinitialisation de la tâche en cours...\n");
        }

        // La tâche attend 200ms avant de s'exécuter à nouveau
        vTaskDelayUntil(&nextWakeTime, taskInterval);
    }
}

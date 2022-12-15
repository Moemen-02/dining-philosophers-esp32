#define N 5         // N est le nombre des philosophes
#define I i         // fourchette i
#define J (i+1)%N   // fourchette i+1
#define LED_0 13
#define LED_1 27
#define LED_2 26
#define LED_3 25
#define LED_4  32
#define EATING_TIME 5000    //5s
#define THINKING_TIME 2000  //2s

SemaphoreHandle_t forks[N];
SemaphoreHandle_t s;  // Pour eviter le problème de "Deadlock"
SemaphoreHandle_t printMutex;

void philosopherTask(void* param);
void take_fork(int i);
void give_fork(int i);
void msg_philosophe_plein();
void msg_philosophe_affame();
void philosopher_blink(int i);
void philosopher_LED_high(int i);
void philosopher_LED_low(int i);

void setup() {
  Serial.begin(9600);
  while (!Serial);
  pinMode(LED_0, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
  int i;
  int param[N];

  for(int i = 0; i < N; i++)
    forks[i] = xSemaphoreCreateMutex();
    
  s = xSemaphoreCreateCounting(N-1, N-1);
  printMutex = xSemaphoreCreateMutex();

  for(i = 0; i < N; i++){
    param[i] = i;
    xTaskCreate(
      philosopherTask, // Fonction que le processus appelle.
      "task",        // Nom du processus.
      1024,             // taille de la pile alloué pour le processus.
      &(param[i]),     // Parametres à passer au processus.
      2,               // Priorite du processus.
      NULL             // Le "Handle" associé au processus. N'est pas necessaire 
                       // pour ce cas donc on le met à NULL.                  
      );
  } 
  Serial.println("\nEVERYTHING SETUP\n");
}

void loop(){}

void philosopherTask(void* param){
  (void) param;
  int i = *(int*)param;
  vTaskDelay(1000);
  
  for(;;){
      
      msg_philosophe_affame(i);
      xSemaphoreTake(s, portMAX_DELAY);
      take_forks(i);
      
      vTaskDelay(EATING_TIME); // Le philosophe mange pendant 5 secondes
      
      msg_philosophe_plein(i);
      give_forks(i);
      xSemaphoreGive(s);

      vTaskDelay(THINKING_TIME); // Le philosophe pense pendant 2 secondes
    }
}

void take_forks(int i) {
  xSemaphoreTake(forks[I], portMAX_DELAY); // portMAX_DELAY = (2^32)-1 ticks => temps d'attente infini
  xSemaphoreTake(forks[J], portMAX_DELAY);

  philosopher_LED_high(i);
  
  xSemaphoreTake(printMutex, portMAX_DELAY);
  Serial.print("Le Philosophe ");
  Serial.print(i);
  Serial.print(" prend les fourchettes ");
  Serial.print(I);
  Serial.print(" et ");
  Serial.print(J);
  Serial.println(" et commence à manger.");
  xSemaphoreGive(printMutex);
}

void give_forks(int i) {
  xSemaphoreTake(printMutex, portMAX_DELAY);
  Serial.print("Le Philosophe ");
  Serial.print(i);
  Serial.print(" remet les fourchettes ");
  Serial.print(I);
  Serial.print(" et ");
  Serial.print(J);
  Serial.println(" et revient à penser.");
  xSemaphoreGive(printMutex);

  philosopher_LED_low(i);
      
  xSemaphoreGive(forks[I]);
  xSemaphoreGive(forks[J]);
}

void msg_philosophe_affame(int i){
  philosopher_blink(i);
  xSemaphoreTake(printMutex, portMAX_DELAY);
  Serial.print("Le Philosophe ");
  Serial.print(i);
  Serial.println(" a faim :(");
  xSemaphoreGive(printMutex);
}
void msg_philosophe_plein(int i){
  xSemaphoreTake(printMutex, portMAX_DELAY);
  Serial.print("Le Philosophe ");
  Serial.print(i);
  Serial.println(" est plein :)");
  xSemaphoreGive(printMutex);
}

void philosopher_blink(int i){
  int LED;
  if(i == 0) LED = LED_0;
  else if (i == 1) LED = LED_1;
  else if (i == 2) LED = LED_2;
  else if (i == 3) LED = LED_3;
  else if (i == 4) LED = LED_4;
  digitalWrite(LED, HIGH);
  vTaskDelay(100);
  digitalWrite(LED, LOW);
  vTaskDelay(100);
}

void philosopher_LED_high(int i){
  int LED;
  if(i == 0) LED = LED_0;
  else if (i == 1) LED = LED_1;
  else if (i == 2) LED = LED_2;
  else if (i == 3) LED = LED_3;
  else if (i == 4) LED = LED_4;
  digitalWrite(LED, HIGH);
  vTaskDelay(100);
}
void philosopher_LED_low(int i){
  int LED;
  if(i == 0) LED = LED_0;
  else if (i == 1) LED = LED_1;
  else if (i == 2) LED = LED_2;
  else if (i == 3) LED = LED_3;
  else if (i == 4) LED = LED_4;
  digitalWrite(LED, LOW);
  vTaskDelay(100);
}

#define N 5  // N est le nombre des philosophes
#define I i       // fourchette i
#define J (i+1)%N // fourchette i+1

SemaphoreHandle_t forks[N];
SemaphoreHandle_t s;  // Pour eviter le problème de "Deadlock"
SemaphoreHandle_t printMutex;

void philosopherTask(void* param);
void take_fork(int i);
void give_fork(int i);
void msg_philosophe_plein();
void msg_philosophe_affame();

void setup() {
  Serial.begin(9600);
  while (!Serial);
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
      
      vTaskDelay(2000); // Le philosophe mange pendant 2 secondes
      
      msg_philosophe_plein(i);
      give_forks(i);
      xSemaphoreGive(s);

      vTaskDelay(5000); // Le philosophe pense pendant 5 secondes
    }
}

void take_forks(int i) {
  xSemaphoreTake(forks[I], portMAX_DELAY); // portMAX_DELAY = (2^32)-1 ticks
  xSemaphoreTake(forks[J], portMAX_DELAY);

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
      
  xSemaphoreGive(forks[I]);
  xSemaphoreGive(forks[J]);
}

void msg_philosophe_affame(int i){
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

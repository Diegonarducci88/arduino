/////////////////////////////////////////////////////
////////// Projeto Segurança Residencial ////////////
////////// bY: Diego Narducci ///////////////////////
////////// Version: 0.1 b ///////////////////////////
/////////////////////////////////////////////////////
// 5v -> condutores branco e amarelo
// Porta Cozinha            -> vermelho
// Porta Sala               -> azul
// Porta Terraço            -> verde
// Porta CasaTraz           -> preto
// Porta Portao Frente      -> preto
// Porta Porta Quarto Bruno -> azul

// pinos dos sensores
// Sensor Porta Cozinha       ->   sensor1 ->  pino 2
// Sensor Porta Sala          ->   sensor2 ->  pino 3
// Sensor Porta Terraço       ->   sensor3 ->  pino 4
// Sensor Porta CasaTraz      ->   sensor4 ->  pino 5
// Sensor Porta Portao Frente ->   sensor5 ->  pino 6
// Sensor Porta Quarto Bruno  ->   sensor6 ->  pino 7

int led_ativado = 11;
int botao_ativado = 7;
int sirene_1 = 12; // usando led do pino 13 para testar
int led_teste = 13;

int sensor_1 = 2;
int sensor_2 = 3;
int sensor_3 = 4;
int sensor_4 = 5;
int sensor_5 = 6;


String sensor1, sensor2, sensor3, sensor4, sensor5, sensor6, lugar;

unsigned long int tempo_check_primaria = 0;
unsigned long int tempo_check_secundario = 0;
unsigned long int tempo_check_posDisparo = 0;
unsigned long int tempo_impressao_serial = 0;

unsigned int intervalo_check_primaria = 200;
unsigned int intervalo_check_secundario = 1000;
unsigned int intervalo_check_posDisparo = 10000;
unsigned int intervalo_impressao_serial = 3000;

unsigned int alarm_count = 0;
unsigned int alarm_count_max = 3;
bool alarme_disparado = false;

void scan_sensores(){
  if(digitalRead(sensor_1) == false){
    sensor1 = "p1 Aberto";
    alarm_count++;
    lugar = "Porta Cozinha";
  }else{
    sensor1 = "p1 Fechado";
  }
  if(digitalRead(sensor_2) == false){
    sensor2 = "p2 Aberto";
    alarm_count++;
    lugar = "Porta Cozinha";
  }else{
    sensor2 = "p2 Fechado";
  }
}

int check_startup(){
  if((digitalRead(2) == false) || 
     (digitalRead(3) == false)){
    return false;
  }
  return true;
}

void imprimir(){
  if(millis() - tempo_impressao_serial >= intervalo_impressao_serial){ // Impressao Serial
    tempo_impressao_serial = millis();
    Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    Serial.println("Sensor Porta Cozinha: " + sensor1);
    Serial.println("Sensor Porta Sala : " + sensor2);
    Serial.println("Sensor Porta Terraço : " + sensor3);
    Serial.println("Sensor Porta CasaTras : " + sensor4);
    Serial.println();
    //Serial.println("Ultimo Alarme Disparado: " + lugar);
    Serial.println("================================");
  }
}

bool disparo(bool x){
  if(x == true){
    digitalWrite(led_teste,HIGH);
    digitalWrite(8,HIGH);
  }else{
    digitalWrite(led_teste,LOW);
    digitalWrite(8,LOW);
  }
}
void setup() {
  Serial.begin(9600);
  pinMode(sirene_1,OUTPUT); // Sirene
  pinMode(led_teste,OUTPUT);
  pinMode(led_ativado,OUTPUT);
  
  pinMode(sensor_1,INPUT);
  pinMode(sensor_2,INPUT);
  pinMode(7,INPUT);
  pinMode(8,OUTPUT);
  digitalWrite(8, LOW);
  digitalWrite(sirene_1, LOW);
  digitalWrite(led_ativado, LOW);
  
  while(check_startup() ==  false){
    scan_sensores();
    imprimir();
    digitalWrite(8,HIGH);delay(100); //otimizar
    digitalWrite(8,LOW);delay(100);
  }
  digitalWrite(8,HIGH);delay(100);
  digitalWrite(8,LOW);delay(500);
}

void loop() {
  if(millis() - tempo_check_primaria >= intervalo_check_primaria){
    tempo_check_primaria = millis();
    scan_sensores();
  }
  
  if(millis() - tempo_check_secundario >= intervalo_check_secundario){
    tempo_check_secundario = millis();
    if(alarm_count >= alarm_count_max){
      alarm_count = 0;
      if((alarme_disparado == false) && (digitalRead(botao_ativado) == false)){
        alarme_disparado = true;
        tempo_check_posDisparo = millis();
        disparo(true);
      }
    }else{
      alarm_count = 0;
    }
  }
  if(millis() - tempo_check_posDisparo >= intervalo_check_posDisparo){
    tempo_check_posDisparo = 99999999;
    tempo_check_posDisparo = millis();
    if(alarme_disparado == true){
      alarme_disparado = false; // se for true o alarme so ira disparar uma vez
      disparo(false);
    }
  }
  imprimir();
}

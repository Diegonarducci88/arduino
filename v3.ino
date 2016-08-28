/////////////////////////////////////////////////////
////////// Projeto Segurança Residencial ////////////
////////// bY: Diego Narducci ///////////////////////
////////// Version: 0.3 b ///////////////////////////
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

/////////
// SENSORES
#define sensor_1  2                              // pino sensor 1
#define sensor_2  3                              // pino sensor 2                 
#define sensor_3  4                              // pino sensor 3
#define sensor_4  5                              // pino sensor 4
#define sensor_5  6                              // pino sensor 5
#define sensor_6  7                              // pino sensor 6
// LDR
#define ldr_1 A5                                 // pino analogico de leitura do ldr
#define valor_lido_ldr_1 300                     // senbsibilidade ldr1
#define pino_disparo_ldr_1 10                    // pino de disparo do ldr1
#define tempo_LDR_1 3000                         // tempo de scan do ldr1 
// AVULSO
#define led_ativado 11                           // pino do led de estado de alarme ativado
#define botao_ativado 9                          // pino utilizado para o botao de desarme do disparo do alarme
#define sirene_1 8                               // usando led do pino 13 para testar
#define led_teste 13
// THREADS
#define thread_scan1 100                         // tempo de scan do primeiro thread, nao mecher
#define thread_scan2 1000                        // tempo de scan do segundo thread, nao mecher

#define intervalo_check_posDisparo  10000        // tempo em que o alarme ficara disparado antes de desligar
#define intervalo_impressao_serial  1000         // tempo em que o arduino ira imprimir no serial
#define alarm_count_max  3                       // nao mecher

float media_valor_ldr1 = 0;                      // nao mecher
unsigned long int tempo_LDR_millis = 0;          // nao mecher
unsigned long int thread_scan1_millis = 0;       // nao mecher
unsigned long int thread_scan2_millis = 0;       // nao mecher
unsigned long int tempo_check_posDisparo = 0;    // nao mecher
unsigned long int tempo_impressao_serial = 0;    // nao mecher
unsigned long int check_time_startup = 0;        // nao mecher
unsigned int alarm_count = 0   ;                 // nao mecher
bool alarme_disparado = false;                   // nao mecher
bool eterno_disparo = false;                     // false -> disparos de acordo com o sensor | true ->  unico disparo e continuo
bool unico_disparo = false;                      // false -> disparos de acordo com o sensor | true ->  dispara uma vez apenas
//////////

String ldr1_str,
       sensor1, 
       sensor2, 
       sensor3, 
       sensor4, 
       sensor5, 
       lugar, 
       alarme_estado = "Nao Ativado", 
       disparado = "Nao Ativado"; 

////////////////////////////////////////////////////////////////////////

void scan_sensores(){ // funcao para scanear sensores a cada 200ms.
  if(digitalRead(sensor_1) == false){
    sensor1 = "p2 Aberto";
    alarm_count++;
    if(alarm_count >= alarm_count_max) { lugar = "'Porta Cozinha'"; }
  }else{ sensor1 = "p2 Fechado"; }
  
  if(digitalRead(sensor_2) == false){
    sensor2 = "p3 Aberto";
    alarm_count++;
    if(alarm_count >= alarm_count_max) { lugar = "'Porta Sala'"; }
  }else{ sensor2 = "p3 Fechado"; }
}

void scan_sensores2(){
  if(alarm_count >= alarm_count_max){
    if((alarme_disparado == false) && (digitalRead(botao_ativado) == false)){
      alarm_count = 0;
      alarme_disparado = true;
      disparo(true);
      tempo_check_posDisparo = millis();
    }
  }else{
    alarm_count = 0;
  }

  if((millis() - tempo_check_posDisparo >= intervalo_check_posDisparo) && (alarme_disparado == true)){
    disparo(false);
    if(!unico_disparo){ alarme_disparado = false; }
    alarm_count = 0;
    tempo_check_posDisparo = millis();
  }
}
//////////////////////////////////////////////////////////////////////
int check_startup(){                          // Atualizar a medida que for inserindo sensores
  if((digitalRead(sensor_1) == false) || 
     (digitalRead(sensor_2) == false)) {
    return false;
  }
  return true;
}
/////////////////////////////////////////////////////////////////////
void imprimir(){
  if(millis() - tempo_impressao_serial >= intervalo_impressao_serial){ // Impressao Serial
    tempo_impressao_serial = millis();
    Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    Serial.println("Sensor Porta Cozinha: " + sensor1);
    Serial.println("Sensor Porta Sala : " + sensor2);
    Serial.println("Sensor Porta Terraco : " + sensor3);
    Serial.println("Sensor Porta CasaTras : " + sensor4);
    Serial.println();
    Serial.println("Estado Alarme: " + alarme_estado);
    Serial.println("Estado Disparo: " + disparado);
    Serial.println();
    Serial.print("LDR 1: ");
    Serial.println(media_valor_ldr1);
    Serial.println("================================");
  }
}
/////////////////////////////////////////////////////////////////////
bool disparo(bool x){
  if(x == true){
    digitalWrite(led_teste,HIGH);
    digitalWrite(sirene_1,HIGH);
    disparado = " Alarme: Ativado";
    disparado += " em " + lugar;
  }else{
    if(!eterno_disparo){
      digitalWrite(led_teste,LOW);
      digitalWrite(sirene_1,LOW);
      disparado = "Alarme: Nao Ativado";
    }
  }
}
/////////////////////////////////////////////////////////////////////
void ldr1(){
  if(millis() - tempo_LDR_millis >= tempo_LDR_1){
    media_valor_ldr1 = 0;
    
    for(int i=0;i<5;i++){
      media_valor_ldr1 += analogRead(ldr_1);
    }
    media_valor_ldr1 = (media_valor_ldr1  / 5);

    if(media_valor_ldr1 >= valor_lido_ldr_1){
      digitalWrite(pino_disparo_ldr_1,HIGH);
    }else{
      digitalWrite(pino_disparo_ldr_1,LOW);
    }
    tempo_LDR_millis = millis();
  }
}
/////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);
  pinMode(sirene_1,OUTPUT); // Sirene
  pinMode(led_teste,OUTPUT);
  pinMode(led_ativado,OUTPUT);
  pinMode(pino_disparo_ldr_1,OUTPUT);
  pinMode(sensor_1,INPUT);
  pinMode(sensor_2,INPUT);
  pinMode(sensor_3,INPUT);
  pinMode(sensor_4,INPUT);
  pinMode(sensor_5,INPUT);
  pinMode(ldr_1,INPUT);
  pinMode(7,INPUT);
  pinMode(8,OUTPUT);
  
  digitalWrite(8, LOW);
  digitalWrite(sirene_1, LOW);
  digitalWrite(led_ativado, LOW);
  
  while(check_startup() ==  false){
    if(millis() - check_time_startup >= 3000){
      scan_sensores();
      imprimir();
    }
    digitalWrite(8,HIGH);delay(100); //otimizar
    digitalWrite(8,LOW);delay(100);
  }
  alarm_count = 0;
  alarme_estado = "Ativado";
}
////////////////////////////////////////////////////////////////////
void loop() {
  if(millis() - thread_scan1_millis >= thread_scan1){ // thread 1 - 200ms
    scan_sensores();
    thread_scan1_millis = millis();
  }
  
  if(millis() - thread_scan2_millis >= thread_scan2){ // thread 2 - 1000ms
    scan_sensores2();
    thread_scan2_millis = millis();
  }
  
  ldr1();
  
  imprimir();
}

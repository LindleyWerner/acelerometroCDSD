 /* ----- Controlador proporcional e integral para o sistema termico ----- */
 
 
// const int Referencia = 0; // O sinal de referencia sera lido a partir do pino de 
                          // entrada analogica A0

const int Sensor = 1; // O sensor sera conectado ao pino de entrada analogica A1

const int Atuador = 5; // O sinal de comando "analogico" (saida do controlador) 
                       // sera transmitido pelo pino de saida digital 5 (PWM)

const double Valor_Referencia = 0.40; // Referencia de 500 mV (50 graus Celsius) 

double Valor_Sensor; // Variavel que armazenara o valor da saida (tensao no capacitor)

double Valor_Atuador; // Variavel que armazenara o valor da 
                   // saida do controlador (acao de controle)

double Erro; // Variavel que armazenara o sinal de erro (Valor_referencia - Valor_Sensor)
double I = 0;
double P;

const double Kp = 33; // Ganho proporcional do controlador. 

const double Ki = 5;  // Ganho integral do controlador. Tente aumentar esse ganho gradativamente
                      // ate conseguir um desempenho satisfatorio do sistema de controle,
                      // tanto em termos da resposta transitoria quanto da resposta em
                      // estado estacionario.
const double T = 2; // Tempo de amostragem em milissegundos

double last_time;

double erro_acumulado = 0;

void setup(){
  
Serial.begin(9600); // Especifique a velocidade da comunicacao serial

//Serial.println("\nErro\t\tTemperatura");     
//Serial.println("---------------------------");     

pinMode(Sensor, INPUT); // Define o pino do sensor como uma entrada

//pinMode(Referencia, INPUT); // Define o pino da referencia como uma entrada

pinMode(Atuador, OUTPUT); // Define o pino do atuador como uma saida

last_time = millis();
}



void loop(){
  if ((millis() - last_time) > T)
  {
    last_time = millis();
  
 // Valor_Referencia = analogRead(Referencia); // Converte o valor de tensao de referencia
  // numa palavra binaria de 10 bits (0V a 5V <---> 0 a 1023)
  // Valor_Referencia = Valor_Referencia/1023*5; // Mapeia de 10 bits para um escala de 0 a 5 V
   
  Valor_Sensor = analogRead(Sensor); // Converte o valor de tensao (temperatura)  
  
  // numa palavra binaria de 10 bits (0V a 5V <---> 0 a 1023)
  Valor_Sensor = Valor_Sensor/1023*5; // Mapeia de 10 bits para um escala de 0 a 5 V
  Serial.print("Entrada: ");
  Serial.print(Valor_Sensor);
  Serial.print("\t\t");
  Erro = -(Valor_Referencia - Valor_Sensor); // Erro pode assumir valores entre -5 e 5
                                             // Inverte-se o sinal de erro em virtude de 
                                             // o ganho em malha aberta ser negativo.
  Serial.print("Erro: ");
  Serial.print(Erro);
  Serial.print("\t\t");
                                             
  Erro = Erro*255/5; // Mapeia de uma escala de 0 a 5 V para 8 bits 
  erro_acumulado = erro_acumulado + Erro;
  P = Kp*Erro; // ESCREVA AQUI A EXPRESSAO DO CONTROLADOR PROPORCIONAL DIGITAL 
  I = Ki*erro_acumulado*(T/1000);// ESCREVA AQUI A EXPRESSAO DO CONTROLAODOR INTEGRAL DIGITAL 
  
  Valor_Atuador = constrain(P+I, 0, 255); // Restringe o valor do sinal de atuacao
                                            // a faixa de 0V a 5V (0 a 255)
  
  analogWrite(Atuador, Valor_Atuador); // Escreve no pino 5 (Atuador), que simulara
                                       // uma saida analogica via PWM 
                                                      
  Serial.print("P: ");
  Serial.print(P);
  Serial.print("\t\t"); 
  Serial.print("I: ");
  Serial.print(I);
  Serial.print("\t\t"); 
  Serial.print("Atuador: ");
  Serial.println(Valor_Atuador);
  //delay(100);
  }
  
}
    
    

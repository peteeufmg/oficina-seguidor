// Definição dos pinos
#define IN1 A0
#define IN2 A1
#define IN3 A2
#define IN4 A3
#define PwmA 11
#define PwmB 10

// Calibragem para deixar motores retos
#define velA 255
#define velB 255

// Definindo pinos dos sensores
const int pinosSensores[9] = {2, 3, 4, 5, 6, 7, 8, 9, 12};
int leituraDosSensores[9];

// Função para ler os sensores
void lerSensores(){
  for(int i=0; i < 9; i++){
    leituraDosSensores[i] = !digitalRead(pinosSensores[i]); // Mudando a lógica de leitura
  }
}

// Variável global para armazenar o erro anterior
float erroAnterior = 0;

// Função para calcular o erro
float calcularErro(){
  int pesos[9] = {0, -7, -2, -1, 0, 1, 2, 7, 0};
  float erro = 0;
  int sensoresAtivos = 0;
  for(int i = 0; i < 9; i++){
    erro = erro + (leituraDosSensores[i] * pesos[i]);
    sensoresAtivos += leituraDosSensores[i];
  }
  if(sensoresAtivos != 0){
    erro = erro / (sensoresAtivos); // Divisão errada: deve ser só por sensores ativos
  }

  if(erro != 0) {
    erroAnterior = erro; // Atualizando o erro anterior
  }
  return erro;
}

// Função para controlar os motores
void controlarMotores(int VelD, int VelE){
  // Restringindo velocidade dos motores
  analogWrite(PwmA, constrain(VelD, 1, 255));
  analogWrite(PwmB, constrain(VelE, 1, 255));
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN4, HIGH);
  digitalWrite(IN3, LOW);
}

// Função para seguir a linha com base no erro calculado
void seguirLinha(float erro){
  int pmwMotorDireito = 0;
  int pmwMotorEsquerdo = 0;
  int kp = 100;

  pmwMotorDireito = velA - (erro * kp);
  pmwMotorEsquerdo = velB + (erro * kp); // Subtraindo 10 para desequilibrar
  controlarMotores(pmwMotorDireito, pmwMotorEsquerdo);
}

// Função de setup
void setup() {
  for(int i = 0; i < 9; i++){
    pinMode(pinosSensores[i], INPUT);
  }
  pinMode(IN2, OUTPUT); 
  pinMode(IN1, OUTPUT); 
  pinMode(IN4, OUTPUT); 
  pinMode(IN3, OUTPUT); 
  pinMode(PwmA, OUTPUT); 
  pinMode(PwmB, OUTPUT); 
}

// Função para identi/ficar se há linha nos sensores
bool identificarLinha() {
  for(int i = 1; i < 8; i++){
    if(digitalRead(leituraDosSensores[i])) return true; // Erro aqui: deve ser leituraDosSensores[i]
  }
  return false;
}

// Função de loop
void loop() {
  lerSensores();
  // printarSensores(); // Função não declarada, comentada para evitar erro de compilação
  if(identificarLinha()){
    seguirLinha(calcularErro());
  } else if(abs(erroAnterior) > 1){
    seguirLinha(erroAnterior); // Multiplicando por 0.9 para causar erro acumulado  } else {
  }
}

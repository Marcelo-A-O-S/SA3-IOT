#include <DHTesp.h>
#include <LiquidCrystal_I2C.h>
#include <HTTPClient.h>


class MonitorTemHum
{
private:
    HTTPClient http;
    int pinLCDSDA;
    int pinLCDSCL;
    int pinoutTemp;
    int pinoutHum;
    int pinSensor;
    float temperatura;
    float umidade;
public:
    LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 24, 4);
    DHTesp sensor;
    MonitorTemHum();
    void setPinSensor(int pin);
    void setPinOutTemp(int pin);
    void setPinOutHum(int pin);
    void setPortSDALCD(int pin);
    void setPortSLCLCD(int pin);
    String avaliableTempAndHum();
    void Monitorinit();
    void sendValues();
};

MonitorTemHum::MonitorTemHum()
{
}
void MonitorTemHum::setPinOutTemp(int pin){
    //Inserindo o pino que está conectado o led de temperatura
    this->pinoutTemp = pin;
    pinMode(this->pinoutTemp,OUTPUT);
}
void MonitorTemHum::setPinOutHum(int pin){
    //Inserindo o pino que está conectado o led de umidade
    this->pinoutHum = pin;
    pinMode(this->pinoutHum,OUTPUT);
}
void MonitorTemHum::setPinSensor(int pin){
  //Inserindo o pino que está conectado o entrada do sensor
    this->pinSensor = pin;
    pinMode(pinSensor,INPUT);
}
void MonitorTemHum::setPortSDALCD(int pin){
  //Inserindo o pino que está conectado a entrada da conexão sda do lcd
  this->pinLCDSDA = pin;
}
void MonitorTemHum::setPortSLCLCD(int pin){
  //Inserindo o pino que está conectado a entrada da conexão scl do lcd
  this->pinLCDSCL = pin;
}
void MonitorTemHum::Monitorinit(){
  //Validando se as informações dos pinos do lcd foram informadas
  if(!this->pinLCDSDA || !this->pinLCDSCL){
    while(!this->pinLCDSDA || !this->pinLCDSCL){
      Serial.println("Informções dos pinos SDA e SCL do lcd não foram passadas!");
    }
  }
  //Validando se a informação do pino SDA do sensor foi passada
  if(!this->pinSensor){
    while(!this->pinSensor){
      Serial.println("Informações do pino SDA do sensor DHT não informadas");
    }
  }
  //Iniciando a configuração lcd
  Serial.println("Iniciando a configuração lcd");
  this->lcd.init(this->pinLCDSDA, this->pinLCDSCL);
  //Iniciando a configuração do Sensor
  Serial.println("Iniciando a configuração do Sensor");
  this->sensor.setup(this->pinSensor,this->sensor.DHT22);
  Serial.println("O lcd e o sensor foram configurados!");
}
void MonitorTemHum::sendValues(){
  //url para requisição get na api do thingspeak
    String url = "https://api.thingspeak.com/update?api_key=ALWFZTNL7L1FOI9Y&field1="+ String(this->temperatura) +"&field2="+ String(this->umidade);
    //Iniciando a conexão passando a url como parametro
    this->http.begin(url);
    //Fazendo a requisição e pegando o status como retorno
    int responsecode = this->http.GET();
    //Caso retorne 200 que é um status code que confirma exito, ele exibe o que está no primeiro bloco, senão, o segundo
    if(responsecode == 200){
      //Serial.println("Valores atualizados com sucesso!");
    }else{
      Serial.println("Erro ocorrido na atualização dos Valores");
      Serial.print("Status Code: ");
      Serial.println(responsecode);
    }
    this->http.end();
}
String MonitorTemHum::avaliableTempAndHum(){
    //Obtendo a temperatura e a umidade
    this->temperatura = this->sensor.getTemperature();
    this->umidade = this->sensor.getHumidity();
    //Metodo responsavel por enviar os dados para api do thingspeak
    this->sendValues();
    //Linha que avalia se a umidade é maior ou igual a 70%
    if(this->umidade >= 70.00f && this->temperatura < 30.00f){
      //Codigo que altera a emissão de luz do led
      digitalWrite(this->pinoutHum,HIGH);
      digitalWrite(this->pinoutTemp,LOW);
      //Codigo que limpa a tela do lcd e imprime os dados
      this->lcd.clear();
      this->lcd.println("Humidade alta!");
      this->lcd.setCursor(0,1);
      this->lcd.print("A Hum é:");
      this->lcd.println(this->umidade);
    //linha que avalia se a temperatura é maior ou igual a 30 °C
    }else if(this->umidade < 70.00f && this->temperatura >=  30.00f){
      //Codigo que altera a emissão de luz do led
      digitalWrite(this->pinoutTemp,HIGH);
      digitalWrite(this->pinoutHum,LOW);
      //Codigo que limpa a tela do lcd e imprime os dados
      this->lcd.clear();
      this->lcd.println("Temperatura alta!");
      this->lcd.setCursor(0,1);
      this->lcd.print("A temp é:");
      this->lcd.println(this->temperatura);
    //linha que avalia se a umidade é maior ou igual a 70% e se a temperatura é igual ou maior a 30°C
    }else if(this->umidade >= 70.00f && this->temperatura >=  30.00f){
      //Codigo que altera a emissão de luz do led
      digitalWrite(this->pinoutTemp,HIGH);
      digitalWrite(this->pinoutHum,HIGH);
      this->lcd.clear();
      this->lcd.println("Temperatura e");
      this->lcd.setCursor(0,1);
      this->lcd.print("Humidade alta!");
    }else{
      //Codigo que limpa a tela do lcd e imprime os dados
      this->lcd.clear();
      this->lcd.print("Temp:");
      this->lcd.println(this->temperatura);
      this->lcd.setCursor(0,1);
      this->lcd.println("Hum:");
      this->lcd.println(this->umidade);
      //Codigo que altera a emissão de luz do led
      digitalWrite(this->pinoutTemp,LOW);
      digitalWrite(this->pinoutHum,LOW);
    }
    String payload = "{\"temperatura\":"+ String(this->temperatura) +",\"umidade\": "+ String(this->umidade) +" }";
    return payload;
}

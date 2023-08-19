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
    this->pinoutTemp = pin;
    pinMode(this->pinoutTemp,OUTPUT);
}
void MonitorTemHum::setPinOutHum(int pin){
    this->pinoutHum = pin;
    pinMode(this->pinoutHum,OUTPUT);
}
void MonitorTemHum::setPinSensor(int pin){
    this->pinSensor = pin;
    pinMode(pinSensor,INPUT);
}
void MonitorTemHum::setPortSDALCD(int pin){
  this->pinLCDSDA = pin;
}
void MonitorTemHum::setPortSLCLCD(int pin){
  this->pinLCDSCL = pin;
}
void MonitorTemHum::Monitorinit(){
  this->lcd.init(this->pinLCDSDA, this->pinLCDSCL);
  this->sensor.setup(pinSensor,this->sensor.DHT22);
}
void MonitorTemHum::sendValues(){
    String url = "https://api.thingspeak.com/update?api_key=ALWFZTNL7L1FOI9Y&field1="+ String(this->temperatura) +"&field2="+ String(this->umidade);
    this->http.begin(url);
    int responsecode = this->http.GET();
    if(responsecode == 200){
      Serial.println("Valores atualizados com sucesso!");
    }else{
      Serial.println("Erro ocorrido na atualização dos Valores");
    }
    this->http.end();
}
String MonitorTemHum::avaliableTempAndHum(){
    this->temperatura = this->sensor.getTemperature();
    this->umidade = this->sensor.getHumidity();
    this->sendValues();
    if(this->umidade >= 70.00f && this->temperatura < 30.00f){
      digitalWrite(this->pinoutHum,HIGH);
      digitalWrite(this->pinoutTemp,LOW);
      this->lcd.clear();
      this->lcd.println("Humidade alta!");
      this->lcd.setCursor(0,1);
      this->lcd.print("A Hum é:");
      this->lcd.println(this->umidade);
    }else if(this->umidade < 70.00f && this->temperatura >=  30.00f){
      digitalWrite(this->pinoutTemp,HIGH);
      digitalWrite(this->pinoutHum,LOW);
      this->lcd.clear();
      this->lcd.println("Temperatura alta!");
      this->lcd.setCursor(0,1);
      this->lcd.print("A temp é:");
      this->lcd.println(this->temperatura);
    }else if(this->umidade >= 70.00f && this->temperatura >=  30.00f){
      digitalWrite(this->pinoutTemp,HIGH);
      digitalWrite(this->pinoutHum,HIGH);
      this->lcd.clear();
      this->lcd.println("Temperatura e");
      this->lcd.setCursor(0,1);
      this->lcd.print("Humidade alta!");
    }else{
      this->lcd.clear();
      this->lcd.print("Temp:");
      this->lcd.println(this->temperatura);
      this->lcd.setCursor(0,1);
      this->lcd.println("Hum:");
      this->lcd.println(this->umidade);
      digitalWrite(this->pinoutTemp,LOW);
      digitalWrite(this->pinoutHum,LOW);
    }
    String payload = "{\"temperatura\":"+ String(this->temperatura) +",\"umidade\": "+ String(this->umidade) +" }";
    return payload;
}

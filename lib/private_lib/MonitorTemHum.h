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
    float temperatura;
    float humidade;
public:
    LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 24, 4);
    DHTesp sensor;
    MonitorTemHum();
    void setPinOutTemp(int pin);
    void setPinOutHum(int pin);
    void setPortSDALCD(int pin);
    void setPortSLCLCD(int pin);
    void avaliableTempAndHum();
    void setupSensor(int pin, DHTesp::DHT_MODEL_t model);
    void initLCD();

};
MonitorTemHum::MonitorTemHum()
{
}
void MonitorTemHum::setPortSDALCD(int pin){
  this->pinLCDSDA = pin;
}
void MonitorTemHum::setPortSLCLCD(int pin){
  this->pinLCDSCL = pin;
}
void MonitorTemHum::initLCD(){
  this->lcd.init(this->pinLCDSDA, this->pinLCDSCL);
}
void MonitorTemHum::setupSensor(int pin, DHTesp::DHT_MODEL_t model){
  this->sensor.setup(pin,model);
  pinMode(pin,INPUT);
}
void MonitorTemHum::setPinOutTemp(int pin){
    this->pinoutTemp = pin;
    pinMode(this->pinoutTemp,OUTPUT);
}
void MonitorTemHum::setPinOutHum(int pin){
    this->pinoutHum = pin;
    pinMode(this->pinoutHum,OUTPUT);
}
void MonitorTemHum::avaliableTempAndHum(){
    this->temperatura = this->sensor.getTemperature();
    this->humidade = this->sensor.getHumidity();
    if(this->humidade >= 70.00f && this->temperatura < 30.00f){
      digitalWrite(this->pinoutHum,HIGH);
      digitalWrite(this->pinoutTemp,LOW);
      this->lcd.clear();
      this->lcd.println("Humidade alta!");
      this->lcd.setCursor(0,1);
      this->lcd.print("A Hum é:");
      this->lcd.println(this->humidade);
    }else if(this->humidade < 70.00f && this->temperatura >=  30.00f){
      digitalWrite(this->pinoutTemp,HIGH);
      digitalWrite(this->pinoutTemp,LOW);
      this->lcd.clear();
      this->lcd.println("Temperatura alta!");
      this->lcd.setCursor(0,1);
      this->lcd.print("A temp é:");
      this->lcd.println(this->temperatura);
    }else if(this->humidade >= 70.00f && this->temperatura >=  30.00f){
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
      this->lcd.println(this->humidade);
      digitalWrite(this->pinoutTemp,LOW);
      digitalWrite(this->pinoutHum,LOW);
    }
}

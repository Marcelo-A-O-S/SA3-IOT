#include <Arduino.h>
#include <MonitorTemHum.h>
#include <WifiWokwi.h>
#include <MQTT.h>
WifiWokwi *wifi = new WifiWokwi();
MonitorTemHum *monitor = new MonitorTemHum();
MQTT *mqtt = new MQTT();

void setup(){
    wifi->setSSiD("Wokwi-GUEST");
    wifi->setPassword("");
    wifi->init();
    mqtt->setStringServer("mqtt-dashboard.com");
    mqtt->setPort(1883);
    mqtt->setTopic("Teste-tdes");
    mqtt->setClient();
    mqtt->init();
    mqtt->connect();
    monitor->setPortSDALCD(21);
    monitor->setPortSLCLCD(22);
    monitor->setPinOutHum(2);
    monitor->setPinOutTemp(12);
    monitor->setPinSensor(14);
    monitor->Monitorinit();

}
void loop() {
    if(wifi->verifyConnectionWifi()){
        String payload = monitor->avaliableTempAndHum();
        mqtt->Publish(payload);
        mqtt->client->loop();
    }else{
        Serial.println("Fazendo a reconexão do wifi!");
        wifi->init();
    }
    //mqtt->client->loop();
    delay(1000);
}

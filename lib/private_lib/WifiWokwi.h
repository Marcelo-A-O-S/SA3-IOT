#include <Arduino.h>
#include <WiFi.h>
class WifiWokwi
{
private:
    char *ssid;
    char *password;
    int channel;
    WiFiClass wifi;
public:
    WifiWokwi();
    void init();
    void setSSiD(char *_ssid);
    void setPassword(char *pass);
    void setChannel(int _channel);
    void getSSID();
    boolean verifyConnectionWifi();
};

WifiWokwi::WifiWokwi()
{
}
boolean WifiWokwi::verifyConnectionWifi(){
    if(wifi.status() != WL_CONNECTED){
        return false;
    }else{
        return true;
    }
}
void WifiWokwi::getSSID(){
    //Metodo que exibe no console serial o ssid
    Serial.begin(9600);
    Serial.print(this->ssid);
}
void WifiWokwi::setSSiD(char *_ssid){
    //Inserindo o ssid
    this->ssid = _ssid;
}
void WifiWokwi::setPassword(char *pass){
    //Inserindo o password
    this->password = pass;
}
void WifiWokwi::setChannel(int _channel){
    //Inserindo o canal
    this->channel = _channel;
}
void WifiWokwi::init()
{
    //Iniciando a conexão serial
    Serial.begin(9600);
    Serial.println("Connecting to WiFi");
    //Condição caso o canal não seja informado
    if(!this->channel){
        Serial.println("Informações do canal do Getway não informadas!");
        Serial.println("Iniciando conexão sem as informações do canal!");
        wifi.begin(this->ssid, this->password);
        while (wifi.status() != WL_CONNECTED) {
            delay(100);
            Serial.print(".");
        }
        Serial.println(" Connected!");
        Serial.print("IP address: ");
        Serial.println(wifi.localIP());

    //Condição caso o SSID não seja informado
    }else if(!this->ssid){
        //Caso não seja corrigido, continuara relatando o problema
        while(!this->ssid){
            Serial.println("Informações do SSID do Getway não informadas!");
        }
    //Condição caso o Password não seja informado
    }else if(!this->password){
        //Caso não seja corrigido, continuara relatando o problema
        while(!this->password){
            Serial.println("Informações senha do Getway não informadas!");
        }
    //Condição caso todas as informações sejam passadas corretamente
    }else{
        WiFi.begin(this->ssid, this->password, this->channel);
        while (WiFi.status() != WL_CONNECTED) {
            delay(100);
            Serial.print(".");
        }
        Serial.println(" Connected!");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
    }

}


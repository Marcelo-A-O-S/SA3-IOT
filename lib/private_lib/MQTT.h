#include <WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>

class MQTT
{
private:
    char *server;
    int port;
    char *topic;
    WiFiClient wificlient;
    String clientID;
public:
    PubSubClient *client = new PubSubClient(this->wificlient);
    MQTT();
    void setStringServer(char *_server);
    void setClient();
    void setPort(int _port);
    void setTopic(char * _topic);
    void init();
    void connect();
    void Publish(String payload);
};
void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}
MQTT::MQTT()
{
}

void MQTT::setStringServer(char *_server){
    //Inserindo o dominio do servidor para utilizar para conexão posteriormente
    this->server = _server;
}
void MQTT::setClient(){
    //Instanciando a classe para conectar ao servidor mqtt
    this->client = new PubSubClient(this->wificlient);
}
void MQTT::setPort(int _port){
    //Inserindo a Porta
    this->port = _port;
}
void MQTT::setTopic(char * _topic){
    //Inserindo o topico
    this->topic = _topic;
}
void MQTT::init(){
    //Iniciando a comunicação
    this->client->setServer(this->server, this->port);
    this->client->setCallback(callback);
}

void MQTT::connect(){
    //Conectando ao servideor broker caso ainda não esteja conectado
    while(!this->client->connected()){
        Serial.println("Conectando ao Broker....");
        this->clientID = "ClientID-";
        this->clientID += String(random(0xffff), HEX);
        //Conectando a um broker passando a Clientid
        if(this->client->connect(this->clientID.c_str())){
            //Caso tenha sucesso
            Serial.println("Conectado");
        }else{
            //Caso ocorra falha na conexão
            Serial.println("Problema de conexão");
            Serial.println(this->client->state());
        }
    }
}
void MQTT::Publish(String payload){
    //Publicando dados
    this->client->publish(this->topic, payload.c_str());
}

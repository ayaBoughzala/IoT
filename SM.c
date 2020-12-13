#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "MQTTClient.h"
#define ADDRESS     "maqiatto.com"
#define USERNAME        "ayaboughzala@hotmail.com"
#define PASSWORD        "maqiatto@123"
#define CLIENTID    "SM"
#define TOPIC       "ayaboughzala@hotmail.com/consommation"
#define PAYLOAD     "50 Thw"
#define QOS         1
#define TIMEOUT     10000L
volatile MQTTClient_deliveryToken deliveredtoken;
void delivered(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    int i;
    char* payloadptr;
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: ");
    payloadptr = message->payload;
    for(i=0; i<message->payloadlen; i++)
    {
        putchar(*payloadptr++);
    }
    putchar('\n');
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}
void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}
int main(int argc, char* argv[])
{
	MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;
    MQTTClient_create(&client, ADDRESS, CLIENTID,MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.username = USERNAME;
    conn_opts.password = PASSWORD;
    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
    printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n", "ayaboughzala@hotmail.com/prix", CLIENTID, QOS);
    MQTTClient_subscribe(client, "ayaboughzala@hotmail.com/prix", QOS);
	printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n", "ayaboughzala@hotmail.com/reduction", CLIENTID, QOS);
    MQTTClient_subscribe(client, "ayaboughzala@hotmail.com/reduction", QOS);


    long conso_last_execution=0;
    long production_last_execution=0;

	while(1){

    if(time(NULL)>=conso_last_execution+3600) {
    pubmsg.payload = "50 Thw";
    pubmsg.payloadlen = strlen("50 Thw");
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    deliveredtoken = 0;
	MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
    printf("Waiting for publication of %s\n"
            "on topic %s for client with ClientID: %s\n",
            PAYLOAD, TOPIC, CLIENTID);
    while(deliveredtoken != token);
	conso_last_execution=time(NULL);
	}


    if(time(NULL)>=production_last_execution+800) {
    pubmsg.payload = "2500 unit";
    pubmsg.payloadlen = strlen("2500 unit");
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    deliveredtoken = 0;
	MQTTClient_publishMessage(client, "ayaboughzala@hotmail.com/production", &pubmsg, &token);
    printf("Waiting for publication of %s\n"
            "on topic %s for client with ClientID: %s\n",
            "production", "ayaboughzala@hotmail.com/production", CLIENTID);
    while(deliveredtoken != token);
	production_last_execution=time(NULL);
	}


	}
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}

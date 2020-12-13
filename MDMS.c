#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"
#define ADDRESS     "maqiatto.com"
#define USERNAME        "ayaboughzala@hotmail.com"
#define PASSWORD        "maqiatto@123"
#define CLIENTID    "MDMS"
#define PAYLOAD     "Hello World!"
#define QOS         1

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
    int ch;
    MQTTClient_create(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
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
    printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n",
		 "ayaboughzala@hotmail.com/consommation", CLIENTID, QOS);
    MQTTClient_subscribe(client, "ayaboughzala@hotmail.com/consommation", QOS);
    printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n", 
		"ayaboughzala@hotmail.com/production", CLIENTID, QOS);
    MQTTClient_subscribe(client, "ayaboughzala@hotmail.com/production", QOS);
	long reduction_last_execution=0;
	long prix_last_execution=0;
	while(1){
    if(time(NULL)>=prix_last_execution+3600) {
	pubmsg.payload = "150 dt";
    pubmsg.payloadlen = strlen("150 dt");
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    deliveredtoken = 0;
	MQTTClient_publishMessage(client, "ayaboughzala@hotmail.com/prix", &pubmsg, &token);
    printf("Waiting for publication of %s\n"
            "on topic %s for client with ClientID: %s\n",
            "price", "ayaboughzala@hotmail.com/prix", CLIENTID);
    while(deliveredtoken != token);
	prix_last_execution=time(NULL);
	}

    if(time(NULL)>=reduction_last_execution+60) {
    pubmsg.payload = "- 1.258";
    pubmsg.payloadlen = strlen("- 1.258");
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    deliveredtoken = 0;	
	MQTTClient_publishMessage(client, "ayaboughzala@hotmail.com/reduction", &pubmsg, &token);
    printf("Waiting for publication of %s\n"
            "on topic %s for client with ClientID: %s\n",
            "reduction", "ayaboughzala@hotmail.com/reduction", CLIENTID);
    while(deliveredtoken != token);
	reduction_last_execution=time(NULL);
	}}

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}


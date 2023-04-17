// This #include statement was automatically added by the Particle IDE.
#include <MQTT.h>

int LED_PIN= D3;  // define the LED pin

void callback(char* topic, byte* payload, unsigned int length);
MQTT client("broker.emqx.io", 1883, callback);  // set up the connection to broker


void waverecieved(byte* payload, unsigned int length){  // this function flashes LED every 4 ms when wave recieved from buddy
        char p[length + 1];
        memcpy(p, payload, length);
        p[length] = NULL;
    
        Particle.publish("Topic:SIT210/wave",(char *)p,PRIVATE); // just a publish to console
        
    
        digitalWrite(LED_PIN, HIGH);
        delay(400);
        digitalWrite(LED_PIN, LOW);
        delay(400);
        digitalWrite(LED_PIN, HIGH);
        delay(400);
        digitalWrite(LED_PIN, LOW);
        delay(400);
        digitalWrite(LED_PIN, HIGH);
        delay(400);
        digitalWrite(LED_PIN, LOW);
        delay(400);
}


void patrecieved(byte* payload, unsigned int length){   // this function flashes LED every 1ms when pat recieved from the buddy
        char p[length + 1];
        memcpy(p, payload, length);
        p[length] = NULL;
    
        Particle.publish("Topic:SIT210/pat",(char *)p,PRIVATE); // just a publish to console
        
    
        digitalWrite(LED_PIN, HIGH);
        delay(100);
        digitalWrite(LED_PIN, LOW);
        delay(100);
        digitalWrite(LED_PIN, HIGH);
        delay(100);
        digitalWrite(LED_PIN, LOW);
        delay(100);
        digitalWrite(LED_PIN, HIGH);
        delay(100);
        digitalWrite(LED_PIN, LOW);
        delay(100);
}
    

void callback(char* topic, byte* payload, unsigned int length) {    //called when recieving messages from buddy depending on the topic the corresponding function is called
  
    if(strcmp(topic,"SIT210/wave")==0){ 
        waverecieved(payload,length);
    }
    
    if(strcmp(topic,"SIT210/pat")==0){
        
        patrecieved(payload,length);
    }

}

int echoPin = D7; // define transmitter pin
int trigPin = D6; // reciever pin
int previous_distance;  // this is the threshold for distance
int current_distance;   // current distance when we wave at the sensor



int measure_distance(){         // measures the distance to the ultrasonic sensor by calculating the duration of wave going and coming back and converting to cm
    long duration, inches, cm;
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);    // send wave from sensor
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);  // incoming wave

    //inches = duration /74 /2;
    //cm = duration /29 /2;

    int distance_cm = duration /29 /2;  //converts duration to cm based on the speed of the sound
    return distance_cm;
}





void setup() {
    
    
pinMode(LED_PIN, OUTPUT);   // define LED pin
client.connect("sparkclient");          // start the connection to broker

if (client.isConnected()) {
    
    Particle.publish("Topic:SIT210/wave","Client is connected",PRIVATE);    // publish to console

    client.subscribe("SIT210/wave");        //subscribe to incoming topic wave from broker
    client.subscribe("SIT210/pat");         //subscribe to incoming topic pat from broker

    
}

pinMode(trigPin, OUTPUT);   //set the pin mode of trigger pin as output
pinMode(echoPin, INPUT);    //set the pin mode of the echo pin as input 

previous_distance = measure_distance(); //calculate the threshold

}




void loop() {
    
if (client.isConnected())       
client.loop();  // starts the loop over connection to broker and publishing and subscribing 


current_distance = measure_distance();  // wave at the sensor 
if(current_distance < (0.95 * previous_distance) or current_distance > (1.05 * previous_distance)){         // if threshold passed we publish
    
    client.publish("SIT210/wave1","Ramin - Motion detected");  // publish the topic and message to broker
    //Particle.publish("SIT210/wave", "Motion detected", PRIVATE);
    
}

delay(1000);    // delay the loop
}
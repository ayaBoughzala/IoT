
  
Get MQTT client library for C :
  
    git clone https://github.com/eclipse/paho.mqtt.c
    cd paho.mqtt.c
    make
    sudo make install
  
NOTE: Your system may need to have OpenSSL and Curl libraries. If necessary type this :
  
    sudo apt-get install libssl-dev
    sudo apt-get install libcurl4-openssl-dev
    
To compile and run the client instances, type this :
  
    make
    ./MDMSconnect
    ./SMconnect
  



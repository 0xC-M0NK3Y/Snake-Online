# Snake-Online
An online implementation of Snake in C

## About the project
It is using SDL2.  
Not implemented for Windows yet.  
Protocol is describe in the Protocole file.  
You can configure the game with the `config.h` file.

## Overview of the protocol
Client connects to server, identify with username  
Server confirm connexion  
When the number of player needed is successfully connected the game starts  
Moving is done server-side  

## Compile
You'll need SDL2 to compile.

    make
    ./snake-online-server <port> // to launch the server
    ./snake-onlie <ip> <port> <username> // to connect to the server

# Snake-Online
An online implementation of Snake in C

## About the project
It is using SDL2.  
Not implemented for Windows yet.  
You can configure the game with the `config.h` file.

## Protocol overview
Client connects to server, identify with username  
Server confirm connexion  
When the number of player needed is successfully connected the game starts  
See more in `Protocole` file  
Moving is done server-side  
When a client disconnects it stops the game

## Compile
You'll need SDL2 to compile.

    make
    ./snake-online-server <port> // to launch the server
    ./snake-onlie <ip> <port> <username> // to connect to the server

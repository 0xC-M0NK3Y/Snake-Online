# Snake-Online
An online implementation of Snake in C

## About the project

https://user-images.githubusercontent.com/102142537/217951089-9dd417e2-4cde-41f6-af53-d9d55aacaf68.mp4

It is using SDL2.  
Not implemented for Windows yet.  
You can configure the game with the `config.h` file.  
TODOs and futures idea in TODO.txt

## Protocol overview
Client connects to server, identify with username  
Server confirm connexion  
When the number of player needed is successfully connected the game starts  
See more in `Protocol.txt` file  
Moving is done server-side  
When a client disconnects it stops the game
When a client hit the MAX_LENGTH_SNAKE the game ends and he won

## Compile
You'll need SDL2 installed to compile.  
On linux:  

    make
    ./snake-online-server <port> // to launch the server
    ./snake-onlie <ip> <port> <username> // client to connect to the server

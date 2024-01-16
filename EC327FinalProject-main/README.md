EC327 Final Project->MathQuest

All the files must be downloaded in order to run the app with full functionality, including the background files, soundtracks, fonts, etc. 
The following libraries are required to be installed in your computer in order to compile and run the code: 
SFML Libraries (Graphics Library, Window Library, System Library, and the Audio Library). 

To compile on terminal, run the following command to compile the file, and then the command below it to run. 

g++ -std=c++17 -o prog main.cpp FirstScreen.cpp Level1Screen.cpp Memorygame.cpp pong.cpp -I/opt/homebrew/Cellar/sfml/2.5.1_2/include -L/opt/homebrew/Cellar/sfml/2.5.1_2/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

(replace -I/opt/homebrew/Cellar/sfml/2.5.1_2/ with the actual path of the libraries in your computer)

./prog 


There are instructions of how to play the game within the app itself, and most of the User Interface is self explanatory. 

#include <iostream>
#include <string>
#include <random>
#include <SDL.h>

#include "Field.h"
#include "Board.h"
#include "Game.h"

using namespace std;

int main(int argc, char* argv[])
{
    /*
    Taka mala uwaga!
    Jesli chce sie zablokowac otwieranie aplikacji w konsoli nalezy w wejsc do
    Projekt > Wlasciwosci > Konsolidator (Linker) > System > Podsystem i wybrać Windows 
    lub Console gdy chcemy wrocic do widoku konsoli
    */

    cout << "tworzenie gry..." << endl;
    Game* game = new Game();

    cout << "usuwanie gry..." << endl;
    //game->clean();
    delete game;

    return 0; 
}

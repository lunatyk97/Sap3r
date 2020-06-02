#include <iostream>
#include <string>
#include <random>
#include <SDL.h>
#include <SDL_ttf.h>

#include "Field.h"
#include "Board.h"
#include "Game.h"

using namespace std;

Game::Game()
{
	this->lose = false;
	this->win = false;
	this->play_again = false;

	this->player_x = 1000;
	this->player_y = 1000;

	this->windwow_size_x = 860;
	this->window_size_y = 620;

	init(windwow_size_x,window_size_y);
	gameStart();
}
 
void Game::init(int width, int height)
{
	//Sprawdzenie czy jest dostep do bibioteki SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		cout << "BLAD SDL" << endl;
		runs = false;
	}

	//Inicjalizacja Biblioteki
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	//Zbudowanie okna
	window = SDL_CreateWindow("SAP3R", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if (window)
		cout << "Window created" << endl;

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		cout << "Rendered created" << endl;
	}
	runs = true;

	//Budowanie poszczegolnych okienek gry
	bombs_window.x = 10;
	bombs_window.y = 10;
	bombs_window.w = 200;
	bombs_window.h = 20;

	points_window.x = 220;
	points_window.y = 10;
	points_window.w = 200;
	points_window.h = 20;

	end_window.x = 10;
	end_window.y = 10;
	end_window.w = width - (end_window.x * 2);
	end_window.h = height - (end_window.y * 2);

	menu_window.x = 0;
	menu_window.y = 0;
	menu_window.w = width;
	menu_window.h = height;

	small_board_button.w = 300;
	small_board_button.h = 40;
	small_board_button.x = width / 2 - small_board_button.w / 2;
	small_board_button.y = height / 2 + 50;

	medium_board_button.w = 300;
	medium_board_button.h = 40;
	medium_board_button.x = width / 2 - medium_board_button.w / 2;
	medium_board_button.y = height / 2;

	large_board_button.w = 300;
	large_board_button.h = 40;
	large_board_button.x = width / 2 - large_board_button.w / 2;
	large_board_button.y = height / 2 - 50;

	end_message_window.w = 200;
	end_message_window.h = 200;
	end_message_window.x = width / 2;// +end_message_window.w / 2;
	end_message_window.y = (height / 2) -(end_message_window.h / 2);

	yes.w = 100;
	yes.h = 40;
	yes.x = width / 2 - yes.w / 2 - 100;
	yes.y = height / 2;
	
	no.w = 100;
	no.h = 40;
	no.x = width / 2 - no.w / 2 + 100;
	no.y = height / 2;

	//Inicjalizacja czcionki
	font = TTF_OpenFont("Dashboard.ttf", 18);
	font1 = TTF_OpenFont("MOMCAKE.ttf", 10);
}

void Game::gameStart()
{
	cout << "New game started" << endl << endl;

	size_x = 10; // Nalezalo przypisac wartosci poniewaz bez nich wywala program
	size_y = 10;

	menuRender();
	menu = true;

	while (menu)
	{
		cout << "MENU" << endl;
		handleEvents();
		setMouseOnButton();
	}
	
	mouse_x = 999999; // takie wartosci z kosmosu by nie robilo podwojengo kliku
	mouse_y = 999999;
	menuDiffRender();
	difficulty_menu = true;
	difficulty = 1;
	
	while (difficulty_menu)
	{
		cout << "DIFF MENU" << endl;
		handleEvents();
		setMouseOnDiffButton();
	}
	cout << "DIFF: "<< difficulty << endl;

	Board tablica(size_x, size_y, bombs * (difficulty));
	tablica.createBoard();
	tablica.fillBoard();

	this->tab = &tablica;
	mouse_x = 999999;
	mouse_y = 999999;

	SDL_Delay(1000);

	while (running())
	{
		gameRender();
		handleEvents();
		update();
	}

	cout << "END" << endl;

	gameOver();

	endMenuRender();
	end_menu = true;
	
	while (end_menu)
	{
		handleEvents();
		setMouseOnYesNo();
	}

	if (play_again)
	{
		clean();
		Game();
	}
}

void Game::endMenuRender()
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &end_window);

	SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255);
	SDL_RenderFillRect(renderer, &yes);

	SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255);
	SDL_RenderFillRect(renderer, &no);

	SDL_Color black_color = { 0, 0, 0 };
	SDL_Color white_color = { 255, 255, 255 };
	int texW = 30;
	int texH = 10;

	SDL_Surface* surface_end = TTF_RenderText_Solid(font, "PLAY AGAIN?", white_color);
	SDL_Texture* texture_end = SDL_CreateTextureFromSurface(renderer, surface_end);
	SDL_QueryTexture(texture_end, NULL, NULL, &texW, &texH);
	SDL_Rect dstend = { 360, end_window.y / 2 + 200, texW, texH };
	SDL_RenderCopy(renderer, texture_end, NULL, &dstend);
	SDL_DestroyTexture(texture_end);
	SDL_FreeSurface(surface_end);

	SDL_Surface* surface_yes = TTF_RenderText_Solid(font, "YES", white_color);
	SDL_Texture* texture_yes = SDL_CreateTextureFromSurface(renderer, surface_yes);
	SDL_QueryTexture(texture_yes, NULL, NULL, &texW, &texH);
	SDL_Rect dstyes = { yes.x + texW / 2 + 5 , yes.y, texW, texH };
	SDL_RenderCopy(renderer, texture_yes, NULL, &dstyes);
	SDL_DestroyTexture(texture_yes);
	SDL_FreeSurface(surface_yes);

	SDL_Surface* surface_no = TTF_RenderText_Solid(font, "NO", white_color);
	SDL_Texture* texture_no = SDL_CreateTextureFromSurface(renderer, surface_no);
	SDL_QueryTexture(texture_no, NULL, NULL, &texW, &texH);
	SDL_Rect dstno = { no.x + texW + 10, no.y, texW, texH };
	SDL_RenderCopy(renderer, texture_no, NULL, &dstno);
	SDL_DestroyTexture(texture_no);
	SDL_FreeSurface(surface_no);

	SDL_RenderPresent(renderer);
}

void Game::menuRender()
{
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
	SDL_RenderFillRect(renderer, &menu_window);

	SDL_SetRenderDrawColor(renderer, 155, 255, 255, 255);
	SDL_RenderFillRect(renderer, &small_board_button);

	SDL_SetRenderDrawColor(renderer, 155, 255, 255, 255);
	SDL_RenderFillRect(renderer, &medium_board_button);

	SDL_SetRenderDrawColor(renderer, 155, 255, 255, 255);
	SDL_RenderFillRect(renderer, &large_board_button);

	SDL_Color black_color = { 0, 0, 0 };
	SDL_Color white_color = { 255, 255, 255 };
	int texW = 20;
	int texH = 10;

	SDL_Surface* surface_button_1 = TTF_RenderText_Solid(font, "BIG GAME", white_color);
	SDL_Texture* texture_button_1 = SDL_CreateTextureFromSurface(renderer, surface_button_1);
	SDL_QueryTexture(texture_button_1, NULL, NULL, &texW, &texH);
	SDL_Rect dstrect1 = { large_board_button.x , large_board_button.y, texW, large_board_button.h };
	SDL_RenderCopy(renderer, texture_button_1, NULL, &dstrect1);
	SDL_DestroyTexture(texture_button_1);
	SDL_FreeSurface(surface_button_1);

	SDL_Surface* surface_button_2 = TTF_RenderText_Solid(font, "MEDIUM GAME", white_color);
	SDL_Texture* texture_button_2 = SDL_CreateTextureFromSurface(renderer, surface_button_2);
	SDL_QueryTexture(texture_button_2, NULL, NULL, &texW, &texH);
	SDL_Rect dstrect2 = { medium_board_button.x , medium_board_button.y, texW, medium_board_button.h };
	SDL_RenderCopy(renderer, texture_button_2, NULL, &dstrect2);
	SDL_DestroyTexture(texture_button_2);
	SDL_FreeSurface(surface_button_2);

	SDL_Surface* surface_button_3 = TTF_RenderText_Solid(font, "SMALL GAME", white_color);
	SDL_Texture* texture_button_3 = SDL_CreateTextureFromSurface(renderer, surface_button_3);
	SDL_QueryTexture(texture_button_3, NULL, NULL, &texW, &texH);
	SDL_Rect dstrect3 = { small_board_button.x , small_board_button.y, texW, small_board_button.h };
	SDL_RenderCopy(renderer, texture_button_3, NULL, &dstrect3);
	SDL_DestroyTexture(texture_button_3);
	SDL_FreeSurface(surface_button_3);

	SDL_RenderPresent(renderer);
}

void Game::menuDiffRender()
{
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
	SDL_RenderFillRect(renderer, &menu_window);

	SDL_SetRenderDrawColor(renderer, 155, 255, 255, 255);
	SDL_RenderFillRect(renderer, &small_board_button);

	SDL_SetRenderDrawColor(renderer, 155, 255, 255, 255);
	SDL_RenderFillRect(renderer, &medium_board_button);

	SDL_SetRenderDrawColor(renderer, 155, 255, 255, 255);
	SDL_RenderFillRect(renderer, &large_board_button);

	SDL_Color black_color = { 0, 0, 0 };
	SDL_Color white_color = { 255, 255, 255 };
	int texW = 20;
	int texH = 10;

	SDL_Surface* surface_button_1 = TTF_RenderText_Solid(font, "HARD", white_color);
	SDL_Texture* texture_button_1 = SDL_CreateTextureFromSurface(renderer, surface_button_1);
	SDL_QueryTexture(texture_button_1, NULL, NULL, &texW, &texH);
	SDL_Rect dstrect1 = { large_board_button.x , large_board_button.y, texW, large_board_button.h };
	SDL_RenderCopy(renderer, texture_button_1, NULL, &dstrect1);
	SDL_DestroyTexture(texture_button_1);
	SDL_FreeSurface(surface_button_1);

	SDL_Surface* surface_button_2 = TTF_RenderText_Solid(font, "MEDIUM", white_color);
	SDL_Texture* texture_button_2 = SDL_CreateTextureFromSurface(renderer, surface_button_2);
	SDL_QueryTexture(texture_button_2, NULL, NULL, &texW, &texH);
	SDL_Rect dstrect2 = { medium_board_button.x , medium_board_button.y, texW, medium_board_button.h };
	SDL_RenderCopy(renderer, texture_button_2, NULL, &dstrect2);
	SDL_DestroyTexture(texture_button_2);
	SDL_FreeSurface(surface_button_2);

	SDL_Surface* surface_button_3 = TTF_RenderText_Solid(font, "EASY", white_color);
	SDL_Texture* texture_button_3 = SDL_CreateTextureFromSurface(renderer, surface_button_3);
	SDL_QueryTexture(texture_button_3, NULL, NULL, &texW, &texH);
	SDL_Rect dstrect3 = { small_board_button.x , small_board_button.y, texW, small_board_button.h };
	SDL_RenderCopy(renderer, texture_button_3, NULL, &dstrect3);
	SDL_DestroyTexture(texture_button_3);
	SDL_FreeSurface(surface_button_3);

	SDL_RenderPresent(renderer);
}


void Game::gameRender()
{
	SDL_RenderClear(renderer);

	//Bombs && Points Window
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &bombs_window);
	SDL_RenderFillRect(renderer, &points_window);
	
	drawBoard();
	
	if (running() == false)
	{
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		cout << "Kaj ta rozowo kolumna" << endl;
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 150);
		SDL_RenderFillRect(renderer, &end_window);
	}

	SDL_Color black_color = { 0, 0, 0 };
	SDL_Color white_color = { 255, 255, 255 };
	int texW = 20;
	int texH = 10;

	SDL_Surface* surface_message = TTF_RenderText_Solid(font, "punkty: " , black_color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface_message);
	SDL_QueryTexture(texture, NULL, NULL, &points_window.w, &points_window.h);
	SDL_Rect dstrect = { points_window.x, points_window.y, points_window.w, points_window.h };
	SDL_RenderCopy(renderer, texture, NULL, &dstrect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface_message);


	char wt[10];
	_itoa_s(points, wt, 10); // pobieranie informacji o liczbie i zamiana jej na array
	SDL_Surface* surface_message1 = TTF_RenderText_Solid(font, wt, white_color);
	SDL_Texture* texture1 = SDL_CreateTextureFromSurface(renderer, surface_message1);
	SDL_QueryTexture(texture1, NULL, NULL, &texW, &texH);
	SDL_Rect dstrect22 = { points_window.x + points_window.w + 10, points_window.y, texW, points_window.h };
	SDL_RenderCopy(renderer, texture1, NULL, &dstrect22);
	SDL_DestroyTexture(texture1);
	SDL_FreeSurface(surface_message1);
	

	SDL_Surface* surface_bomb = TTF_RenderText_Solid(font, "bomby: ", black_color);
	SDL_Texture* texture_bomb = SDL_CreateTextureFromSurface(renderer, surface_bomb);
	SDL_QueryTexture(texture_bomb, NULL, NULL, &bombs_window.w, &bombs_window.h);
	SDL_Rect dstrect1 = { bombs_window.x, bombs_window.y,bombs_window.w,bombs_window.h };
	SDL_RenderCopy(renderer, texture_bomb, NULL, &dstrect1);
	SDL_DestroyTexture(texture_bomb);
	SDL_FreeSurface(surface_bomb);

	char wt2[10];
	_itoa_s(tab->bombs, wt2, 10);
	SDL_Surface* surface_bomb1 = TTF_RenderText_Solid(font, wt2, white_color);
	SDL_Texture* texture_bomb1 = SDL_CreateTextureFromSurface(renderer, surface_bomb1);
	SDL_QueryTexture(texture_bomb1, NULL, NULL, &texW, &texH);
	SDL_Rect dstrect12 = { bombs_window.x + bombs_window.w + 10, bombs_window.y, texW, bombs_window.h };
	SDL_RenderCopy(renderer, texture_bomb1, NULL, &dstrect12);
	SDL_DestroyTexture(texture_bomb1);
	SDL_FreeSurface(surface_bomb1);


	//Renderowanie elementow rendera
	SDL_RenderPresent(renderer);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}


void Game::setMouseOnYesNo()
{

	for (int y = 0; y < size_y; y++)
	{
		for (int x = 0; x < size_x; x++)
		{
			if (mouse_x >= yes.x && mouse_x < (yes.x + yes.w)) {
				if (mouse_y >= yes.y && mouse_y < (yes.y + yes.h))
				{
					//cout << "YES" << endl;
					play_again = true;
					end_menu = false;
				}
			}
			
			if (mouse_x >= no.x && mouse_x < (no.x + no.w)) {
				if (mouse_y >= no.y && mouse_y < (no.y + no.h))
				{
					//cout << "NO" << endl;
					play_again = false;
					end_menu = false;
				}
			}
		}
	}
}

void Game::setMouseOnDiffButton()
{

	for (int y = 0; y < size_y; y++)
	{
		for (int x = 0; x < size_x; x++)
		{
			if (mouse_x >= large_board_button.x && mouse_x < (large_board_button.x + large_board_button.w)) {
				if (mouse_y >= large_board_button.y && mouse_y < (large_board_button.y + large_board_button.h))
				{
					// hard
					difficulty_menu = false;
					difficulty = 3;
					
				}
			}

			if (mouse_x >= medium_board_button.x && mouse_x < (medium_board_button.x + medium_board_button.w)) {
				if (mouse_y >= medium_board_button.y && mouse_y < (medium_board_button.y + medium_board_button.h))
				{
					// medium
					difficulty_menu = false;
					difficulty = 2;
					
				}
			}


			if (mouse_x >= small_board_button.x && mouse_x < (small_board_button.x + small_board_button.w)) {
				if (mouse_y >= small_board_button.y && mouse_y < (small_board_button.y + small_board_button.h))
				{
					// easy
					difficulty_menu = false;
					difficulty = 1;
					
				}
			}
		}
	}
}

void Game::setMouseOnButton()
{

	for (int y = 0; y < size_y; y++)
	{
		for (int x = 0; x < size_x; x++)
		{
			if (mouse_x >= large_board_button.x && mouse_x < (large_board_button.x + large_board_button.w)) {
				if (mouse_y >= large_board_button.y && mouse_y < (large_board_button.y + large_board_button.h))
				{
					// large button

					size_x = 20;
					size_y = 20;

					points = 0;
					bombs = 40;

					menu = false;
				}
			}

			if (mouse_x >= medium_board_button.x && mouse_x < (medium_board_button.x + medium_board_button.w)) {
				if (mouse_y >= medium_board_button.y && mouse_y < (medium_board_button.y + medium_board_button.h))
				{
					// medium button

					size_x = 20;
					size_y = 10;

					points = 0;
					bombs = 30;

					menu = false;
				}
			}


			if (mouse_x >= small_board_button.x && mouse_x < (small_board_button.x + small_board_button.w)) {
				if (mouse_y >= small_board_button.y && mouse_y < (small_board_button.y + small_board_button.h))
				{
					// small button

					size_x = 10;
					size_y = 10;

					points = 0;
					bombs = 10;

					menu = false;
				}
			}
		}
	}
}

void Game::handleEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT:
		runs = false;
		break;

	case SDL_MOUSEBUTTONDOWN:
		if (event.button.button == SDL_BUTTON_LEFT) {
			SDL_GetMouseState(&mouse_x, &mouse_y);
			//SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		}
		if (event.button.button == SDL_BUTTON_RIGHT) {
			int x;
			int y;
			SDL_GetMouseState(&x, &y);
			setMarkXY(x, y);
			//SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		}
		break;
	}
}
 
void Game::update()
{
	gameTurn();
}

void Game::clean()
{
	//Zakonczenie dzialania Biblioteki
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	cout << "Game cleaned" << endl;
}

void Game::gameTurn() 
{
	cout << "Trun" << endl << endl;
	//tab->showBoard();
	//setPlayerXY();
	setMouseXY();
	checkPlayerXY();

	//cout << "uncovered: " << tab->uncovered_num << "full size: " << (tab->size_x * tab->size_y) << "bombs: " << tab->bombs << endl;
	if (tab->uncovered_num >= ((tab->size_x * tab->size_y) - tab->bombs))
	{
		cout << "LOOK";
		cout << "uncovered: " << tab->uncovered_num << "full size: " << (tab->size_x * tab->size_y) << "bombs: " << tab->bombs << endl;
		setWin();
	}
}

void Game::setWin() 
{
	this->win = true;
	this->runs = false;
	cout << "you win!!!!!!!!" << endl;
}

void Game::winRender()
{
	int texW = 0;
	int texH = 0;

	SDL_Surface* surface_win = TTF_RenderText_Solid(font, "YOU WIN", green_color);
	SDL_Texture* texture_win = SDL_CreateTextureFromSurface(renderer, surface_win);
	SDL_QueryTexture(texture_win, NULL, NULL, &texW, &texH);
	SDL_Rect dstwin = { end_message_window.x , end_message_window.y, texW, texH };
	SDL_RenderCopy(renderer, texture_win, NULL, &dstwin);
	SDL_DestroyTexture(texture_win);
	SDL_FreeSurface(surface_win);

	SDL_RenderPresent(renderer);
}

void Game::loseRender()
{
	int texW = 200;
	int texH = 200;
	
	SDL_Surface* surface_lose = TTF_RenderText_Solid(font, "YOU LOSE", red_color);
	SDL_Texture* texture_lose = SDL_CreateTextureFromSurface(renderer, surface_lose);
	SDL_QueryTexture(texture_lose, NULL, NULL, &texW, &texH);
	SDL_Rect dstlose = { end_message_window.x , end_message_window.y, texW, texH };
	SDL_RenderCopy(renderer, texture_lose, NULL, &dstlose);
	SDL_DestroyTexture(texture_lose);
	SDL_FreeSurface(surface_lose);

	SDL_RenderPresent(renderer);
}

void Game::setLose()
{
	this->lose = true;
	this->runs = false;
}

bool Game::hadWon()
{
	if (this->win == true)
		return true;
	return false;
}

bool Game::hadLost()
{
	if (this->lose == true)
		return true;
	return false;
}

void Game::setPlayerXY() {
	cout << "Podaj x: ";
	cin >> this->player_x;
	cout << "Podaj y: ";
	cin >> this->player_y;
}

void Game::setMouseXY()
{

	for (int y = 0; y < size_y; y++)
	{
		for (int x = 0; x < size_x; x++)
		{
			if (mouse_x >= tab->plansza2d[y][x].rect.x && mouse_x < (tab->plansza2d[y][x].rect.x + tab->plansza2d[y][x].rect.w)) {
				if (mouse_y >= tab->plansza2d[y][x].rect.y && mouse_y < (tab->plansza2d[y][x].rect.y + tab->plansza2d[y][x].rect.h))
				{

					this->player_x = x;
					this->player_y = y;
				
				}
			}
		}
	}
}


void Game::setMarkXY(int mx, int my)
{

	for (int y = 0; y < size_y; y++)
	{
		for (int x = 0; x < size_x; x++)
		{
			if (mx >= tab->plansza2d[y][x].rect.x && mx < (tab->plansza2d[y][x].rect.x + tab->plansza2d[y][x].rect.w)) {
				if (my >= tab->plansza2d[y][x].rect.y && my < (tab->plansza2d[y][x].rect.y + tab->plansza2d[y][x].rect.h))
				{
					if(!tab->plansza2d[y][x].marked())
						tab->plansza2d[y][x].setMark();
					else
						tab->plansza2d[y][x].unMark();
				}
			}
		}
	}
}

void Game::checkPlayerXY()
{

	//Warunek wyjœcia z funkcji
	if (player_x >= tab->size_x || player_x < 0 || player_y >= tab->size_y || player_y < 0 )
	{	
		cout << "Poza plansza, sproboj jeszcze raz!" << endl;
		return;
	}

	if (tab->plansza2d[player_y][player_x].isBoomb())
	{
		cout << "Trafiles na bombe" << endl;
		tab->plansza2d[player_y][player_x].uncoverValue();
		setLose();
		//zaznaczenie trafienia tej bomby na polu
	}
	else if (tab->plansza2d[player_y][player_x].isZero())
	{
		//cout << "Trafiles na puste pole" << endl;
		tab->uncoverNearZeros(player_x, player_y);
	}
	else if(tab->plansza2d[player_y][player_x].value > 0 && tab->plansza2d[player_y][player_x].value < 9 && tab->plansza2d[player_y][player_x].isCovered())
	{
		cout << tab->plansza2d[player_y][player_x].value << endl;
		tab->plansza2d[player_y][player_x].uncoverValue();
		tab->addOneUncover();
		points += tab->plansza2d[player_y][player_x].value;
	}

}

void Game::drawBoard()
{
	for (int y = 0; y < size_y; y++)
	{
		for (int x = 0; x < size_x; x++)
		{
			if (!tab->plansza2d[y][x].isCovered())
			{
				char wt[10];
				_itoa_s(tab->plansza2d[y][x].value, wt, 10); // pobieranie informacji o liczbie i zamiana jej na array

				SDL_SetRenderDrawColor(renderer, tab->plansza2d[y][x].color.r, tab->plansza2d[y][x].color.g, tab->plansza2d[y][x].color.b, 255);
				SDL_RenderFillRect(renderer, &tab->plansza2d[y][x].rect);
			
				if (tab->plansza2d[y][x].value > 0)
				{
					SDL_Color black_color = { 0, 0, 0 };
					SDL_Surface* surface_message = TTF_RenderText_Solid(font, wt, black_color);
					SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface_message);
					
					int w = 0;
					int h = 0;
					//Dzielenie przez dodanie do wymiarów surface i podzielenie przez 2 centruje teks w œrodku komurki
					SDL_QueryTexture(texture, NULL, NULL, &w, &h);
					SDL_Rect dstrect = { (tab->plansza2d[y][x].rect.x + surface_message->w/2) ,  tab->plansza2d[y][x].rect.y , w, h };

					SDL_RenderCopy(renderer, texture, NULL, &dstrect); 

					SDL_DestroyTexture(texture);
					SDL_FreeSurface(surface_message);
				}

				if (!runs && tab->plansza2d[y][x].value == 9)
				{
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
					SDL_RenderDrawRect(renderer, &tab->plansza2d[y][x].rect);	
				}

			}
			else
			{
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				SDL_RenderFillRect(renderer, &tab->plansza2d[y][x].rect);

				if (tab->plansza2d[y][x].marked())
				{
					SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
					SDL_RenderFillRect(renderer, &tab->plansza2d[y][x].rect);
				}

				if (hadLost() && tab->plansza2d[y][x].value == 9)
				{
					SDL_Color red_color = { 255, 0, 0 };
					SDL_Surface* surface_message = TTF_RenderText_Solid(font, " X ", red_color);
					SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface_message);

					int w = 0;
					int h = 0;

					SDL_QueryTexture(texture, NULL, NULL, &w, &h);
					SDL_Rect dstrect = { tab->plansza2d[y][x].rect.x,  tab->plansza2d[y][x].rect.y, w, h };

					SDL_RenderCopy(renderer, texture, NULL, &dstrect);

					SDL_DestroyTexture(texture);
					SDL_FreeSurface(surface_message);
				}
			}
		}
	}
}

void Game::gameOver() 
{
	gameRender();

	if (hadWon())
		winRender();
	else
		loseRender();

	SDL_Delay(3000);
	tab->showBoard();
	cout << "GAME OVER" << endl;
	tab->destroyBoard();
}

bool Game::running(){ return runs; }

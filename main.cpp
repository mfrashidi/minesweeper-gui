#include "SBDL.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>
using namespace std;

//Default variables
const int window_width = 500;
const int window_height = 600;
int table[10][10];
int MINES_LOCATION[10][2];
int SCORE;
int NUMBER_OF_FLAGS;
bool IS_LOST;
bool IS_WON;
bool IS_BW;
int START_TIME;
int TIME_LENGTH = 100;
int SIZE = 10;
int MINES = 10;
bool LOST_BY_TIME;
Texture NORMAL_BLOCK;
Texture FLAG_BLOCK;
Texture NUMBER_BLOCK;
Texture MINE_BLOCK;
Texture MINE_FLAG_BLOCK;
Texture MINE_SELECT_BLOCK;
Texture SELECT_BLOCK;
Texture TIME_TEXTURE;
Texture FLAG_TEXTURE;
Texture EMOJI_TEXTURE;
int EMOJI_X;
int EMOJI_Y;
Sound *SELECT_SOUND;
Sound *FLAG_SOUND;
Sound *UNAVAILABLE_FLAG_SOUND;
Sound *LOST_SOUND;
Sound *WIN_SOUND;
Font *TIME_FONT;
Font *FLAG_FONT;
int MOUSE_X;
int MOUSE_Y;
int CURRENT_SELECT[2]={-1,-1};
int LAST_SELECT[2]={-1,-1};
int REMAINING_TIME;
const int DEFAULT_CHAR = '.';
const char MINE_CHAR = 'X';
const char FLAG_CHAR = 'F';


SDL_Rect getObjectRect(Texture object,int x,int y); //Returns react of an object
void check_select(int x, int y); //Determine which block is the mouse on
void initial(); //Initialize the basics to start the game
bool is_mine(int i,int j); //Check if a block is mine or not
bool is_flag(int i,int j); //Check if a block is flag or not
void print_table(bool showMines=false); //Print the whole table
int mines_nearby(int i, int j); //Returns the number of mines that are around a block
int flags_nearby(int i, int j); //Returns the number of flags that are around a block
void select(); //Select a block
void get_theme(); //Set the theme of the game
void flag(); //Make a block flag
void update_header(); //Update the header information

int main(int argc, char *argv[])
{
	srand(time(0));
	get_theme();

	SBDL::InitEngine("Minesweeper", window_width, window_height);

	const int FPS = 25; //frame per second
	const int delay = 1000 / FPS; //delay we need at each frame
	

	Texture background = SBDL::loadTexture(string("assets/img/")+((IS_BW)?"bw":"colorful")+".png");
	EMOJI_TEXTURE = SBDL::loadTexture("assets/img/smiling.png");
	EMOJI_X = (window_width-EMOJI_TEXTURE.width)/2;
	EMOJI_Y = (100-EMOJI_TEXTURE.height)/2+10;
	
	NORMAL_BLOCK = SBDL::loadTexture("assets/img/normal.png");
	FLAG_BLOCK = SBDL::loadTexture("assets/img/flag.png");
	MINE_FLAG_BLOCK = SBDL::loadTexture("assets/img/bomb_flag.png");
	MINE_BLOCK = SBDL::loadTexture("assets/img/bomb.png");
	MINE_SELECT_BLOCK = SBDL::loadTexture("assets/img/bomb_red.png");
	SELECT_BLOCK = SBDL::loadTexture("assets/img/select.png");
	NUMBER_BLOCK = SBDL::loadTexture("assets/img/1.png");

	TIME_FONT = SBDL::loadFont("assets/fonts/ShareTechMono.ttf", 30);
	FLAG_FONT = SBDL::loadFont("assets/fonts/ShareTechMono.ttf", 40);
	SELECT_SOUND = SBDL::loadSound("assets/sounds/select.mp3");
	FLAG_SOUND = SBDL::loadSound("assets/sounds/flag.mp3");
	LOST_SOUND = SBDL::loadSound("assets/sounds/lost_sound.mp3");
	WIN_SOUND = SBDL::loadSound("assets/sounds/win.mp3");
	UNAVAILABLE_FLAG_SOUND = SBDL::loadSound("assets/sounds/unavailable_flag.mp3");
	initial();
	while (SBDL::isRunning()) {
		if(SCORE==SIZE*SIZE-MINES && !IS_WON){
			EMOJI_TEXTURE = SBDL::loadTexture("assets/img/party.png");
			SBDL::playSound(WIN_SOUND, 1);
			IS_WON=true;
		}
		int startTime = SBDL::getTime();
		SBDL::updateEvents();
		SBDL::clearRenderScreen();
		
		SBDL::showTexture(background, 0, 0);
		
		print_table();
		
		if(!IS_LOST && !IS_WON) check_select(SBDL::Mouse.x,SBDL::Mouse.y);
		
		update_header();

		if(SBDL::Mouse.clicked()){
			MOUSE_X = SBDL::Mouse.x;
			MOUSE_Y = SBDL::Mouse.y;
			if(SBDL::mouseInRect(getObjectRect(EMOJI_TEXTURE,EMOJI_X,EMOJI_Y))){
				initial();
			}else{
				if(!IS_LOST && !IS_WON){
					select();
				}
			}
		}else if(SBDL::Mouse.right_clicked() && !IS_LOST && !IS_WON){
			flag();
		}
		SBDL::updateRenderScreen();
		int elapsedTime = SBDL::getTime() - startTime;
		if (elapsedTime < delay)
			SBDL::delay(delay - elapsedTime);
	}
}

void update_header(){
	SBDL::showTexture(EMOJI_TEXTURE, EMOJI_X, EMOJI_Y);

	REMAINING_TIME = (START_TIME - SBDL::getTime())/1000;
		
	if(REMAINING_TIME==0 && !IS_LOST && !IS_WON){
		IS_LOST = true;
		SBDL::playSound(LOST_SOUND, 1);
		EMOJI_TEXTURE = SBDL::loadTexture("assets/img/crying.png");
	}
	if(IS_LOST || IS_WON) REMAINING_TIME=0;
		
	SBDL::freeTexture(TIME_TEXTURE);
	TIME_TEXTURE = SBDL::createFontTexture(TIME_FONT, to_string(REMAINING_TIME), 0, 0, 0);
	SBDL::showTexture(TIME_TEXTURE, window_width-100,(100-TIME_TEXTURE.height)/2+10);

	SBDL::freeTexture(FLAG_TEXTURE);
	FLAG_TEXTURE = SBDL::createFontTexture(FLAG_FONT, to_string(NUMBER_OF_FLAGS), 0, 0, 0);
	SBDL::showTexture(FLAG_TEXTURE, 50,(100-FLAG_TEXTURE.height)/2+10);
}

SDL_Rect getObjectRect(Texture object,int x,int y){
	SDL_Rect objRect;
	objRect.x = x; 
	objRect.y = y; 
	objRect.w = object.width; 
	objRect.h = object.height;
	return objRect;
}

void check_select(int x, int y){
	CURRENT_SELECT[0]=-1;
	CURRENT_SELECT[1]=-1;
	for(int i=0;i<SIZE;i++){
		for(int j=0;j<SIZE;j++){
			if(40+i*43<=x && x<=73+i*43 && 135+j*43<=y && y<=171+j*43){
				//cout << "(" << i << "," << j << ")" << endl;
				CURRENT_SELECT[0]=i;
				CURRENT_SELECT[1]=j;
			}
		}
	}
	if(CURRENT_SELECT[0]!=-1&&CURRENT_SELECT[1]!=-1){
		if(LAST_SELECT[0]!=CURRENT_SELECT[0] || LAST_SELECT[1]!=CURRENT_SELECT[1]){
			if(table[CURRENT_SELECT[0]][CURRENT_SELECT[1]]!=0) SBDL::playSound(SELECT_SOUND, 1);
			LAST_SELECT[0] = CURRENT_SELECT[0];
			LAST_SELECT[1] = CURRENT_SELECT[1];
		}
		if(table[CURRENT_SELECT[0]][CURRENT_SELECT[1]]!=0){
			SBDL::showTexture(SELECT_BLOCK, 40+CURRENT_SELECT[0]*43, 135+CURRENT_SELECT[1]*43);
		}
	}
}

void initial(){
	EMOJI_TEXTURE = SBDL::loadTexture("assets/img/smiling.png");
	START_TIME = SBDL::getTime()+TIME_LENGTH*1000;
    SCORE = 0;
    NUMBER_OF_FLAGS = 0;
    IS_LOST=false;
    IS_WON=false;
    LOST_BY_TIME = false;
    for(int i=0;i<SIZE;i++){
        for(int j=0;j<SIZE;j++){
            table[i][j]=DEFAULT_CHAR;
        }
    }
    int n = 0;
    int i,j;
    while(n!=MINES){
        i = rand()%(SIZE);
        j = rand()%(SIZE);
        if(table[i][j]==DEFAULT_CHAR){
            //cout << "[" << i << "][" << j << "]" << endl; Prints mines positions
            MINES_LOCATION[n][0]=i;
            MINES_LOCATION[n][1]=j;
            table[i][j] = MINE_CHAR;
            n++;
        }
    }
}
bool is_mine(int i,int j){
    for(int k=0;k<MINES;k++){
        if(MINES_LOCATION[k][0]==i&&MINES_LOCATION[k][1]==j) return true;
    }
    return false;
}
bool is_flag(int i,int j){
    return ((table[i][j]==FLAG_CHAR)?true:false);
}
void print_table(bool showMines){
	int x=40; //33
	int y=135; //36
	for(int i=0;i<SIZE;i++){
		for(int j=0;j<SIZE;j++){
			if(!IS_LOST && !IS_WON){
				if(table[i][j]==DEFAULT_CHAR || table[i][j]==MINE_CHAR){
					SBDL::showTexture(NORMAL_BLOCK, x+i*43, y+j*43);
				}else if(table[i][j]==FLAG_CHAR){
					SBDL::showTexture(FLAG_BLOCK, x+i*43, y+j*43);
				}else{
					if(table[i][j]>0){
						NUMBER_BLOCK = SBDL::loadTexture("assets/img/"+to_string(table[i][j])+((IS_BW)?"_bw":"")+".png");
						SBDL::showTexture(NUMBER_BLOCK, x+i*43, y+j*43);
					}
				}
			}else{
				if(table[i][j]==DEFAULT_CHAR){
					SBDL::showTexture(NORMAL_BLOCK, x+i*43, y+j*43);
				}else if(table[i][j]==FLAG_CHAR){
					if(is_mine(i,j)){
						SBDL::showTexture(MINE_FLAG_BLOCK, x+i*43, y+j*43);
					}else{
						SBDL::showTexture(FLAG_BLOCK, x+i*43, y+j*43);
					}
				}else if(table[i][j]==MINE_CHAR){
					if(i==CURRENT_SELECT[0]&&j==CURRENT_SELECT[1]){
						SBDL::showTexture(MINE_SELECT_BLOCK, x+i*43, y+j*43);
					}else{
						SBDL::showTexture(MINE_BLOCK, x+i*43, y+j*43);
					}
				}else{
					if(table[i][j]>0){
						NUMBER_BLOCK = SBDL::loadTexture("assets/img/"+to_string(table[i][j])+((IS_BW)?"_bw":"")+".png");
						SBDL::showTexture(NUMBER_BLOCK, x+i*43, y+j*43);
					}
				}
			}
		}
	}
}

int mines_nearby(int i, int j){
    int n = 0;
    if(j-1>=0){if(is_mine(i,j-1)) n++;}
    if(j+1<=SIZE-1){if(is_mine(i,j+1)) n++;}
    if(i-1>=0){if(is_mine(i-1,j)) n++;}
    if(i+1<=SIZE-1){if(is_mine(i+1,j)) n++;}
    if(j-1>=0 && i-1>=0){if(is_mine(i-1,j-1)) n++;}
    if(j+1<=SIZE-1 && i+1<=SIZE-1){if(is_mine(i+1,j+1)) n++;}
    if(j+1<=SIZE-1 && i-1>=0){if(is_mine(i-1,j+1)) n++;}
    if(j-1>=0 && i+1<=SIZE-1){if(is_mine(i+1,j-1)) n++;}
    return n;
}

int flags_nearby(int i, int j){
    int n = 0;
    if(j-1>=0){if(is_flag(i,j-1)) n++;}
    if(j+1<=SIZE-1){if(is_flag(i,j+1)) n++;}
    if(i-1>=0){if(is_flag(i-1,j)) n++;}
    if(i+1<=SIZE-1){if(is_flag(i+1,j)) n++;}
    if(j-1>=0 && i-1>=0){if(is_flag(i-1,j-1)) n++;}
    if(j+1<=SIZE-1 && i+1<=SIZE-1){if(is_flag(i+1,j+1)) n++;}
    if(j+1<=SIZE-1 && i-1>=0){if(is_flag(i-1,j+1)) n++;}
    if(j-1>=0 && i+1<=SIZE-1){if(is_flag(i+1,j-1)) n++;}
    return n;
}

void select(){
	int i = CURRENT_SELECT[0];
	int j = CURRENT_SELECT[1];
	if(i!=-1){
		if(table[i][j]==DEFAULT_CHAR){
			int nearMines = mines_nearby(i,j);
			int nearFlags = flags_nearby(i,j);
			if(nearMines==0&&nearFlags==0){
				if(j-1>=0 && table[i][j-1]==DEFAULT_CHAR){table[i][j-1] = mines_nearby(i,j-1);SCORE++;}
				if(j+1<=SIZE-1 && table[i][j+1]==DEFAULT_CHAR){table[i][j+1] = mines_nearby(i,j+1);SCORE++;}
				if(i-1>=0 && table[i-1][j]==DEFAULT_CHAR){table[i-1][j] = mines_nearby(i-1,j);SCORE++;}
				if(i+1<=SIZE-1 && table[i+1][j]==DEFAULT_CHAR){table[i+1][j] = mines_nearby(i+1,j);SCORE++;}
				if(j-1>=0 && i-1>=0 && table[i-1][j-1]==DEFAULT_CHAR){table[i-1][j-1] = mines_nearby(i-1,j-1);SCORE++;}
				if(j+1<=SIZE-1 && i+1<=SIZE-1 && table[i+1][j+1]==DEFAULT_CHAR){table[i+1][j+1] = mines_nearby(i+1,j+1);SCORE++;}
				if(j+1<=SIZE-1 && i-1>=0 && table[i-1][j+1]==DEFAULT_CHAR){table[i-1][j+1] = mines_nearby(i-1,j+1);SCORE++;}
				if(j-1>=0 && i+1<=SIZE-1 && table[i+1][j-1]==DEFAULT_CHAR){table[i+1][j-1] = mines_nearby(i+1,j-1);SCORE++;}
			}
			EMOJI_TEXTURE = SBDL::loadTexture("assets/img/excited.png");
			table[i][j] = nearMines;
			SCORE++;
		}else if(table[i][j]==FLAG_CHAR){
			SBDL::playSound(UNAVAILABLE_FLAG_SOUND,1);
		}else if(table[i][j]==MINE_CHAR){
			IS_LOST = true;
			SBDL::playSound(LOST_SOUND, 1);
			EMOJI_TEXTURE = SBDL::loadTexture("assets/img/crying.png");
		}else{
			EMOJI_TEXTURE = SBDL::loadTexture("assets/img/poker.png");
		}
	}
}
void get_theme(){
	char choice;
	while(true){
		cout << "What theme do you want?\na) Colorful\nb) B/W" << endl;
		cin >> choice;
		if(choice=='a'||choice=='A'){
			IS_BW=false;
			break;
		}
		else if(choice=='b'||choice=='B'){
			IS_BW=true;
			break;
		}

	}
}
void flag(){
	int i = CURRENT_SELECT[0];
	int j = CURRENT_SELECT[1];
	if(i!=-1){
		if(table[i][j]==DEFAULT_CHAR|| table[i][j]==MINE_CHAR){
			if(NUMBER_OF_FLAGS<SIZE){
				table[i][j] = FLAG_CHAR;
				EMOJI_TEXTURE = SBDL::loadTexture("assets/img/thinking.png");
				SBDL::playSound(FLAG_SOUND, 1);
				NUMBER_OF_FLAGS++;
			}else{
				SBDL::playSound(UNAVAILABLE_FLAG_SOUND,1);
			}
		}else if(table[i][j]==FLAG_CHAR){
			if(is_mine(i,j)){
				table[i][j]=MINE_CHAR;
			}else{
				table[i][j]=DEFAULT_CHAR;
			}
			SBDL::playSound(FLAG_SOUND, 1);
			NUMBER_OF_FLAGS--;
		}
	}
}
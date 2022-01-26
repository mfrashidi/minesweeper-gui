#include <iostream>
#include <string.h>
#include "SBDL.h"
#include <fstream>
using namespace std;


Texture MAIN_MENU_BACKGROUND;
Texture LEADERBOARD_BACKGROUND;
Texture SAVED_GAMES_BACKGROUND;
Texture CUSTOM_GAMES_BACKGROUND;
Font *USERNAME_FONT;
Font *FLAG_FONT;
Font *REMAINING_TIME_FONT;
Font *LEADERBOARD_NAME_FONT;
Font *LEADERBOARD_SCORE_FONT;
Font *SAVED_GAME_FONT;
Font *CUSTOM_MODE_FONT;
Texture USERNAME_TEXTURE;
Texture EASY_MODE;
Texture EASY_MODE_SELECTED;
Texture MEDIUM_MODE;
Texture MEDIUM_MODE_SELECTED;
Texture HARD_MODE;
Texture HARD_MODE_SELECTED;
Texture CUSTOM_MODE;
Texture CUSTOM_MODE_SELECTED;
Texture SAVED_GAME_DATA_TEXTURE;
Texture SAVED_GAMES_ICON;
Texture CHANGE_USER_ICON;
Texture LEADERBOARD_ICON;
Texture HEADER;
Texture IN_GAME_BACKGROUND;
Texture USERNAME_BACKGROUND;
Texture REPEAT_GAME_ICON;
Texture RESET_ICON;
Texture BACK_ICON;
Texture UP_ICON;
Texture DOWN_ICON;
Texture RIGHT_ICON;
Texture LEFT_ICON;
Texture PLAY_BUTTON;
Texture PLAY_BUTTON_SELECTED;
Texture SELECTED_BUTTON;
Texture SAVE_GAME_ICON;
Texture NORMAL_BLOCK;
Texture CUSTOM_MODE_DATA_TEXTURE;
Texture FLAG_BLOCK;
Texture NUMBER_BLOCK;
Texture MINE_BLOCK;
Texture MINE_FLAG_BLOCK;
Texture MINE_SELECT_BLOCK;
Texture SELECT_BLOCK;
Texture FLAG_NUMBERS_TEXTURE;
Texture REMAINING_TIME_TEXTURE;
Texture BULLY_MAGUIRE;
Texture LEADERBOARD_NAME_TEXTURE;
Texture LEADERBOARD_SCORE_TEXTURE;
Sound *SELECT_SOUND;
Sound *LOST_SOUND;
Sound *UNAVAILABLE_SOUND;
Sound *FLAG_SOUND;
Sound *WIN_SOUND;
Sound *MENU_SELECT_SOUND;
char USERNAME[11];
std::string USERNAME_STRING;
int USERNAME_LENGTH=15;
int WINDOW_WIDTH = 500;
int WINDOW_HEIGHT = 600;
int SIZE;
int MINES;
bool SHOW_NUMBERS = true;
bool IS_TIMING = false;
int GAME_MODE = 0;
bool IS_LOADED_GAME;
int MINES_LOCATION[399][2];
char table[20][20];
int NUMBER_OF_FLAGS;
int SELECTED_ITEM[2];
bool IS_LOST;
bool IS_WON;
bool REPEAT_GAME = true;
bool START_GAME = true;
int SCORE;
int TIME_LENGTH = 0;
int LAST_SECOND;
int START_TIME;
int row,cols,input;
bool LOST_BY_TIME = false;
string SAVED_DATE;
int SLOT;
int LAST_MENU_SELECT = -1;
string KEY = "DONT EVEN THINK ABOUT IT";

int CURRENT_SELECT[2]={-1,-1};
int LAST_SELECT[2]={-1,-1};

const int DEFAULT_CHAR = '.';
const char MINE_CHAR = 'X';
const char FLAG_CHAR = 'F';

void get_username();
void change_username(char* username, char f, char c='a');
int size_of(char* username);
const std::string get_string(char* USERNAME);
SDL_Rect getObjectRect(Texture object,int x,int y);
void calculate_window_size(int size);
void change_window_size(int width, int height);
void start_game();
void initial();
void print_table(bool showMines=false);
bool is_mine(int i,int j);
bool is_flag(int i,int j);
int mines_nearby(int i, int j);
int flags_nearby(int i, int j);
void check_select(int x, int y);
char int_to_char(int x);
void select();
void select(int i, int j);
void flag();
void add_score(int score);
int get_score(const char* line);
void show_leaderboard();
char* substring(char* s, int begin, int size);
char* substring(const char* s, int begin, int size);
int char_to_int(char* s);
void bubble_sort(int scores[],char* names[], int size);
void get_settings();
void load_game();
bool is_file_exist(string file);
void save_game(int slot=0);
void secure(string file_name);
string encrypt(string msg);
const string current_time();
void load_slot(int slot);
void remove_slot(int slot);

int main(int argc, char *argv[])
{
    srand(time(0));
	SBDL::InitEngine("Minesweeper", WINDOW_WIDTH, WINDOW_HEIGHT);

    get_username();
	MAIN_MENU_BACKGROUND = SBDL::loadTexture("assets/img/main_menu.png");
    EASY_MODE = SBDL::loadTexture("assets/img/easy.png");
    EASY_MODE_SELECTED = SBDL::loadTexture("assets/img/easy_selected.png");
    MEDIUM_MODE = SBDL::loadTexture("assets/img/medium.png");
    MEDIUM_MODE_SELECTED = SBDL::loadTexture("assets/img/medium_selected.png");
    HARD_MODE = SBDL::loadTexture("assets/img/hard.png");
    HARD_MODE_SELECTED = SBDL::loadTexture("assets/img/hard_selected.png");
    CUSTOM_MODE = SBDL::loadTexture("assets/img/custom.png");
    CUSTOM_MODE_SELECTED = SBDL::loadTexture("assets/img/custom_selected.png");
    SAVED_GAMES_ICON = SBDL::loadTexture("assets/img/save.png");
    LEADERBOARD_ICON = SBDL::loadTexture("assets/img/leaderboard_icon.png");
    CHANGE_USER_ICON = SBDL::loadTexture("assets/img/username.png");
    SELECTED_BUTTON = SBDL::loadTexture("assets/img/button_selected.png");

    SBDL::freeSound(MENU_SELECT_SOUND);
	MENU_SELECT_SOUND = SBDL::loadSound("assets/sounds/menu_select.wav");
	

	while (SBDL::isRunning()) {
        WINDOW_HEIGHT = 600;
        WINDOW_WIDTH = 500;
        change_window_size(WINDOW_WIDTH,WINDOW_HEIGHT);
        std::string title = "Minesweeper (";
        title.append(USERNAME);
        title.append(")");
        SDL_SetWindowTitle(SBDL::Core::window,title.c_str());
		SBDL::updateEvents();
		SBDL::clearRenderScreen();

        SBDL::showTexture(MAIN_MENU_BACKGROUND,0,0);
        SBDL::showTexture(EASY_MODE,(WINDOW_WIDTH / 2) - (EASY_MODE.width / 2), 160);
        SBDL::showTexture(MEDIUM_MODE,(WINDOW_WIDTH / 2) - (MEDIUM_MODE.width / 2), 240);
        SBDL::showTexture(HARD_MODE,(WINDOW_WIDTH / 2) - (HARD_MODE.width / 2), 320);
        SBDL::showTexture(CUSTOM_MODE,(WINDOW_WIDTH / 2) - (CUSTOM_MODE.width / 2), 400);
        SBDL::showTexture(CHANGE_USER_ICON,50, 520);
        SBDL::showTexture(SAVED_GAMES_ICON,110, 520);
        SBDL::showTexture(LEADERBOARD_ICON,410, 520);

        if(SBDL::mouseInRect(getObjectRect(EASY_MODE,(WINDOW_WIDTH / 2) - (EASY_MODE.width / 2), 160))){
            SBDL::showTexture(EASY_MODE_SELECTED,(WINDOW_WIDTH / 2) - (EASY_MODE_SELECTED.width / 2), 160);
			if(LAST_MENU_SELECT!=0) SBDL::playSound(MENU_SELECT_SOUND, 1);
            LAST_MENU_SELECT = 0;
            if(SBDL::Mouse.clicked()){
                SIZE = 5;
                MINES = 4;
                SHOW_NUMBERS = true;
                IS_TIMING = false;
                GAME_MODE = 0;
                IS_LOADED_GAME = false;
                start_game();
            }
        }else if(SBDL::mouseInRect(getObjectRect(MEDIUM_MODE,(WINDOW_WIDTH / 2) - (MEDIUM_MODE.width / 2), 240))){
			if(LAST_MENU_SELECT!=1) SBDL::playSound(MENU_SELECT_SOUND, 1);
            LAST_MENU_SELECT = 1;
            SBDL::showTexture(MEDIUM_MODE_SELECTED,(WINDOW_WIDTH / 2) - (MEDIUM_MODE_SELECTED.width / 2), 240);
            if(SBDL::Mouse.clicked()){
                SIZE = 12;
                MINES = 28;
                SHOW_NUMBERS = true;
                IS_TIMING = false;
                GAME_MODE = 1;
                IS_LOADED_GAME = false;
                start_game();
            }
        }else if(SBDL::mouseInRect(getObjectRect(HARD_MODE,(WINDOW_WIDTH / 2) - (HARD_MODE.width / 2), 320))){
			if(LAST_MENU_SELECT!=2) SBDL::playSound(MENU_SELECT_SOUND, 1);
            LAST_MENU_SELECT = 2;
            SBDL::showTexture(HARD_MODE_SELECTED,(WINDOW_WIDTH / 2) - (HARD_MODE_SELECTED.width / 2), 320);
            if(SBDL::Mouse.clicked()){
                SIZE = 20;
                MINES = 96;
                SHOW_NUMBERS = true;
                IS_TIMING = false;
                GAME_MODE = 2;
                IS_LOADED_GAME = false;
                start_game();
            }
        }else if(SBDL::mouseInRect(getObjectRect(CUSTOM_MODE,(WINDOW_WIDTH / 2) - (CUSTOM_MODE.width / 2), 400))){
            SBDL::showTexture(CUSTOM_MODE_SELECTED,(WINDOW_WIDTH / 2) - (CUSTOM_MODE_SELECTED.width / 2), 400);
            if(LAST_MENU_SELECT!=3) SBDL::playSound(MENU_SELECT_SOUND, 1);
            LAST_MENU_SELECT = 3;
            if(SBDL::Mouse.clicked()) get_settings();
        }else if(SBDL::mouseInRect(getObjectRect(CHANGE_USER_ICON,50,520))&& SBDL::Mouse.clicked()){
            get_username();
        }else if(SBDL::mouseInRect(getObjectRect(LEADERBOARD_ICON,410,520))&& SBDL::Mouse.clicked()){
            show_leaderboard();
        }else if(SBDL::mouseInRect(getObjectRect(SAVED_GAMES_ICON,110,520))&& SBDL::Mouse.clicked()){
            load_game();
        }


		SBDL::updateRenderScreen();

		SBDL::delay(30);
	}

	return 0;
}
SDL_Rect getObjectRect(Texture object,int x,int y){
	SDL_Rect objRect;
	objRect.x = x; 
	objRect.y = y; 
	objRect.w = object.width; 
	objRect.h = object.height;
	return objRect;
}
void get_username(){

    SBDL::freeTexture(USERNAME_BACKGROUND);
    USERNAME_BACKGROUND = SBDL::loadTexture("assets/img/get_username.png");
	USERNAME_FONT = SBDL::loadFont("assets/fonts/ProductSans.ttf", 20);

    while (SBDL::isRunning()) {
		SBDL::updateEvents();
		SBDL::clearRenderScreen();

        SDL_SetWindowTitle(SBDL::Core::window,"Set Username");

        SBDL::showTexture(USERNAME_BACKGROUND, 0, 0);

        if(SBDL::keyPressed(SDL_SCANCODE_A)) change_username(USERNAME,'a','a');
        else if(SBDL::keyPressed(SDL_SCANCODE_B)) change_username(USERNAME,'a','b');
        else if(SBDL::keyPressed(SDL_SCANCODE_C)) change_username(USERNAME,'a','c');
        else if(SBDL::keyPressed(SDL_SCANCODE_D)) change_username(USERNAME,'a','d');
        else if(SBDL::keyPressed(SDL_SCANCODE_E)) change_username(USERNAME,'a','e');
        else if(SBDL::keyPressed(SDL_SCANCODE_F)) change_username(USERNAME,'a','f');
        else if(SBDL::keyPressed(SDL_SCANCODE_G)) change_username(USERNAME,'a','g');
        else if(SBDL::keyPressed(SDL_SCANCODE_H)) change_username(USERNAME,'a','h');
        else if(SBDL::keyPressed(SDL_SCANCODE_I)) change_username(USERNAME,'a','i');
        else if(SBDL::keyPressed(SDL_SCANCODE_J)) change_username(USERNAME,'a','j');
        else if(SBDL::keyPressed(SDL_SCANCODE_K)) change_username(USERNAME,'a','k');
        else if(SBDL::keyPressed(SDL_SCANCODE_L)) change_username(USERNAME,'a','l');
        else if(SBDL::keyPressed(SDL_SCANCODE_M)) change_username(USERNAME,'a','m');
        else if(SBDL::keyPressed(SDL_SCANCODE_N)) change_username(USERNAME,'a','n');
        else if(SBDL::keyPressed(SDL_SCANCODE_O)) change_username(USERNAME,'a','o');
        else if(SBDL::keyPressed(SDL_SCANCODE_P)) change_username(USERNAME,'a','p');
        else if(SBDL::keyPressed(SDL_SCANCODE_Q)) change_username(USERNAME,'a','q');
        else if(SBDL::keyPressed(SDL_SCANCODE_R)) change_username(USERNAME,'a','r');
        else if(SBDL::keyPressed(SDL_SCANCODE_S)) change_username(USERNAME,'a','s');
        else if(SBDL::keyPressed(SDL_SCANCODE_T)) change_username(USERNAME,'a','t');
        else if(SBDL::keyPressed(SDL_SCANCODE_U)) change_username(USERNAME,'a','u');
        else if(SBDL::keyPressed(SDL_SCANCODE_V)) change_username(USERNAME,'a','v');
        else if(SBDL::keyPressed(SDL_SCANCODE_W)) change_username(USERNAME,'a','w');
        else if(SBDL::keyPressed(SDL_SCANCODE_X)) change_username(USERNAME,'a','x');
        else if(SBDL::keyPressed(SDL_SCANCODE_Y)) change_username(USERNAME,'a','y');
        else if(SBDL::keyPressed(SDL_SCANCODE_Z)) change_username(USERNAME,'a','z');
        else if(SBDL::keyPressed(SDL_SCANCODE_Z)) change_username(USERNAME,'a','z');
        else if(SBDL::keyPressed(SDL_SCANCODE_BACKSPACE)&&size_of(USERNAME)>0) change_username(USERNAME,'r');
        else if(SBDL::keyPressed(SDL_SCANCODE_RETURN)&&size_of(USERNAME)>0) break;
		SBDL::freeTexture(USERNAME_TEXTURE);
        if(size_of(USERNAME)>0){
		    USERNAME_TEXTURE = SBDL::createFontTexture(USERNAME_FONT, USERNAME_STRING, 0, 0, 0);
	        SBDL::showTexture(USERNAME_TEXTURE, (WINDOW_WIDTH / 2) - (USERNAME_TEXTURE.width / 2), (WINDOW_HEIGHT / 2) - (USERNAME_TEXTURE.height / 2));
        }

		SBDL::updateRenderScreen();

		SBDL::delay(30);
	}
}

void change_username(char* username, char f, char c){
    int size = size_of(username);
    if(f=='a'){
        if(size<USERNAME_LENGTH){
            username[size] = c;
            username[size+1] = '\0';
        }
    }else if(f=='r'){
        username[size-1] = '\0';
    }
    USERNAME_STRING = USERNAME;
}
int size_of(char* username){
    for(int i=0;i<USERNAME_LENGTH+1;i++) if(username[i]=='\0') return i;
    return 0;
}

void change_window_size(int width, int height){
	SDL_SetWindowSize(SBDL::Core::window,width,height);
    SDL_RenderSetLogicalSize(SBDL::Core::renderer,width,height);
}

void initial(){
    SCORE = 0;
    NUMBER_OF_FLAGS = 0;
    IS_LOST=false;
    IS_WON=false;
    row = -1;
    cols = -1;
    input = -1;
    if(IS_TIMING){
        START_TIME = time(0)+TIME_LENGTH;
        LOST_BY_TIME = false;
    }
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
            MINES_LOCATION[n][0]=i;
            MINES_LOCATION[n][1]=j;
            table[i][j] = MINE_CHAR;
            n++;
        }
    }
    SELECTED_ITEM[0] = SELECTED_ITEM[1] = SIZE/2;
    calculate_window_size(SIZE);
}

void start_game(){
    if(!IS_LOADED_GAME) initial();
    if(IS_LOADED_GAME) calculate_window_size(SIZE);

    SBDL::freeTexture(IN_GAME_BACKGROUND);
    IN_GAME_BACKGROUND = SBDL::loadTexture("assets/img/game.png");
    SBDL::freeTexture(HEADER);
    HEADER = SBDL::loadTexture("assets/img/header.png");
    SBDL::freeTexture(REPEAT_GAME_ICON);
    REPEAT_GAME_ICON = SBDL::loadTexture("assets/img/restart.png");
    SBDL::freeTexture(BACK_ICON);
    BACK_ICON = SBDL::loadTexture("assets/img/back.png");
    SBDL::freeTexture(SAVE_GAME_ICON);
    SAVE_GAME_ICON = SBDL::loadTexture("assets/img/save.png");

    SBDL::freeTexture(NORMAL_BLOCK);
    NORMAL_BLOCK = SBDL::loadTexture("assets/img/normal.png");
    SBDL::freeTexture(FLAG_BLOCK);
	FLAG_BLOCK = SBDL::loadTexture("assets/img/flag.png");
    SBDL::freeTexture(MINE_FLAG_BLOCK);
	MINE_FLAG_BLOCK = SBDL::loadTexture("assets/img/bomb_flag.png");
    SBDL::freeTexture(MINE_BLOCK);
	MINE_BLOCK = SBDL::loadTexture("assets/img/bomb.png");
    SBDL::freeTexture(MINE_SELECT_BLOCK);
	MINE_SELECT_BLOCK = SBDL::loadTexture("assets/img/bomb_red.png");
    SBDL::freeTexture(SELECT_BLOCK);
	SELECT_BLOCK = SBDL::loadTexture("assets/img/select.png");
    SBDL::freeTexture(NUMBER_BLOCK);
	NUMBER_BLOCK = SBDL::loadTexture("assets/img/1.png");

    SBDL::freeSound(WIN_SOUND);
	WIN_SOUND = SBDL::loadSound("assets/sounds/win.mp3");
    SBDL::freeSound(LOST_SOUND);
	LOST_SOUND = SBDL::loadSound("assets/sounds/lost_sound.mp3");
	SBDL::freeSound(UNAVAILABLE_SOUND);
	UNAVAILABLE_SOUND = SBDL::loadSound("assets/sounds/unavailable_flag.mp3");

	FLAG_FONT = SBDL::loadFont("assets/fonts/ShareTechMono.ttf", 30);
	REMAINING_TIME_FONT = SBDL::loadFont("assets/fonts/ShareTechMono.ttf", 30);

    std::string title;
    if(IS_LOADED_GAME){
        title.append("Save Slot #");
        title.append(to_string(SLOT));
        title.append(" - ");
        title.append(USERNAME);
    }else{
        if(GAME_MODE==0) title.append("Easy");
        if(GAME_MODE==1) title.append("Medium");
        if(GAME_MODE==2) title.append("Hard");
        if(GAME_MODE==3) title.append("Custom");
    }
    SDL_SetWindowTitle(SBDL::Core::window,title.c_str());

    int i=0;
    while (SBDL::isRunning()) {
		SBDL::updateEvents();
		SBDL::clearRenderScreen();
        SBDL::showTexture(IN_GAME_BACKGROUND, 0, 0);
        SBDL::showTexture(HEADER,(WINDOW_WIDTH / 2) - (HEADER.width / 2), 25);
        if(!IS_LOADED_GAME){
            SBDL::showTexture(BACK_ICON,(WINDOW_WIDTH / 2) - ((HEADER.width-300) / 2), 50);
            SBDL::showTexture(REPEAT_GAME_ICON,(WINDOW_WIDTH / 2)- (REPEAT_GAME_ICON.width / 2), 50);
            SBDL::showTexture(SAVE_GAME_ICON,((WINDOW_WIDTH/2) - (REPEAT_GAME_ICON.width)) + ((HEADER.width-300) / 2), 50);
        }else{
            SBDL::showTexture(BACK_ICON,(WINDOW_WIDTH / 2)- (BACK_ICON.width / 2), 50);
        }

        print_table();
        if(!IS_LOST && !IS_WON) check_select(SBDL::Mouse.x,SBDL::Mouse.y);

        SBDL::freeTexture(FLAG_NUMBERS_TEXTURE);
        FLAG_NUMBERS_TEXTURE = SBDL::createFontTexture(FLAG_FONT, to_string(MINES-NUMBER_OF_FLAGS), 0, 0, 0);
	    SBDL::showTexture(FLAG_NUMBERS_TEXTURE, (WINDOW_WIDTH / 2) - (HEADER.width / 2) + 30 + 35 - (FLAG_NUMBERS_TEXTURE.width/2), 52);

        SBDL::freeTexture(REMAINING_TIME_TEXTURE);
        if(IS_TIMING){
            if(START_TIME-time(0)<=0 && !IS_LOST && !IS_WON){
                IS_LOST = true;
                LAST_SECOND = START_TIME-time(0);
                if(IS_LOADED_GAME) remove_slot(SLOT);
                SBDL::playSound(LOST_SOUND, 1);
            }
            if(IS_WON || IS_LOST) REMAINING_TIME_TEXTURE = SBDL::createFontTexture(REMAINING_TIME_FONT, to_string(LAST_SECOND), 0, 0, 0);
            else REMAINING_TIME_TEXTURE = SBDL::createFontTexture(REMAINING_TIME_FONT, to_string((START_TIME-time(0))), 0, 0, 0);
        }else REMAINING_TIME_TEXTURE = SBDL::createFontTexture(REMAINING_TIME_FONT, "-", 0, 0, 0);
	    SBDL::showTexture(REMAINING_TIME_TEXTURE, (WINDOW_WIDTH/2)+177-REMAINING_TIME_TEXTURE.width/2, 52);

        if(IS_WON && i<290){
            SBDL::freeTexture(BULLY_MAGUIRE);
            std::string name = "assets/img/tobey/frame_";
            if(i<10){
                name.append("00");
                name.append(to_string(i));
            }else if(i<100){
                name.append("0");
                name.append(to_string(i));
            }else name.append(to_string(i));
            name.append("_delay-0.03s.jpg");
            BULLY_MAGUIRE = SBDL::loadTexture(name.c_str());
            SBDL::showTexture(BULLY_MAGUIRE,(WINDOW_WIDTH / 2) - (BULLY_MAGUIRE.width / 2)-50,WINDOW_HEIGHT-BULLY_MAGUIRE.height);
            i++;
        }


        if(IS_LOADED_GAME && !IS_WON && !IS_LOST && !LOST_BY_TIME) save_game(SLOT);

        SBDL::updateRenderScreen();

        if(SBDL::mouseInRect(getObjectRect(BACK_ICON,(WINDOW_WIDTH / 2) - ((IS_LOADED_GAME)?(BACK_ICON.width / 2):((HEADER.width-300) / 2)),50))){
            SBDL::showTexture(SELECTED_BUTTON,(WINDOW_WIDTH / 2) - ((IS_LOADED_GAME)?(BACK_ICON.width / 2):((HEADER.width-300) / 2)),50);
            if(SBDL::Mouse.clicked()){
                break;
            }
        }else if(SBDL::mouseInRect(getObjectRect(REPEAT_GAME_ICON,(WINDOW_WIDTH / 2)-(REPEAT_GAME_ICON.width / 2),50))&& SBDL::Mouse.clicked() && !IS_LOADED_GAME){
            start_game();
            break;
        }else if(SBDL::mouseInRect(getObjectRect(SAVE_GAME_ICON,((WINDOW_WIDTH/2) - (REPEAT_GAME_ICON.width)) + ((HEADER.width-300) / 2),50))&& SBDL::Mouse.clicked() && !IS_LOADED_GAME && !IS_WON && !IS_LOST){
            save_game();
            break;
        }else if(!IS_LOST && !IS_WON && SBDL::Mouse.clicked()){
			select();
		}else if(SBDL::Mouse.right_clicked() && !IS_LOST && !IS_WON){
			flag();
		}
        if(SCORE==SIZE*SIZE-MINES && !IS_WON){
            add_score(MINES+((IS_TIMING)?(START_TIME-time(0))/5:0));
			SBDL::playSound(WIN_SOUND, 1);
			IS_WON=true;
            if(IS_LOADED_GAME) remove_slot(SLOT);
            if(IS_TIMING) LAST_SECOND = START_TIME-time(0);
		}

		SBDL::delay(30);
    }
}

int get_score(const char* line){
    int score=0;
    int i=0;
    while(true){
        if(line[i]=='\n'||line[i]=='\0') break;
        if(line[i]>='0'&&line[i]<='9'){
            score *= 10;
            score += (int)line[i]-48;
        }
        i++;
    }
    return score;
}

void add_score(int score){
    ifstream read_backup("assets/data/leaderboard.txt");
    ofstream write_backup("assets/data/tmp");
    string line;
    bool is_found = false;
    while(getline(read_backup,line)){
        if(strstr(line.c_str(),USERNAME)){
            write_backup << USERNAME << "=" << get_score(line.c_str())+score << '\n';
            is_found = true;
        }else{
            write_backup << line << '\n';
        }
    }
    if(!is_found) write_backup << USERNAME << "=" << get_score(line.c_str())+score << '\n';
    write_backup.close();
    read_backup.close();

    ifstream read("assets/data/tmp");
    ofstream write("assets/data/leaderboard.txt");
    while(getline(read,line)){
        write << line << '\n';
    }
    write.close();
    read.close();
    remove("assets/data/tmp");
}

void print_table(bool showMines){
	int x=(WINDOW_WIDTH/2)-((SIZE*33+(SIZE-1)*10)/2);
	int y= 120 + ((WINDOW_HEIGHT-120)/2)-((SIZE*37+(SIZE-1)*7)/2);
	for(int i=0;i<SIZE;i++){
		for(int j=0;j<SIZE;j++){
			if(!IS_LOST && !IS_WON){
				if(table[i][j]==DEFAULT_CHAR || table[i][j]==MINE_CHAR){
					SBDL::showTexture(NORMAL_BLOCK, x+i*43, y+j*43);
				}else if(table[i][j]==FLAG_CHAR){
					SBDL::showTexture(FLAG_BLOCK, x+i*43, y+j*43);
				}else{
					if(table[i][j]>0){
                        SBDL::freeTexture(NUMBER_BLOCK);
						NUMBER_BLOCK = SBDL::loadTexture("assets/img/"+to_string((int)table[i][j]-48)+".png");
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
                        SBDL::freeTexture(NUMBER_BLOCK);
						NUMBER_BLOCK = SBDL::loadTexture("assets/img/"+to_string((int)table[i][j]-48)+".png");
						SBDL::showTexture(NUMBER_BLOCK, x+i*43, y+j*43);
					}
				}
			}
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

void calculate_window_size(int size){
    if(size>10){
	    WINDOW_WIDTH = (SIZE*33+(SIZE-1)*10) + 50;
	    WINDOW_HEIGHT = (SIZE*37+(SIZE-1)*7) + 150;
    }else{
        WINDOW_WIDTH = 500;
        WINDOW_HEIGHT = 600;
    }
    change_window_size(WINDOW_WIDTH,WINDOW_HEIGHT);
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
void check_select(int x, int y){
	CURRENT_SELECT[0]=-1;
	CURRENT_SELECT[1]=-1;

	int START_X =(WINDOW_WIDTH/2)-((SIZE*33+(SIZE-1)*10)/2);
	int START_Y = 120 + ((WINDOW_HEIGHT-120)/2)-((SIZE*37+(SIZE-1)*7)/2);
    
    SBDL::freeSound(SELECT_SOUND);
    SELECT_SOUND = SBDL::loadSound("assets/sounds/select.mp3");

	for(int i=0;i<SIZE;i++){
		for(int j=0;j<SIZE;j++){
			if(START_X+i*43<=x && x<=START_X+33+i*43 && START_Y+j*43<=y && y<=START_Y+36+j*43){
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
            SBDL::freeTexture(SELECT_BLOCK);
            SELECT_BLOCK = SBDL::loadTexture("assets/img/select.png");
			SBDL::showTexture(SELECT_BLOCK, START_X+CURRENT_SELECT[0]*43, START_Y+CURRENT_SELECT[1]*43);
		}
	}
}

void select(){
	int i = CURRENT_SELECT[0];
	int j = CURRENT_SELECT[1];

	if(i!=-1){
		if(table[i][j]==DEFAULT_CHAR){
			int nearMines = mines_nearby(i,j);
            int nearFlags = flags_nearby(i,j);
            table[i][j] = int_to_char(nearMines);
            if(nearMines==0&&nearFlags==0){
                if(j-1>=0 && table[i][j-1]==DEFAULT_CHAR){
                    if(mines_nearby(i,j-1)==0) select(i,j-1);
                    if(table[i][j-1]==DEFAULT_CHAR){
                        table[i][j-1] = int_to_char(mines_nearby(i,j-1));
                        SCORE++;
                    }
                }if(j+1<=SIZE-1 && table[i][j+1]==DEFAULT_CHAR){
                    if(mines_nearby(i,j+1)==0) select(i,j+1);
                    if(table[i][j+1]==DEFAULT_CHAR){
                        table[i][j+1] = int_to_char(mines_nearby(i,j+1));
                        SCORE++;
                    }
                }if(i-1>=0 && table[i-1][j]==DEFAULT_CHAR){
                    if(mines_nearby(i-1,j)==0) select(i-1,j);
                    if(table[i-1][j]==DEFAULT_CHAR){
                        table[i-1][j] = int_to_char(mines_nearby(i-1,j));
                        SCORE++;
                    }
                }if(i+1<=SIZE-1 && table[i+1][j]==DEFAULT_CHAR){
                    if(mines_nearby(i+1,j)==0) select(i+1,j);
                    if(table[i+1][j]==DEFAULT_CHAR){
                        table[i+1][j] = int_to_char(mines_nearby(i+1,j));
                        SCORE++;
                    }
                }if(j-1>=0 && i-1>=0 && table[i-1][j-1]==DEFAULT_CHAR){
                    if(mines_nearby(i-1,j-1)==0) select(i-1,j-1);
                    if(table[i-1][j-1]==DEFAULT_CHAR){
                        table[i-1][j-1] = int_to_char(mines_nearby(i-1,j-1));
                        SCORE++;
                    }
                }if(j+1<=SIZE-1 && i+1<=SIZE-1 && table[i+1][j+1]==DEFAULT_CHAR){
                    if(mines_nearby(i+1,j+1)==0) select(i+1,j+1);
                    if(table[i+1][j+1]==DEFAULT_CHAR){
                        table[i+1][j+1] = int_to_char(mines_nearby(i+1,j+1));
                        SCORE++;
                    }
                }if(j+1<=SIZE-1 && i-1>=0 && table[i-1][j+1]==DEFAULT_CHAR){
                    if(mines_nearby(i-1,j+1)==0) select(i-1,j+1);
                    if(table[i-1][j+1]==DEFAULT_CHAR){
                        table[i-1][j+1] = int_to_char(mines_nearby(i-1,j+1));
                        SCORE++;
                    }
                }if(j-1>=0 && i+1<=SIZE-1 && table[i+1][j-1]==DEFAULT_CHAR){
                    if(mines_nearby(i+1,j-1)==0) select(i+1,j-1);
                    if(table[i+1][j-1]==DEFAULT_CHAR){
                        table[i+1][j-1] = int_to_char(mines_nearby(i+1,j-1));
                        SCORE++;
                    }
                }
            }
            SCORE++;
		}else if(table[i][j]==FLAG_CHAR){
			SBDL::playSound(UNAVAILABLE_SOUND,1);
		}else if(table[i][j]==MINE_CHAR){
			IS_LOST = true;
            if(IS_LOADED_GAME) remove_slot(SLOT);
            if(IS_TIMING) LAST_SECOND=START_TIME-time(0);
			SBDL::playSound(LOST_SOUND, 1);
		}else{
			SBDL::playSound(UNAVAILABLE_SOUND,1);
		}
	}
}
char int_to_char(int x){
    char a = 48+x;
    return ((a==0)?'-':a);
}
char* substring(const char* s, int begin, int size){ 
    char* news = new char[size+1];
    for (int i=0, j = begin; i < size; ++i, j++)
        news[i] = s[j]; 
    news[size] = '\0';
    return news;
}
void select(int i, int j){
    if(table[i][j]==DEFAULT_CHAR){
        int nearMines = mines_nearby(i,j);
        int nearFlags = flags_nearby(i,j);
        table[i][j] = int_to_char(nearMines);
        if(nearMines==0&&nearFlags==0){
            if(j-1>=0 && table[i][j-1]==DEFAULT_CHAR){
                if(mines_nearby(i,j-1)==0) select(i,j-1);
                if(table[i][j-1]==DEFAULT_CHAR){
                    table[i][j-1] = int_to_char(mines_nearby(i,j-1));
                    SCORE++;
                }
            }if(j+1<=SIZE-1 && table[i][j+1]==DEFAULT_CHAR){
                if(mines_nearby(i,j+1)==0) select(i,j+1);
                if(table[i][j+1]==DEFAULT_CHAR){
                    table[i][j+1] = int_to_char(mines_nearby(i,j+1));
                    SCORE++;
                }
            }if(i-1>=0 && table[i-1][j]==DEFAULT_CHAR){
                if(mines_nearby(i-1,j)==0) select(i-1,j);
                if(table[i-1][j]==DEFAULT_CHAR){
                    table[i-1][j] = int_to_char(mines_nearby(i-1,j));
                    SCORE++;
                }
            }if(i+1<=SIZE-1 && table[i+1][j]==DEFAULT_CHAR){
                if(mines_nearby(i+1,j)==0) select(i+1,j);
                if(table[i+1][j]==DEFAULT_CHAR){
                    table[i+1][j] = int_to_char(mines_nearby(i+1,j));
                    SCORE++;
                }
            }if(j-1>=0 && i-1>=0 && table[i-1][j-1]==DEFAULT_CHAR){
                if(mines_nearby(i-1,j-1)==0) select(i-1,j-1);
                if(table[i-1][j-1]==DEFAULT_CHAR){
                    table[i-1][j-1] = int_to_char(mines_nearby(i-1,j-1));
                    SCORE++;
                }
            }if(j+1<=SIZE-1 && i+1<=SIZE-1 && table[i+1][j+1]==DEFAULT_CHAR){
                if(mines_nearby(i+1,j+1)==0) select(i+1,j+1);
                if(table[i+1][j+1]==DEFAULT_CHAR){
                    table[i+1][j+1] = int_to_char(mines_nearby(i+1,j+1));
                    SCORE++;
                }
            }if(j+1<=SIZE-1 && i-1>=0 && table[i-1][j+1]==DEFAULT_CHAR){
                if(mines_nearby(i-1,j+1)==0) select(i-1,j+1);
                if(table[i-1][j+1]==DEFAULT_CHAR){
                    table[i-1][j+1] = int_to_char(mines_nearby(i-1,j+1));
                    SCORE++;
                }
            }if(j-1>=0 && i+1<=SIZE-1 && table[i+1][j-1]==DEFAULT_CHAR){
                if(mines_nearby(i+1,j-1)==0) select(i+1,j-1);
                if(table[i+1][j-1]==DEFAULT_CHAR){
                    table[i+1][j-1] = int_to_char(mines_nearby(i+1,j-1));
                    SCORE++;
                }
            }
        }
        SCORE++;
    }
}
void flag(){
	int i = CURRENT_SELECT[0];
	int j = CURRENT_SELECT[1];

	SBDL::freeSound(UNAVAILABLE_SOUND);
	UNAVAILABLE_SOUND = SBDL::loadSound("assets/sounds/unavailable_flag.mp3");
	SBDL::freeSound(FLAG_SOUND);
	FLAG_SOUND = SBDL::loadSound("assets/sounds/flag.mp3");

	if(i!=-1){
		if(table[i][j]==DEFAULT_CHAR|| table[i][j]==MINE_CHAR){
			if(NUMBER_OF_FLAGS<MINES){
				table[i][j] = FLAG_CHAR;
				SBDL::playSound(FLAG_SOUND, 1);
				NUMBER_OF_FLAGS++;
			}else{
				SBDL::playSound(UNAVAILABLE_SOUND,1);
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

int char_to_int(char* s){
    int number = 0;
    int i = 0;
    while(true){
        if(s[i]=='\0') break;
        number*=10;
        number+=(int)s[i]-48;
        i++;
    }
    return number;
}
void bubble_sort(int scores[],char* names[], int size){
    for(int i=0;i<size-1;i++)
        for(int j=0;j<size-i-1;j++)
            if(scores[j]<scores[j+1]){
                int tmp = scores[j];
                scores[j] = scores[j+1];
                scores[j+1] = tmp;
                char* tmpName = names[j];
                names[j] = names[j+1];
                names[j+1] = tmpName;
            }
}

void show_leaderboard(){
    int n = 0;
    int i = 0;
    string line;
    ifstream read("assets/data/leaderboard.txt");
    while(getline(read,line)) n++;
    read.clear();
    read.seekg(0, read.beg);    
    char* names[n];
    int scores[n];
    while(getline(read,line)){
        names[i] = substring(line.c_str(),0,strchr(line.c_str(), '=')-line.c_str());
        scores[i] = char_to_int(substring(line.c_str(),strchr(line.c_str(), '=')-line.c_str()+1,strlen(line.c_str())));
        i++;
    }
    bubble_sort(scores,names,n);

    SBDL::freeTexture(LEADERBOARD_BACKGROUND);
    if(n!=0) LEADERBOARD_BACKGROUND = SBDL::loadTexture("assets/img/leaderboard.png");
    else LEADERBOARD_BACKGROUND = SBDL::loadTexture("assets/img/leaderboard_empty.png");

	LEADERBOARD_NAME_FONT = SBDL::loadFont("assets/fonts/ShareTechMono.ttf", 25);
	LEADERBOARD_SCORE_FONT = SBDL::loadFont("assets/fonts/ShareTechMono.ttf", 25);

    SBDL::freeTexture(BACK_ICON);
    BACK_ICON = SBDL::loadTexture("assets/img/back.png");

    SBDL::freeTexture(RESET_ICON);
    RESET_ICON = SBDL::loadTexture("assets/img/restart.png");

    while (SBDL::isRunning()) {
		SBDL::updateEvents();
		SBDL::clearRenderScreen();

        SDL_SetWindowTitle(SBDL::Core::window,"Leaderboard");

        SBDL::showTexture(LEADERBOARD_BACKGROUND,0,0);
        SBDL::showTexture(BACK_ICON,60,80);
        SBDL::showTexture(RESET_ICON,410,520);

        if(SBDL::mouseInRect(getObjectRect(BACK_ICON,60,80))){
            SBDL::showTexture(SELECTED_BUTTON,60,80);
            if(SBDL::Mouse.clicked()){
                break;
            }
        }
        if(SBDL::mouseInRect(getObjectRect(RESET_ICON,410,520))){
            SBDL::showTexture(SELECTED_BUTTON,410,520);
            if(SBDL::Mouse.clicked()){
                remove("assets/data/leaderboard.txt");
                break;
            }
        }
        if(n!=0){
            for(int i=0;i<((n<3)?n:3);i++){
                SBDL::freeTexture(LEADERBOARD_NAME_TEXTURE);
                LEADERBOARD_NAME_TEXTURE = SBDL::createFontTexture(LEADERBOARD_NAME_FONT, names[i], 255, 255, 255);
	            SBDL::showTexture(LEADERBOARD_NAME_TEXTURE, 160,225+25-LEADERBOARD_NAME_TEXTURE.height/2+i*80);


                SBDL::freeTexture(LEADERBOARD_SCORE_TEXTURE);
                LEADERBOARD_SCORE_TEXTURE = SBDL::createFontTexture(LEADERBOARD_SCORE_FONT, to_string(scores[i]), 0, 0, 0);
	            SBDL::showTexture(LEADERBOARD_SCORE_TEXTURE, 345-LEADERBOARD_SCORE_TEXTURE.width,225+25-LEADERBOARD_SCORE_TEXTURE.height/2+i*80);
            }
        }
        SBDL::updateRenderScreen();

		SBDL::delay(30);
    }
}

bool is_file_exist(string file){
    std::ifstream infile(file);
    return infile.good();
}

void load_game(){
    int slots = 0;
    while(is_file_exist(string("assets/data/")+string(USERNAME)+string("_save_")+to_string(slots+1)+string(".txt"))){
        slots++;
    }
    int slot = 1;
    SBDL::freeTexture(BACK_ICON);
    BACK_ICON = SBDL::loadTexture("assets/img/back.png");


    SBDL::freeTexture(UP_ICON);
    UP_ICON = SBDL::loadTexture("assets/img/up.png");


    SBDL::freeTexture(DOWN_ICON);
    DOWN_ICON = SBDL::loadTexture("assets/img/down.png");

    SBDL::freeTexture(PLAY_BUTTON);
    PLAY_BUTTON = SBDL::loadTexture("assets/img/play.png");
    SBDL::freeTexture(PLAY_BUTTON_SELECTED);
    PLAY_BUTTON_SELECTED = SBDL::loadTexture("assets/img/play_selected.png");

	SAVED_GAME_FONT = SBDL::loadFont("assets/fonts/ProductSans.ttf", 20);

    SBDL::freeSound(MENU_SELECT_SOUND);
	MENU_SELECT_SOUND = SBDL::loadSound("assets/sounds/menu_select.wav");

    bool IS_PLAY_BUTTON_SELECTED = false;

    while (SBDL::isRunning()) {
		SBDL::updateEvents();
		SBDL::clearRenderScreen();

        SDL_SetWindowTitle(SBDL::Core::window,"Saved Games");

        if(slots==0){
            SBDL::freeTexture(SAVED_GAMES_BACKGROUND);
            SAVED_GAMES_BACKGROUND = SBDL::loadTexture("assets/img/saved_games_empty.png");
            SBDL::showTexture(SAVED_GAMES_BACKGROUND,0,0);
        }else{
            SBDL::freeTexture(SAVED_GAMES_BACKGROUND);
            SAVED_GAMES_BACKGROUND = SBDL::loadTexture("assets/img/saved_games.png");
            SBDL::showTexture(SAVED_GAMES_BACKGROUND,0,0);

            SBDL::showTexture(UP_ICON,400,235);
            SBDL::showTexture(DOWN_ICON,400,312);

            SBDL::showTexture(PLAY_BUTTON,(WINDOW_WIDTH-PLAY_BUTTON.width)/2,450);

            load_slot(slot);


            std::string title = "#";
            title.append(to_string(slot));

            SBDL::freeTexture(SAVED_GAME_DATA_TEXTURE);
            SAVED_GAME_DATA_TEXTURE = SBDL::createFontTexture(SAVED_GAME_FONT, title.c_str(), 0, 0, 0);
	        SBDL::showTexture(SAVED_GAME_DATA_TEXTURE, (WINDOW_WIDTH-SAVED_GAME_DATA_TEXTURE.width)/2,180);

            SBDL::freeTexture(SAVED_GAME_DATA_TEXTURE);
            SAVED_GAME_DATA_TEXTURE = SBDL::createFontTexture(SAVED_GAME_FONT, SAVED_DATE, 0, 0, 0);
	        SBDL::showTexture(SAVED_GAME_DATA_TEXTURE, (WINDOW_WIDTH-SAVED_GAME_DATA_TEXTURE.width)/2,240);

            SBDL::freeTexture(SAVED_GAME_DATA_TEXTURE);

            if(GAME_MODE==0) SAVED_GAME_DATA_TEXTURE = SBDL::createFontTexture(SAVED_GAME_FONT, "Easy", 0, 0, 0);
            else if(GAME_MODE==1) SAVED_GAME_DATA_TEXTURE = SBDL::createFontTexture(SAVED_GAME_FONT, "Medium", 0, 0, 0);
            else if(GAME_MODE==2) SAVED_GAME_DATA_TEXTURE = SBDL::createFontTexture(SAVED_GAME_FONT, "Hard", 0, 0, 0);
            else if(GAME_MODE==3) SAVED_GAME_DATA_TEXTURE = SBDL::createFontTexture(SAVED_GAME_FONT, "Custom", 0, 0, 0);

	        SBDL::showTexture(SAVED_GAME_DATA_TEXTURE, (WINDOW_WIDTH-SAVED_GAME_DATA_TEXTURE.width)/2,320);
        }

        SBDL::showTexture(BACK_ICON,60,80);
        if(SBDL::mouseInRect(getObjectRect(BACK_ICON,60,80))){
            SBDL::showTexture(SELECTED_BUTTON,60,80);
            if(SBDL::Mouse.clicked()) break;
        }
        else if(SBDL::mouseInRect(getObjectRect(PLAY_BUTTON,(WINDOW_WIDTH-PLAY_BUTTON.width)/2,450)) && slots!=0){
            SBDL::showTexture(PLAY_BUTTON_SELECTED,(WINDOW_WIDTH-PLAY_BUTTON_SELECTED.width)/2,450);
            if(!IS_PLAY_BUTTON_SELECTED){
                SBDL::playSound(MENU_SELECT_SOUND, 1);
                IS_PLAY_BUTTON_SELECTED = true;
            }
            if(SBDL::Mouse.clicked()){
                SLOT = slot;
                IS_LOADED_GAME = true;
                start_game();
                break;
            }
        }else if(SBDL::mouseInRect(getObjectRect(UP_ICON,400,235))&& SBDL::Mouse.clicked()){
            if(slot==slots) slot=1;
            else slot++;
        }else if(SBDL::mouseInRect(getObjectRect(DOWN_ICON,400,312))&& SBDL::Mouse.clicked()){
            if(slot==1) slot=slots;
            else slot--;
        }else{
            IS_PLAY_BUTTON_SELECTED=false;
        }

        SBDL::updateRenderScreen();

		SBDL::delay(30);

    }
}

void save_game(int slot){
    if(slot==0){
        slot=1;
        while(is_file_exist(string("assets/data/")+string(USERNAME)+string("_save_")+to_string(slot)+string(".txt"))){
            slot++;
        }
    }
    ofstream save_slot(string("assets/data/")+string(USERNAME)+string("_save_")+to_string(slot)+string(".txt"));
    save_slot << current_time() << "\n";
    save_slot << GAME_MODE << "\n";
    save_slot << SCORE << "\n";
    save_slot << NUMBER_OF_FLAGS << "\n";
    save_slot << SIZE << "\n";
    save_slot << MINES << "\n";
    save_slot << SHOW_NUMBERS << "\n";
    if(IS_TIMING) save_slot << START_TIME-time(0) << "\n";
    else save_slot << -1 << "\n";
    for(int i=0;i<MINES;i++){
        save_slot << MINES_LOCATION[i][0] << "," << MINES_LOCATION[i][1] << "/";
    }
    save_slot << "\n";
    for(int i=0;i<SIZE;i++){
        for(int j=0;j<SIZE;j++){
            save_slot << table[i][j];
        }
    }
    save_slot << "\n";
    save_slot.close();
    secure(string("assets/data/")+string(USERNAME)+string("_save_")+to_string(slot)+string(".txt"));
}

string encrypt(string msg){
    string key = KEY;
    string tmp(key);
    while (key.size() < msg.size())
        key += tmp;
    for (string::size_type i = 0; i < msg.size(); ++i)
        msg[i] ^= key[i];
    return msg;
}

void secure(string file_name){
    ifstream file(file_name);
    ofstream tmp_file("tmp");
    string line;
    while(getline(file,line)){
        tmp_file << encrypt(line) << endl;
    }
    file.close();
    tmp_file.close();

    ofstream file2(file_name);
    ifstream tmp_file2("tmp");
    while(getline(tmp_file2,line)){
        file2 << line << endl;
    }
    file2.close();
    tmp_file2.close();
    remove("tmp");
}

const string current_time() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    return buf;
}

void load_slot(int slot){
    secure(string("assets/data/")+string(USERNAME)+string("_save_")+to_string(slot)+string(".txt"));
    ifstream save_slot(string("assets/data/")+string(USERNAME)+string("_save_")+to_string(slot)+string(".txt"));
    string line;
    int i = 0;
    IS_LOST = false;
    IS_WON = false;
    while(getline(save_slot,line)){
        if(i==0) SAVED_DATE = line;
        else if(i==1) GAME_MODE = stoi(line);
        else if(i==2) SCORE = stoi(line);
        else if(i==3) NUMBER_OF_FLAGS = stoi(line);
        else if(i==4) SIZE = stoi(line);
        else if(i==5) MINES = stoi(line);
        else if(i==6) SHOW_NUMBERS = stoi(line);
        else if(i==7){
            if(stoi(line)==-1) IS_TIMING = false;
            else{
                IS_TIMING = true;
                START_TIME = time(0) + stoi(line) + 1;
            }
        }else if(i==8){
            int x = 0;
            int y = 0;
            int k = 0;
            bool first = true;
            int j = 0;
            while(j<MINES){
                if(line[k]==','){
                    MINES_LOCATION[j][0]=x;
                    first = false;
                }else if(line[k]=='/'){
                    MINES_LOCATION[j][1]=y;
                    x = 0;
                    y = 0;
                    j++;
                    first = true;
                }else{
                    if(first){
                        x *= 10;
                        x += ((int)line[k])-48;
                    }else{
                        y *= 10;
                        y += ((int)line[k])-48;
                    }
                }
                k++;
            }
        }else if(i==9){
            for(int i=0;i<SIZE;i++){
                for(int j=0;j<SIZE;j++){
                    table[i][j] = line[i*SIZE+j];
                }
            }
        }
        i++;
    }
    save_slot.close();
    secure(string("assets/data/")+string(USERNAME)+string("_save_")+to_string(slot)+string(".txt"));
    SELECTED_ITEM[0] = SELECTED_ITEM[1] = SIZE/2;
}

void remove_slot(int slot){
    remove((string("assets/data/")+string(USERNAME)+string("_save_")+to_string(slot)+string(".txt")).c_str());
    int i = slot+1;
    while(is_file_exist(string("assets/data/")+string(USERNAME)+string("_save_")+to_string(i)+string(".txt"))){
        rename((string("assets/data/")+string(USERNAME)+string("_save_")+to_string(i)+string(".txt")).c_str(),(string("assets/data/")+string(USERNAME)+string("_save_")+to_string(i-1)+string(".txt")).c_str());
        i++;
    }
}

void get_settings(){

    SBDL::freeTexture(CUSTOM_GAMES_BACKGROUND);
    CUSTOM_GAMES_BACKGROUND = SBDL::loadTexture("assets/img/custom_menu.png");

    SBDL::freeTexture(BACK_ICON);
    BACK_ICON = SBDL::loadTexture("assets/img/back.png");

    SBDL::freeTexture(RIGHT_ICON);
    RIGHT_ICON = SBDL::loadTexture("assets/img/right.png");

    SBDL::freeTexture(LEFT_ICON);
    LEFT_ICON = SBDL::loadTexture("assets/img/left.png");

    CUSTOM_MODE_FONT = SBDL::loadFont("assets/fonts/ProductSans.ttf", 20);

    SBDL::freeTexture(PLAY_BUTTON);
    PLAY_BUTTON = SBDL::loadTexture("assets/img/play.png");
    SBDL::freeTexture(PLAY_BUTTON_SELECTED);
    PLAY_BUTTON_SELECTED = SBDL::loadTexture("assets/img/play_selected.png");

    SIZE = 5;
    MINES = 1;
    TIME_LENGTH = 0;
    bool IS_PLAY_BUTTON_SELECTED = false;

    while (SBDL::isRunning()) {
		SBDL::updateEvents();
		SBDL::clearRenderScreen();
        SDL_SetWindowTitle(SBDL::Core::window,"Custom Mode");

        SBDL::showTexture(CUSTOM_GAMES_BACKGROUND,0,0);

        SBDL::showTexture(BACK_ICON,60,80);

        std::string title = "";
        title.append(to_string(SIZE));
        title.append(" * ");
        title.append(to_string(SIZE));

        SBDL::freeTexture(CUSTOM_MODE_DATA_TEXTURE);
        CUSTOM_MODE_DATA_TEXTURE = SBDL::createFontTexture(CUSTOM_MODE_FONT, title.c_str(), 0, 0, 0);
	    SBDL::showTexture(CUSTOM_MODE_DATA_TEXTURE, (WINDOW_WIDTH-CUSTOM_MODE_DATA_TEXTURE.width)/2+10,226);

        SBDL::freeTexture(CUSTOM_MODE_DATA_TEXTURE);
        CUSTOM_MODE_DATA_TEXTURE = SBDL::createFontTexture(CUSTOM_MODE_FONT, to_string(MINES), 0, 0, 0);
	    SBDL::showTexture(CUSTOM_MODE_DATA_TEXTURE, (WINDOW_WIDTH-CUSTOM_MODE_DATA_TEXTURE.width)/2+10,306);

        SBDL::freeTexture(CUSTOM_MODE_DATA_TEXTURE);
        CUSTOM_MODE_DATA_TEXTURE = SBDL::createFontTexture(CUSTOM_MODE_FONT, ((TIME_LENGTH==0)?"UNLIMITED":to_string(TIME_LENGTH)), 0, 0, 0);
	    SBDL::showTexture(CUSTOM_MODE_DATA_TEXTURE, (WINDOW_WIDTH-CUSTOM_MODE_DATA_TEXTURE.width)/2+10,386);

        SBDL::showTexture(PLAY_BUTTON,(WINDOW_WIDTH-PLAY_BUTTON.width)/2,470);

        if(SIZE<20) SBDL::showTexture(RIGHT_ICON,378,225);
        if(MINES<SIZE*SIZE-1) SBDL::showTexture(RIGHT_ICON,378,304);
        if(TIME_LENGTH<999) SBDL::showTexture(RIGHT_ICON,378,383);

        if(SIZE>5) SBDL::showTexture(LEFT_ICON,90,225);
        if(MINES>1) SBDL::showTexture(LEFT_ICON,90,304);
        if(TIME_LENGTH>0) SBDL::showTexture(LEFT_ICON,90,383);

        if(SBDL::mouseInRect(getObjectRect(BACK_ICON,60,80))){
            SBDL::showTexture(SELECTED_BUTTON,60,80);
            if(SBDL::Mouse.clicked()) break;
        }
        if(SBDL::mouseInRect(getObjectRect(RIGHT_ICON,378,225)) && SIZE<20){
            SBDL::showTexture(SELECTED_BUTTON,378,225);
            if(SBDL::Mouse.clicked()) SIZE++;
        }
        if(SBDL::mouseInRect(getObjectRect(LEFT_ICON,90,225)) && SIZE>5){
            SBDL::showTexture(SELECTED_BUTTON,90,225);
            if(SBDL::Mouse.clicked()){
                SIZE--;
                if(MINES>SIZE*SIZE-1) MINES=SIZE*SIZE-1;
            }
        }
        if(SBDL::mouseInRect(getObjectRect(RIGHT_ICON,378,304)) && MINES<SIZE*SIZE-1){
            SBDL::showTexture(SELECTED_BUTTON,378,304);
            if(SBDL::Mouse.clicked()) MINES++;
        }
        if(SBDL::mouseInRect(getObjectRect(LEFT_ICON,90,304)) && MINES>1){
            SBDL::showTexture(SELECTED_BUTTON,90,304);
            if(SBDL::Mouse.clicked()) MINES--;
        }
        if(SBDL::mouseInRect(getObjectRect(RIGHT_ICON,378,383))){
            SBDL::showTexture(SELECTED_BUTTON,378,383);
            if(SBDL::Mouse.clicked()) TIME_LENGTH++;
        }
        if(SBDL::mouseInRect(getObjectRect(LEFT_ICON,90,383)) && TIME_LENGTH>0){
            SBDL::showTexture(SELECTED_BUTTON,90,383);
            if(SBDL::Mouse.clicked()) TIME_LENGTH--;
        }
        else if(SBDL::mouseInRect(getObjectRect(PLAY_BUTTON,(WINDOW_WIDTH-PLAY_BUTTON.width)/2,470))){
            SBDL::showTexture(PLAY_BUTTON_SELECTED,(WINDOW_WIDTH-PLAY_BUTTON_SELECTED.width)/2,470);
            if(!IS_PLAY_BUTTON_SELECTED){
                SBDL::playSound(MENU_SELECT_SOUND, 1);
                IS_PLAY_BUTTON_SELECTED = true;
            }
            if(SBDL::Mouse.clicked()){
                if(TIME_LENGTH==0) IS_TIMING=false;
                else IS_TIMING=true;
                GAME_MODE=3;
                IS_LOADED_GAME = false;
                SBDL::Mouse.state = SDL_RELEASED;
                start_game();
                break;
            }
        }else{
            IS_PLAY_BUTTON_SELECTED=false;
        }
        SBDL::updateRenderScreen();

		SBDL::delay(30);

    }
}
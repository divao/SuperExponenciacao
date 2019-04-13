#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>

#include <stdlib.h>
#include <time.h>

#include "Background.h"

#include <iostream>
using namespace std;

//Variaveis globais
const int WIDTH = 960;
const int HEIGHT = 600;

enum STATE{TITLE, ALGOBRUTE, ALGODEC, TRACEBRUTE, TRACEDEC, CHOOSETRACE, CHOOSEALGORITHM, INFO, BRUTERUN, DECRUN};
enum KEYS{LEFT, RIGHT};
bool keys[2] = {false, false};

int passosBrute[12] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
int aDec[12] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
int nDec[12] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
int resultadoDec[12] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
int a = 2;
int n = 1;
int oldN = n;
int proxBrute = 0;
int proxDec = 0;

//Funcoes globais
void ChangeState(int &state, int newState);
int brutePow(int, int, int *);
int decPow(int, int, int *, int *, int *);

int main(void){
	
	bool done = false;
	bool redraw = true;
	const int FPS = 60;
	int state = -1;
	int posX = 0;
	int posY = 0;
	
	srand(time(NULL));

	//object variables
	Background back;

	//Allegro variables
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_FONT *fontTimes38 = NULL;
	ALLEGRO_FONT *fontTimes46 = NULL;
	ALLEGRO_SAMPLE *song = NULL;
	ALLEGRO_BITMAP *backImage = NULL;
	ALLEGRO_BITMAP *titleImage = NULL;
	ALLEGRO_BITMAP *trace = NULL;
	ALLEGRO_BITMAP *traceMouse = NULL;
	ALLEGRO_BITMAP *algo = NULL;
	ALLEGRO_BITMAP *algoMouse = NULL;
	ALLEGRO_BITMAP *inter = NULL;
	ALLEGRO_BITMAP *interMouse = NULL;
	ALLEGRO_BITMAP *bruteButton = NULL;
	ALLEGRO_BITMAP *bruteButtonMouse = NULL;
	ALLEGRO_BITMAP *decButton = NULL;
	ALLEGRO_BITMAP *decButtonMouse = NULL;
	ALLEGRO_BITMAP *menuButton = NULL;
	ALLEGRO_BITMAP *menuButtonMouse = NULL;
	ALLEGRO_BITMAP *credits = NULL;
	ALLEGRO_BITMAP *algoBruteImage = NULL;
	ALLEGRO_BITMAP *algoDecImage = NULL;
	ALLEGRO_BITMAP *blockNro = NULL;
	ALLEGRO_BITMAP *menosBotao = NULL;
	ALLEGRO_BITMAP *maisBotao = NULL;
	ALLEGRO_BITMAP *startButton = NULL;
	ALLEGRO_BITMAP *proxBotao = NULL;
	ALLEGRO_BITMAP *proxMouse = NULL;
	ALLEGRO_BITMAP *antBotao = NULL;
	ALLEGRO_BITMAP *antMouse = NULL;

	//Initialization Functions
	if(!al_init())										//initialize Allegro
		return -1;

	display = al_create_display(WIDTH, HEIGHT);			//create our display object

	if(!display)										//TRACE display object
		return -1;

	al_init_primitives_addon();
	al_install_keyboard();
	al_install_mouse();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();
	al_install_audio();
	al_init_acodec_addon();

	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);
	
	al_reserve_samples(10);
	
	song = al_load_sample("audio/song.ogg");
	backImage = al_load_bitmap("img/background.png");
	titleImage = al_load_bitmap("img/titleImage.png");
	trace = al_load_bitmap("img/trace.png");
	traceMouse = al_load_bitmap("img/traceMouse.png");
	algo = al_load_bitmap("img/algo.png");
	algoMouse = al_load_bitmap("img/algoMouse.png");
	inter = al_load_bitmap("img/inter.png");
	interMouse = al_load_bitmap("img/interMouse.png");
	bruteButton = al_load_bitmap("img/bruteButton.png");
	bruteButtonMouse = al_load_bitmap("img/bruteButtonMouse.png");
	decButton = al_load_bitmap("img/decButton.png");
	decButtonMouse = al_load_bitmap("img/decButtonMouse.png");
	menuButton = al_load_bitmap("img/menuButton.png");
	menuButtonMouse = al_load_bitmap("img/menuButtonMouse.png");
	credits = al_load_bitmap("img/credits.png");
	algoBruteImage= al_load_bitmap("img/algoBrute.png");
	algoDecImage= al_load_bitmap("img/algoDec.png");
	blockNro = al_load_bitmap("img/blockNro.png");
	menosBotao = al_load_bitmap("img/menosBotao.png");
	maisBotao = al_load_bitmap("img/maisBotao.png");
	startButton = al_load_bitmap("img/startButton.png");
	proxBotao = al_load_bitmap("img/proxBotao.png");
	proxMouse = al_load_bitmap("img/proxMouse.png");
	antBotao = al_load_bitmap("img/antBotao.png");
	antMouse = al_load_bitmap("img/antMouse.png");
	
	fontTimes38 = al_load_font("fontes/timesbd.ttf", 38, 0);
	fontTimes46 = al_load_font("fontes/timesbd.ttf", 46, 0);

	ChangeState(state, TITLE);

	back.InitBackground(0, 0, 960, 600, backImage);

	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	
	al_play_sample(song, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);

	al_start_timer(timer);
	while(!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if(ev.type == ALLEGRO_EVENT_TIMER){
			redraw = true;
		}
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
			done = true;
		}
		else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if(ev.mouse.button & 1){
				if(state != TITLE){
					if(posX >= 5 && posX <= 5 + 111 && posY >= 366 - 55 / 2 && posY <= 366 + 55 / 2){
						ChangeState(state, TITLE);
						al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
					}
				}
				if(state == TITLE){
					if(posX >= 220 - 166 / 2 && posX <= 220 + 166 / 2 && posY >= 366 - 55 / 2 && posY <= 366 + 55 / 2){
						ChangeState(state, CHOOSETRACE);
						al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
					}else if(posX >= 480 - 221 / 2 && posX <= 480 + 221 / 2 && posY >= 366 - 55 / 2 && posY <= 366 + 55 / 2){
						ChangeState(state, CHOOSEALGORITHM);
						al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
					}else if(posX >= 720 - 60 / 2 && posX <= 720 + 60 / 2 && posY >= 366 - 60 / 2 && posY <= 366 + 60 / 2){
						ChangeState(state, INFO);
						al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
					}
				}else if(state == CHOOSETRACE){
					if(posX >= 330 - 221 / 2 && posX <= 330 + 221 / 2 && posY >= 366 - 55 / 2 && posY <= 366 + 55 / 2){
						ChangeState(state, TRACEBRUTE);
						al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
					}else if(posX >= 660 - 221 / 2 && posX <= 660 + 221 / 2 && posY >= 366 - 55 / 2 && posY <= 366 + 55 / 2){
						ChangeState(state, TRACEDEC);
						al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
					}
				}else if(state == CHOOSEALGORITHM){
					if(posX >= 330 - 221 / 2 && posX <= 330 + 221 / 2 && posY >= 366 - 55 / 2 && posY <= 366 + 55 / 2){
						ChangeState(state, ALGOBRUTE);
						al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
					}else if(posX >= 660 - 221 / 2 && posX <= 660 + 221 / 2 && posY >= 366 - 55 / 2 && posY <= 366 + 55 / 2){
						ChangeState(state, ALGODEC);
						al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
					}
				}else if(state == TRACEBRUTE){
					if(posX >= 154 && posX <= 154 + 50 && posY >= 185 && posY <= 185 + 50){
						if(a > 2)
							a--;
					}else if(posX >= 276 && posX <= 276 + 50 && posY >= 185 && posY <= 185 + 50){
						if(a < 5)
							a++;
					}else if(posX >= 154 && posX <= 154 + 50 && posY >= 341 && posY <= 341 + 50){
						if(n > 1)
							n--;
					}else if(posX >= 276 && posX <= 276 + 50 && posY >= 341 && posY <= 341 + 50){
						if(n < 12)
							n++;
					}else if(posX >= 643 - 181 / 2 && posX <= 643 + 181 / 2 && posY >= 268 && posY <= 268 + 60){
						ChangeState(state, BRUTERUN);
					}	
				}else if(state == BRUTERUN){
					if(posX >= WIDTH - 111 - 13 && posX <= WIDTH - 13 && posY >= 366 - 55 / 2 && posY <= 366 + 55 / 2){
						if(proxBrute < n)
							proxBrute++;
					}else if(posX >= WIDTH - 111 - 13 && posX <= WIDTH - 13 && posY >= 246 - 55 / 2 && posY <= 246 + 55 / 2){
						if(proxBrute > 1)
							proxBrute--;
					}
				}else if(state == TRACEDEC){
					if(posX >= 154 && posX <= 154 + 50 && posY >= 185 && posY <= 185 + 50){
						if(a > 2)
							a--;
					}else if(posX >= 276 && posX <= 276 + 50 && posY >= 185 && posY <= 185 + 50){
						if(a < 5)
							a++;
					}else if(posX >= 154 && posX <= 154 + 50 && posY >= 341 && posY <= 341 + 50){
						if(n > 1)
							n--;
					}else if(posX >= 276 && posX <= 276 + 50 && posY >= 341 && posY <= 341 + 50){
						if(n < 12)
							n++;
					}else if(posX >= 643 - 181 / 2 && posX <= 643 + 181 / 2 && posY >= 268 && posY <= 268 + 60){
						ChangeState(state, DECRUN);
					}
				}else if(state == DECRUN){
					if(posX >= WIDTH - 111 - 13 && posX <= WIDTH - 13 && posY >= 366 - 55 / 2 && posY <= 366 + 55 / 2){
						if(proxDec < n)
							proxDec++;
					}else if(posX >= WIDTH - 111 - 13 && posX <= WIDTH - 13 && posY >= 246 - 55 / 2 && posY <= 246 + 55 / 2){
						if(proxDec > 1)
							proxDec--;
					}
				}
			}
		}else if(ev.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			posX = ev.mouse.x;
			posY = ev.mouse.y;
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
			switch(ev.keyboard.keycode){
				case ALLEGRO_KEY_ESCAPE:
					done = true;
					break;
				case ALLEGRO_KEY_LEFT:
					keys[LEFT] = true;
					break;
				case ALLEGRO_KEY_RIGHT:
					keys[RIGHT] = true;
					break;
				case ALLEGRO_KEY_SPACE:
					break;
				case ALLEGRO_KEY_ENTER:
					break;
			}
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_UP){
			switch(ev.keyboard.keycode){
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			}
		}
		if(redraw && al_is_event_queue_empty(event_queue)){
			redraw = false; 
			back.DrawBackground();
			
			if(state == TITLE || state == CHOOSETRACE || state == CHOOSEALGORITHM){
				al_draw_bitmap(titleImage, WIDTH / 2 - 461 / 2, 110, 0);
			}
			
			if(state != TITLE){
				if(posX >= 5 && posX <= 5 + 111 && posY >= 366 - 55 / 2 && posY <= 366 + 55 / 2){
					al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
					al_draw_bitmap(menuButtonMouse, 0, 366 - 2 - 60 / 2, 0);
				}else{
					al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
					al_draw_bitmap(menuButton, 5, 366 - 55 / 2, 0);
				}
			}
			
			if(state == TITLE){
				if(posX >= 220 - 166 / 2 && posX <= 220 + 166 / 2 && posY >= 366 - 55 / 2 && posY <= 366 + 55 / 2){
					al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
					al_draw_bitmap(traceMouse, 220 - 181 / 2, 366 - 60 / 2, 0);
					al_draw_bitmap(algo, 480 - 221 / 2, 366 - 55 / 2, 0);
					al_draw_bitmap(inter, 720 - 55 / 2, 366 - 55 / 2, 0);
				}else if(posX >= 480 - 221 / 2 && posX <= 480 + 221 / 2 && posY >= 366 - 55 / 2 && posY <= 366 + 55 / 2){
					al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
					al_draw_bitmap(algoMouse, 480 - 241 / 2, 366 - 60 / 2, 0);
					al_draw_bitmap(trace, 220 - 166 / 2, 366 - 55 / 2, 0);
					al_draw_bitmap(inter, 720 - 55 / 2, 366 - 55 / 2, 0);
				}else if(posX >= 720 - 60 / 2 && posX <= 720 + 60 / 2 && posY >= 366 - 60 / 2 && posY <= 366 + 60 / 2){
					al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
					al_draw_bitmap(interMouse, 720 - 60 / 2, 366 - 60 / 2, 0);
					al_draw_bitmap(trace, 220 - 166 / 2, 366 - 55 / 2, 0);
					al_draw_bitmap(algo, 480 - 221 / 2, 366 - 55 / 2, 0);
				}else{
					al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
					al_draw_bitmap(trace, 220 - 166 / 2, 366 - 55 / 2, 0);
					al_draw_bitmap(algo, 480 - 221 / 2, 366 - 55 / 2, 0);
					al_draw_bitmap(inter, 720 - 55 / 2, 366 - 55 / 2, 0);
				}
			}else if(state == CHOOSETRACE || state == CHOOSEALGORITHM){
				if(posX >= 330 - 221 / 2 && posX <= 330 + 221 / 2 && posY >= 366 - 55 / 2 && posY <= 366 + 55 / 2){
					al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
					al_draw_bitmap(bruteButtonMouse, 330 - 241 / 2, 366 - 60 / 2, 0);
					al_draw_bitmap(decButton, 660 - 221 / 2, 366 - 55 / 2, 0);
				}else if(posX >= 660 - 221 / 2 && posX <= 660 + 221 / 2 && posY >= 366 - 55 / 2 && posY <= 366 + 55 / 2){
					al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
					al_draw_bitmap(bruteButton, 330 - 221 / 2, 366 - 55 / 2, 0);
					al_draw_bitmap(decButtonMouse, 660 - 241 / 2, 366 - 60 / 2, 0);
				}else{
					al_draw_bitmap(bruteButton, 330 - 221 / 2, 366 - 55 / 2, 0);
					al_draw_bitmap(decButton, 660 - 221 / 2, 366 - 55 / 2, 0);
				}
			}else if(state == INFO){
				al_draw_bitmap(credits, 130, 90, 0);
			}else if(state == ALGOBRUTE){
				al_draw_bitmap(algoBruteImage, 130, 104, 0);
			}else if(state == ALGODEC){
				al_draw_bitmap(algoDecImage, 130, 104, 0);
			}else if(state == TRACEBRUTE){
				al_draw_bitmap(blockNro, 210, 180, 0);
				al_draw_text(fontTimes38, al_map_rgb(246,254,122), 210 + 60 / 2, 180 - 38 - 4, ALLEGRO_ALIGN_CENTRE, "Base a");
				al_draw_textf(fontTimes46, al_map_rgb(162,227,217), 210 + 60 / 2, 186, ALLEGRO_ALIGN_CENTRE, "%d", a);
				al_draw_bitmap(menosBotao, 154, 185, 0);
				al_draw_bitmap(maisBotao, 276, 185, 0);
				
				al_draw_bitmap(blockNro, 210, 336, 0);
				al_draw_text(fontTimes38, al_map_rgb(246,254,122), 210 + 60 / 2, 336 - 38 - 4, ALLEGRO_ALIGN_CENTRE, "Expoente n");
				al_draw_textf(fontTimes46, al_map_rgb(162,227,217), 210 + 60 / 2, 336 + 6, ALLEGRO_ALIGN_CENTRE, "%d", n);
				al_draw_bitmap(menosBotao, 154, 341, 0);
				al_draw_bitmap(maisBotao, 276, 341, 0);
				
				al_draw_bitmap(startButton, 643 - 181 / 2 , 268, 0);
			}else if(state == BRUTERUN){
				al_draw_bitmap(blockNro, 210, 180, 0);
				al_draw_text(fontTimes38, al_map_rgb(246,254,122), 210 + 60 / 2, 180 - 38 - 4, ALLEGRO_ALIGN_CENTRE, "Base a");
				al_draw_textf(fontTimes46, al_map_rgb(162,227,217), 210 + 60 / 2, 186, ALLEGRO_ALIGN_CENTRE, "%d", a);
				
				al_draw_bitmap(blockNro, 210, 336, 0);
				al_draw_text(fontTimes38, al_map_rgb(246,254,122), 210 + 60 / 2, 336 - 38 - 4, ALLEGRO_ALIGN_CENTRE, "Expoente n");
				al_draw_textf(fontTimes46, al_map_rgb(162,227,217), 210 + 60 / 2, 336 + 6, ALLEGRO_ALIGN_CENTRE, "%d", n);
				
				if(proxBrute < 13){
					if(passosBrute[proxBrute - 1] != -1){
						if(proxBrute > 1){
							if(posX >= WIDTH - 111 - 13 && posX <= WIDTH - 13 && posY >= 246 - 55 / 2 && posY <= 246 + 55 / 2){
								al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
								al_draw_bitmap(antMouse, WIDTH - 111 - 13 - 5, 246 - 2 - 60 / 2, 0);
							}else{
								al_draw_bitmap(antBotao, WIDTH - 111 - 13, 246 - 55 / 2, 0);
							}
						}
						if(proxBrute < n){
							if(posX >= WIDTH - 111 - 13 && posX <= WIDTH - 13 && posY >= 366 - 55 / 2 && posY <= 366 + 55 / 2){
								al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
								al_draw_bitmap(proxMouse, WIDTH - 111 - 13 - 5, 366 - 2 - 60 / 2, 0);
							}else{
								al_draw_bitmap(proxBotao, WIDTH - 111 - 13, 366 - 55 / 2, 0);
							}
						}
					
						al_draw_textf(fontTimes38, al_map_rgb(246,254,122), 572, 160, ALLEGRO_ALIGN_CENTRE, "Passo %d:", proxBrute);
						if(proxBrute == 1)
							al_draw_text(fontTimes38, al_map_rgb(246,254,122), 572, 160 + 38 + 38 + 10, ALLEGRO_ALIGN_CENTRE, "a = 1");
						else
						al_draw_textf(fontTimes38, al_map_rgb(246,254,122), 572, 160 + 38 + 38 + 10, ALLEGRO_ALIGN_CENTRE, "a = %d", passosBrute[proxBrute - 2]);
						al_draw_textf(fontTimes38, al_map_rgb(246,254,122), 572, 160 + 38 + 38 + 38 + 15, ALLEGRO_ALIGN_CENTRE, "a = a * %d", a);
						al_draw_textf(fontTimes38, al_map_rgb(246,254,122), 572, 160 + 38 + 38 + 38 + 38 + 20, ALLEGRO_ALIGN_CENTRE, "a = %d", passosBrute[proxBrute - 1]);
						if(proxBrute == n)
							al_draw_textf(fontTimes38, al_map_rgb(226,0,0), 572, 160 + 38 + 38 + 38 + 38 + 38 + 25, ALLEGRO_ALIGN_CENTRE, "Resultado = %d", passosBrute[proxBrute - 1]);
					}
				}
			}else if(state == TRACEDEC){
				al_draw_bitmap(blockNro, 210, 180, 0);
				al_draw_text(fontTimes38, al_map_rgb(246,254,122), 210 + 60 / 2, 180 - 38 - 4, ALLEGRO_ALIGN_CENTRE, "Base a");
				al_draw_textf(fontTimes46, al_map_rgb(162,227,217), 210 + 60 / 2, 186, ALLEGRO_ALIGN_CENTRE, "%d", a);
				al_draw_bitmap(menosBotao, 154, 185, 0);
				al_draw_bitmap(maisBotao, 276, 185, 0);
				
				al_draw_bitmap(blockNro, 210, 336, 0);
				al_draw_text(fontTimes38, al_map_rgb(246,254,122), 210 + 60 / 2, 336 - 38 - 4, ALLEGRO_ALIGN_CENTRE, "Expoente n");
				al_draw_textf(fontTimes46, al_map_rgb(162,227,217), 210 + 60 / 2, 336 + 6, ALLEGRO_ALIGN_CENTRE, "%d", n);
				al_draw_bitmap(menosBotao, 154, 341, 0);
				al_draw_bitmap(maisBotao, 276, 341, 0);
				
				al_draw_bitmap(startButton, 643 - 181 / 2 , 268, 0);
			}else if(state == DECRUN){
				al_draw_bitmap(blockNro, 210, 180, 0);
				al_draw_text(fontTimes38, al_map_rgb(246,254,122), 210 + 60 / 2, 180 - 38 - 4, ALLEGRO_ALIGN_CENTRE, "Base a");
				al_draw_textf(fontTimes46, al_map_rgb(162,227,217), 210 + 60 / 2, 186, ALLEGRO_ALIGN_CENTRE, "%d", a);
				
				al_draw_bitmap(blockNro, 210, 336, 0);
				al_draw_text(fontTimes38, al_map_rgb(246,254,122), 210 + 60 / 2, 336 - 38 - 4, ALLEGRO_ALIGN_CENTRE, "Expoente n");
				al_draw_textf(fontTimes46, al_map_rgb(162,227,217), 210 + 60 / 2, 336 + 6, ALLEGRO_ALIGN_CENTRE, "%d", oldN);
				
				if(proxDec < 13){
					if(resultadoDec[proxBrute - 1] >= 0){
						if(nDec[proxDec - 1] == 0){
							aDec[proxDec - 1] = resultadoDec[proxDec - 1];
							n = proxDec;
						}
						if(proxDec > 1){
							if(posX >= WIDTH - 111 - 13 && posX <= WIDTH - 13 && posY >= 246 - 55 / 2 && posY <= 246 + 55 / 2){
								al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
								al_draw_bitmap(antMouse, WIDTH - 111 - 13 - 5, 246 - 2 - 60 / 2, 0);
							}else{
								al_draw_bitmap(antBotao, WIDTH - 111 - 13, 246 - 55 / 2, 0);
							}
						}
						if(proxDec < n){
							if(posX >= WIDTH - 111 - 13 && posX <= WIDTH - 13 && posY >= 366 - 55 / 2 && posY <= 366 + 55 / 2){
								al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
								al_draw_bitmap(proxMouse, WIDTH - 111 - 13 - 5, 366 - 2 - 60 / 2, 0);
							}else{
								al_draw_bitmap(proxBotao, WIDTH - 111 - 13, 366 - 55 / 2, 0);
							}
						}
					
						al_draw_textf(fontTimes38, al_map_rgb(246,254,122), 572, 160, ALLEGRO_ALIGN_CENTRE, "Passo %d:", proxDec);
						al_draw_textf(fontTimes38, al_map_rgb(246,254,122), 572, 160 + 38 + 38 + 10, ALLEGRO_ALIGN_CENTRE, "a = %d", aDec[proxDec - 1]);
						al_draw_textf(fontTimes38, al_map_rgb(246,254,122), 572, 160 + 38 + 38 + 38 + 15, ALLEGRO_ALIGN_CENTRE, "n = %d", nDec[proxDec - 1]);
						al_draw_textf(fontTimes38, al_map_rgb(246,254,122), 572, 160 + 38 + 38 + 38 + 38 + 20, ALLEGRO_ALIGN_CENTRE, "resultado = %d", resultadoDec[proxDec - 1]);
						if(proxDec == n)
							al_draw_textf(fontTimes38, al_map_rgb(226,0,0), 572, 160 + 38 + 38 + 38 + 38 + 38 + 25, ALLEGRO_ALIGN_CENTRE, "Resultado = %d", resultadoDec[proxDec - 1]);
					}
				}
			}
			
			al_flip_display();
			al_clear_to_color(al_map_rgb(0,0,0));
		}
	}
	
	al_destroy_bitmap(backImage);
	al_destroy_bitmap(titleImage);
	al_destroy_bitmap(trace);
	al_destroy_bitmap(traceMouse);
	al_destroy_bitmap(algo);
	al_destroy_bitmap(algoMouse);
	al_destroy_bitmap(inter);
	al_destroy_bitmap(interMouse);
	al_destroy_bitmap(bruteButton);
	al_destroy_bitmap(bruteButtonMouse);
	al_destroy_bitmap(decButton);
	al_destroy_bitmap(decButtonMouse);
	al_destroy_bitmap(menuButton);
	al_destroy_bitmap(menuButtonMouse);
	al_destroy_bitmap(credits);
	al_destroy_bitmap(algoBruteImage);
	al_destroy_bitmap(algoDecImage);
	al_destroy_bitmap(blockNro);
	al_destroy_bitmap(menosBotao);
	al_destroy_bitmap(maisBotao);
	al_destroy_bitmap(startButton);
	al_destroy_bitmap(proxBotao);
	al_destroy_bitmap(proxMouse);
	al_destroy_bitmap(antBotao);
	al_destroy_bitmap(antMouse);
	al_destroy_sample(song);
	al_destroy_font(fontTimes38);
	al_destroy_font(fontTimes46);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	al_destroy_display(display);						//destroy our display object

	return 0;
}

void ChangeState(int &state, int newState){
	state = newState;
	
	if(state == TITLE){
		a = 2;
		n = 1;
		proxBrute = 0;
		proxDec = 0;
		for(int i = 0; i < 12; i++){
			passosBrute[i] = -1;
			aDec[i] = -1;
			nDec[i] = -1;
			resultadoDec[i] = -1;
		}
	}else if(state == BRUTERUN){
		brutePow(a, n, passosBrute);
		proxBrute = 1;
	}else if(state == DECRUN){
		oldN = n;
		decPow(a, n, aDec, nDec, resultadoDec);
		proxDec = 1;
	}
}

int brutePow(int a, int n, int *passosBrute){
	int resultado = 1;
	for(int i = 0; i < n; i++){
		resultado *= a;
		passosBrute[i] = resultado;
	}
	return resultado;
}

int decPow(int a, int n, int *aDec, int *nDec, int *resultadoDec){
	int resultado = 1;
	int i = 0;

	while (n > 0){
		if (n % 2 == 1)
    		resultado *= a;
		n /= 2;
		a *= a;
		aDec[i] = a;
		nDec[i] = n;
		resultadoDec[i] = resultado;
		i++;
	}

	return resultado;
}

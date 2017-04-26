#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include<iostream>
using namespace std;

class ghost {
private:
	int width, height;
	int xPos, yPos;
	bool dead;
public:
	void set_values(int x, int y, int w, int h);
	void drawghost();
	bool isDead();//checks if something is dead
	void killghost();//kills brick
	
};


//function to detect collision
//paramaters are x & y coordinates of top left corner of first box, then its width and height
//then same for other box


//using constants in this game so you have sensible words, not unknown numbers in code
const float FPS = 60;
const int SCREEN_W = 1000;
const int SCREEN_H = 800;
const int BOUNCER_SIZE = 30; 
const int WALLSIZE = 40;
const int DOT = 4;
int wallCollide(int x, int y, int dir, int level[20][20]); //declaration
const int RIGHT = 1;
const int UP = 3;
const int DOWN = 4;
const int LEFT = 2;


//an enumeration is like a user-defined variable, with all the given values it can hold
enum MYKEYS {
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};


int main()
{
	int level[20][20] = {
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,0,0,0,0,1,1,1,0,0,0,1,0,0,0,0,0,0,0,1,
		1,0,1,1,0,1,1,0,0,1,0,0,0,1,1,0,1,1,0,1,
		1,0,1,1,0,1,1,0,1,1,1,1,0,1,1,0,1,1,0,1,
		1,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,1,
		1,0,0,1,0,1,1,0,1,0,1,0,1,1,1,0,0,1,1,1,
		1,0,1,1,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,1,
		1,0,0,1,0,1,1,1,0,1,1,1,0,0,1,0,1,1,0,1,
		1,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,0,1,
		1,0,0,1,1,1,1,0,1,0,1,0,0,0,0,0,1,0,1,1,
		1,0,0,0,0,0,0,0,1,0,1,0,1,1,1,0,0,0,0,1,
		1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,0,0,1,0,1,
		1,0,0,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,1,1,0,0,1,0,1,1,0,1,1,1,0,1,1,0,1,
		1,0,0,1,1,1,0,1,0,1,0,0,0,0,0,0,0,1,0,1,
		1,0,1,0,0,1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,
		1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,
		1,0,1,1,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,1,
		1,0,0,0,0,1,1,1,1,1,0,0,0,1,0,0,0,0,0,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,

	};

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *pacman = NULL;
	ALLEGRO_BITMAP*platform = NULL;
	ALLEGRO_BITMAP*dot = NULL;
	int bouncer_x = 50;
	int bouncer_y = 45;
	bool key[4] = { false, false, false, false };

	bool redraw = true;
	bool doexit = false;
	/////
	al_init();
	al_init_primitives_addon();

	//including some error messages to help debug
	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	if (!al_install_keyboard()) {
		fprintf(stderr, "failed to initialize the keyboard!\n");
		return -1;
	}

	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}

	display = al_create_display(SCREEN_W, SCREEN_H);
	if (!display) {
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}

	//hook up fonts for on-screen text
	al_init_font_addon();
	al_init_ttf_addon();
	ALLEGRO_FONT *font = al_load_ttf_font("graff.ttf", 72, 0);

	//set up the two squares
	pacman = al_create_bitmap(BOUNCER_SIZE, BOUNCER_SIZE);
	al_set_target_bitmap(pacman);
	al_clear_to_color(al_map_rgb(255, 0, 255));

	///wall///////////
	platform = al_create_bitmap(WALLSIZE, WALLSIZE);
	al_set_target_bitmap(platform);
	al_clear_to_color(al_map_rgb(0, 100, 255));
	//////dots/////
	dot = al_create_bitmap(DOT, DOT);
	al_set_target_bitmap(dot);
	al_clear_to_color(al_map_rgb(255, 255, 255));
	/////////


	al_set_target_bitmap(al_get_backbuffer(display));

	//set up your event queue
	event_queue = al_create_event_queue();
	if (!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_bitmap(pacman);
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	al_register_event_source(event_queue, al_get_keyboard_event_source());

	//draw the initial screen, start up the timer
	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_flip_display();

	al_start_timer(timer);
	ghost ghost1;
	ghost1.set_values(0, 0, 30, 30);
	ghost ghost2;
	ghost2.set_values(110, 0, 30, 30);
	ghost ghost3;
	ghost3.set_values(220, 0, 30, 30);
	ghost ghost4;
	ghost4.set_values(220, 0, 30, 30);
	
	while (!doexit)

	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		if (ghost1.isDead() && ghost2.isDead() && ghost3.isDead() && ghost4.isDead()) {
			al_rest(1);
			al_clear_to_color(al_map_rgb(0, 200, 255));
			al_flip_display();
			doexit = true;
			//if (!ghost1.isDead()) {
			//		ghost1.killghost(); //when you hit it, kill it
			//	}
			//if (!ghost2.isDead()) {
			//	ghost2.killghost(); //when you hit it, kill it
			//}
			//if (!ghost3.isDead()) {
			//	ghost3.killghost(); //when you hit it, kill it
			//}
			//if (!ghost4.isDead()) {
			//	ghost4.killghost(); //when you hit it, kill it
			//}
		}
		//if a clock ticks, check if we should be moving a square (by checking key or key2)
		if (ev.type == ALLEGRO_EVENT_TIMER) {
			if (key[KEY_UP] && (wallCollide(bouncer_x, bouncer_y, UP, level) != 1)) {
				bouncer_y -= 4.0;
			}

			if (key[KEY_DOWN] && (wallCollide(bouncer_x, bouncer_y, DOWN, level) != 1)) {
				bouncer_y += 4.0;
			}

			if (key[KEY_LEFT] && (wallCollide(bouncer_x, bouncer_y, LEFT, level) != 1)) {
				bouncer_x -= 4.0;
			}

			if (key[KEY_RIGHT] && (wallCollide(bouncer_x, bouncer_y, RIGHT, level) != 1)) {
				bouncer_x += 4.0;
			}
			{
				if ((level[bouncer_y / WALLSIZE][bouncer_x / WALLSIZE]) == 0)
					((level[bouncer_y / WALLSIZE][bouncer_x / WALLSIZE]) = 6);
			}


			//call chase function
			//ghost1.chase();

			redraw = true;
		}
		//kill program if window "X" has been clicked
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {

		}

		//update key/key2 if a key has been pressed
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {

			case ALLEGRO_KEY_UP:
				key[KEY_UP] = true;
				break;

			case ALLEGRO_KEY_DOWN:
				key[KEY_DOWN] = true;
				break;

			case ALLEGRO_KEY_LEFT:
				key[KEY_LEFT] = true;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT] = true;
				break;




			}
		}
		//update key/key2 if a key has been released
		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {

			case ALLEGRO_KEY_UP:
				key[KEY_UP] = false;
				break;

			case ALLEGRO_KEY_DOWN:
				key[KEY_DOWN] = false;
				break;

			case ALLEGRO_KEY_LEFT:
				key[KEY_LEFT] = false;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT] = false;
				break;



			}
		}



		//////RENDER SECTION////////////////
		if (redraw == true) {
			//take all the above information and update screen
			al_clear_to_color(al_map_rgb(0, 0, 0));

			al_draw_bitmap(pacman, bouncer_x, bouncer_y, 0);
			/////////Wall/////
			for (int i = 0; i < 20; i++)
				for (int j = 0; j < 20; j++) {
					if (level[i][j] == 1)
						al_draw_bitmap(platform, j * 40, i * 40, 0);
				}
			////dots
			for (int i = 0; i < 20; i++)
				for (int j = 0; j < 20; j++) {
					if (level[i][j] == 0)
						al_draw_bitmap(dot, j * 40 + 20, i * 40 + 20, 0);

				}



		}
		if (ghost1.isDead() == false) {
			ghost1.drawghost();
		}
		if (ghost2.isDead() == false) {
			ghost2.drawghost();
		}
		if (ghost3.isDead() == false) {
			ghost3.drawghost();
		}
		if (ghost4.isDead() == false) {
			ghost4.drawghost();

		}
		al_flip_display();
	}
	
	al_destroy_bitmap(pacman);
	al_destroy_bitmap(platform);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}
void ghost::set_values(int x, int y, int w, int h) {
	xPos = x;
	yPos = y;

	width = w;
	height = h;
	dead = false;
}

void ghost::drawghost() {
	al_draw_filled_rectangle(xPos, yPos, xPos + width, yPos + height, al_map_rgb(255, 255, 255));


}
bool ghost::isDead() {
	return dead;
}



//matrix function detection!!!

int wallCollide(int x, int y, int dir, int level[20][20])
{
	//temporarty variables
	int new_x1;
	int new_x2;
	int new_x3;
	int new_y1;
	int new_y2;
	int new_y3;

	if (dir == RIGHT) { 		// Moving Right
								// Check along the far right side of the sprite, plus 3 (the amount we’re moving)
		new_x1 = x + 5 + BOUNCER_SIZE;
		new_x2 = x + 5 + BOUNCER_SIZE;
		new_x3 = x + 5 + BOUNCER_SIZE;
		// Check at three point along that edge
		new_y1 = y;
		new_y2 = y + BOUNCER_SIZE / 2;
		new_y3 = y + BOUNCER_SIZE;

		if ((level[new_y1 / 40][new_x1 / 40] == 1) || level[new_y2 / 40][new_x2 / 40]==1 || (level[new_y3 / 40][new_x3 / 40] == 1))//add in for middle and bottom point
		{
			cout << "right collision!" << endl;
			return 1; //collision!
		}
	}
	if (dir == LEFT) { 		// Moving LEFT
							// Check along the far right side of the sprite, plus 3 (the amount we’re moving)
		new_x1 = x - 5;
		new_x2 = x - 5;
		new_x3 = x - 5;

		// Check at three point along that edge
		new_y1 = y;
		new_y2 = y + BOUNCER_SIZE / 2;
		new_y3 = y + BOUNCER_SIZE;

		if ((level[new_y1 / 40][new_x1 / 40] == 1) || level[new_y2 / 40][new_x2 / 40]==1 || (level[new_y3 / 40][new_x3 / 40] == 1))//add in for middle and bottom point
		{
			cout << "LEFT collision!" << endl;
			return 1; //collision!
		}
	}
	if (dir == UP) { 		// Moving LEFT
							// Check along the far right side of the sprite, plus 3 (the amount we’re moving)
		new_x1 = x;
		new_x2 = x + BOUNCER_SIZE / 2;
		new_x3 = x + BOUNCER_SIZE;

		// Check at three point along that edge
		new_y1 = y-5;
		new_y2 = y-5;
		new_y3 = y-5;

		if ((level[new_y1 / 40][new_x1 / 40] == 1) || level[new_y2 / 40][new_x2 / 40]==1 || (level[new_y3 / 40][new_x3 / 40] == 1))//add in for middle and bottom point
		{
			cout << "UP collision!" << endl;
			return 1; //collision!
		}
	}
			if (dir == DOWN) { 		// Moving LEFT
									// Check along the far right side of the sprite, plus 3 (the amount we’re moving)
				new_x1 = x;
				new_x2 = x + BOUNCER_SIZE / 2;
				new_x3 = x + BOUNCER_SIZE;

				// Check at three point along that edge
				new_y1 = y+5 + BOUNCER_SIZE;
				new_y2 = y+5 + BOUNCER_SIZE;
				new_y3 = y+5 + BOUNCER_SIZE;

				if ((level[new_y1 / 40][new_x1 / 40] == 1) || level[new_y2 / 40][new_x2 / 40]==1 || (level[new_y3 / 40][new_x3 / 40] == 1))//add in for middle and bottom point
				{
					cout << "DOWN collision!" << endl;
					return 1; //collision!
				}
	}
	return 0;
}
void ghost::killghost() {
	dead = true;
}

//paste chaser function here
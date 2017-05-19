#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include<iostream>
#include <allegro5/allegro_acodec.h>
#include <Windows.h>
using namespace std;

class ghost {
private:
	int width, height;

	bool dead;
	int dir;
public:
	int xPos, yPos;
	void set_values(int x, int y, int w, int h);
	void drawghost();
	bool isDead();//checks if something is dead
	bool killpac(int b1_x, int b1_y, int b1_w, int b1_h);
	void ghost::chase1(int x, int y, int level[20][20]);
	void ghost::chase2(int x, int y, int level[20][20]);
	void ghost::chase3(int x, int y, int level[20][20]);
	void ghost::chase4(int x, int y, int level[20][20]);
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
const int RIGHT = 3;
const int UP = 2;
const int DOWN = 4;
const int LEFT = 1;
int lives1 = 3;
/* dirs
1 = left
2 = up
3 = right
4 = down*/

//an enumeration is like a user-defined variable, with all the given values it can hold
enum MYKEYS {
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};


int main()
{
	int level[20][20] = {
		1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,
		1,0,0,0,0,1,1,1,0,0,0,1,0,0,0,0,0,0,0,1,
		1,0,1,1,0,1,1,0,0,1,0,0,0,1,1,0,1,1,0,1,
		1,0,1,1,0,1,1,0,1,1,1,1,0,1,1,0,1,1,0,1,
		1,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,1,
		1,0,1,1,0,1,1,0,1,0,1,0,1,1,1,0,0,1,1,1,
		1,0,1,1,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,1,
		1,0,1,1,0,1,1,1,0,1,1,1,0,0,1,0,1,1,0,1,
		1,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,0,1,
		1,0,1,1,1,1,1,0,1,0,1,0,0,0,0,0,1,0,1,1,
		1,0,1,0,0,0,0,0,1,0,1,0,1,1,1,0,0,0,0,1,
		1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,0,0,1,0,1,
		1,0,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,1,1,1,0,0,1,0,1,1,0,1,1,1,0,1,1,0,1,
		1,0,1,1,1,1,0,1,0,1,0,0,0,0,0,0,0,1,0,1,
		1,0,0,0,0,1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,
		1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,
		1,0,1,1,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,1,
		1,0,0,0,0,1,1,1,1,0,0,0,0,1,0,0,0,0,0,1,
		1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,

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

	//sound
	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(2);

	ALLEGRO_SAMPLE *sample1 = NULL; //variable to hold the audio file
	ALLEGRO_SAMPLE *sample2 = NULL;
	ALLEGRO_SAMPLE_INSTANCE *instance1 = NULL;
	ALLEGRO_SAMPLE_INSTANCE *instance2 = NULL;
	//these two functions initalize the two included audio libraries
	al_install_audio();



	al_reserve_samples(2);//decides how many samples we're using, creates a default mixer

						  //returns a memory address to the music file. make sure it's in your project folder!
	sample1 = al_load_sample("background.wav");
	sample2 = al_load_sample("chomp.wav");
	instance1 = al_create_sample_instance(sample1);
	instance2 = al_create_sample_instance(sample2);
	al_set_sample_instance_playmode(instance1, ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_playmode(instance2, ALLEGRO_PLAYMODE_ONCE);

	al_attach_sample_instance_to_mixer(instance1, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(instance2, al_get_default_mixer());
	//end sound 
	//hook up fonts for on-screen text
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_audio();

	ALLEGRO_FONT *font1 = al_load_ttf_font("sleep.ttf", 100, 0);
	ALLEGRO_FONT *font2 = al_load_ttf_font("alice.ttf", 60, 0);
	ALLEGRO_FONT *font3 = al_load_ttf_font("wah.ttf", 80, 0);


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

	al_draw_text(font3, ALLEGRO_COLOR(al_map_rgb(0, 0, 255)), 500, 350, 1, "Ready To Play!");
	al_flip_display();
	al_rest(4);
	al_start_timer(timer);
	ghost ghost1;
	ghost1.set_values(365, 162, 30, 30);
	ghost ghost2;
	ghost2.set_values(365, 200, 30, 30);
	ghost ghost3;
	ghost3.set_values(365, 408, 30, 30);
	ghost ghost4;
	ghost4.set_values(365, 370, 30, 30);

	while (!doexit)

	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		al_play_sample_instance(instance1);
		al_play_sample(sample1, 0, 0.0, 0.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		if (ghost1.isDead() && ghost2.isDead() && ghost3.isDead() && ghost4.isDead()) {
			al_rest(1);
			al_clear_to_color(al_map_rgb(0, 200, 255));
			al_flip_display();
			doexit = true;

		}
		if (lives1 == 0) {
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_flip_display();
			al_draw_text(font1, al_map_rgb(255, 0, 0), 500, 350, ALLEGRO_ALIGN_CENTRE, "Game Over");
			al_flip_display();
			al_rest(3);
			return 0; //kill program

		}
		//if a clock ticks, check if we should be moving a square (by checking key or key2)
		if (ev.type == ALLEGRO_EVENT_TIMER) {
			if (key[KEY_UP] && (wallCollide(bouncer_x, bouncer_y, UP, level) != 1)) {
				bouncer_y -= 4.0;
				//ghost1.killghost(); //uncomment after implementing super dots
			}

			if (key[KEY_DOWN] && (wallCollide(bouncer_x, bouncer_y, DOWN, level) != 1)) {
				bouncer_y += 4.0;
				//ghost2.killghost();
			}

			if (key[KEY_LEFT] && (wallCollide(bouncer_x, bouncer_y, LEFT, level) != 1)) {
				bouncer_x -= 4.0;
				//ghost3.killghost();
			}

			if (key[KEY_RIGHT] && (wallCollide(bouncer_x, bouncer_y, RIGHT, level) != 1)) {
				bouncer_x += 4.0;
				//ghost4.killghost();
			}
			{
				if ((level[bouncer_y / WALLSIZE][bouncer_x / WALLSIZE]) == 0) {
					((level[bouncer_y / WALLSIZE][bouncer_x / WALLSIZE]) = 6);
					al_play_sample_instance(instance2);
					cout << "playing ch9omp" << endl;
				}

			}


			//call chase function
			ghost1.chase1(bouncer_x, bouncer_y, level);
			ghost2.chase2(ghost1.xPos, ghost1.yPos, level);
			ghost3.chase3(bouncer_x, bouncer_y, level);
			ghost4.chase4(bouncer_x, bouncer_y, level);

			//it's simple: we kill the pacman.
			if (ghost1.killpac(bouncer_x, bouncer_y, BOUNCER_SIZE, BOUNCER_SIZE) == 1) {
				lives1--;
				ghost1.xPos = 50;
				ghost1.yPos = 45;
				bouncer_x = 163;
				bouncer_y = 250;
				//play death sound
			}
			if (ghost2.killpac(bouncer_x, bouncer_y, BOUNCER_SIZE, BOUNCER_SIZE) == 1) {
				lives1--;
				ghost2.xPos = 700;
				ghost2.yPos = 45;
				bouncer_x = 163;
				bouncer_y = 250;
				//play death sound
			}
			if (ghost3.killpac(bouncer_x, bouncer_y, BOUNCER_SIZE, BOUNCER_SIZE) == 1) {
				lives1--;
				ghost3.xPos = 700;
				ghost3.yPos = 700;
				bouncer_x = 163;
				bouncer_y = 250;
				//play death sound
			}
			if (ghost4.killpac(bouncer_x, bouncer_y, BOUNCER_SIZE, BOUNCER_SIZE) == 1) {
				lives1--;
				ghost4.xPos = 50;
				ghost4.yPos = 700;
				bouncer_x = 163;
				bouncer_y = 250;
				//play death sound
			}



			redraw = true;
		}
		//kill program if window "X" has been clicked
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			return 0;
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
	al_destroy_sample(sample2);

	return 0;
}

void ghost::set_values(int x, int y, int w, int h) {
	xPos = x;
	yPos = y;
	dir = LEFT;
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
								// Check along the far right side of the sprite, plus 3 (the amount were moving)
		new_x1 = x + 5 + BOUNCER_SIZE;
		new_x2 = x + 5 + BOUNCER_SIZE;
		new_x3 = x + 5 + BOUNCER_SIZE;
		// Check at three point along that edge
		new_y1 = y;
		new_y2 = y + BOUNCER_SIZE / 2;
		new_y3 = y + BOUNCER_SIZE;

		if ((level[new_y1 / 40][new_x1 / 40] == 1) || level[new_y2 / 40][new_x2 / 40] == 1 || (level[new_y3 / 40][new_x3 / 40] == 1))//add in for middle and bottom point
		{
			cout << "right collision!" << endl;
			return 1; //collision!
		}
	}
	if (dir == LEFT) { 		// Moving LEFT
							// Check along the far right side of the sprite, plus 3 (the amount were moving)
		new_x1 = x - 5;
		new_x2 = x - 5;
		new_x3 = x - 5;

		// Check at three point along that edge
		new_y1 = y;
		new_y2 = y + BOUNCER_SIZE / 2;
		new_y3 = y + BOUNCER_SIZE;

		if ((level[new_y1 / 40][new_x1 / 40] == 1) || level[new_y2 / 40][new_x2 / 40] == 1 || (level[new_y3 / 40][new_x3 / 40] == 1))//add in for middle and bottom point
		{
			//cout << "LEFT collision!" << endl;
			return 1; //collision!
		}
	}
	if (dir == UP) { 		// Moving LEFT
							// Check along the far right side of the sprite, plus 3 (the amount were moving)
		new_x1 = x;
		new_x2 = x + BOUNCER_SIZE / 2;
		new_x3 = x + BOUNCER_SIZE;

		// Check at three point along that edge
		new_y1 = y - 5;
		new_y2 = y - 5;
		new_y3 = y - 5;

		if ((level[new_y1 / 40][new_x1 / 40] == 1) || level[new_y2 / 40][new_x2 / 40] == 1 || (level[new_y3 / 40][new_x3 / 40] == 1))//add in for middle and bottom point
		{
			//cout << "UP collision!" << endl;
			return 1; //collision!
		}
	}
	if (dir == DOWN) { 		// Moving LEFT
							// Check along the far right side of the sprite, plus 3 (the amount were moving)
		new_x1 = x;
		new_x2 = x + BOUNCER_SIZE / 2;
		new_x3 = x + BOUNCER_SIZE;

		// Check at three point along that edge
		new_y1 = y + 5 + BOUNCER_SIZE;
		new_y2 = y + 5 + BOUNCER_SIZE;
		new_y3 = y + 5 + BOUNCER_SIZE;

		if ((level[new_y1 / 40][new_x1 / 40] == 1) || level[new_y2 / 40][new_x2 / 40] == 1 || (level[new_y3 / 40][new_x3 / 40] == 1))//add in for middle and bottom point
		{
			cout << "DOWN collision!" << endl;
			return 1; //collision!
		}
	}
	return 0;
}
bool ghost::killpac(int b1_x, int b1_y, int b1_w, int b1_h) {
	if ((b1_x > xPos + 30 - 1) || // is b1 on the right side of b2?
		(b1_y > yPos + 30 - 1) || // is b1 under b2?
		(xPos > b1_x + b1_w - 1) || // is b2 on the right side of b1?
		(yPos > b1_y + b1_h - 1))   // is b2 under b1?
	{
		// no collision
		return 0;

	}

	// collision
	return 1;
}

//chase function. x and y are pacman's position.
void ghost::chase1(int x, int y, int level[20][20]) {


	/* dirs
	1 = left
	2 = up
	3 = right
	4 = down*/

	//this is just for testing
	/*  cout << "state is " << dir << endl;
	if (wallCollide(xPos, yPos, 1, level))
	cout << "there's a wall to the left of me" << endl;
	if (wallCollide(xPos, yPos, 2, level))
	cout << "there's a wall above me" << endl;
	if (wallCollide(xPos, yPos, 3, level))
	cout << "there's a wall to the right of me" << endl;
	if (wallCollide(xPos, yPos, 4, level))
	cout << "there's a wall below me" << endl;*/

	/////////////////////////////////LEFT STATE (1)//////////////////////////////////////////////////////////////////

	//if we're moving left and there's an opening above and pacman is above, move up
	if ((dir == LEFT) && !wallCollide(xPos, yPos, UP, level) && y<yPos)
		while (!wallCollide(xPos, yPos, 2, level)) {
			// cout << "trying to move through hole above!" << endl;
			dir = UP;
			yPos -= 3;
			return;
		}

	//if we're moving left and there's an opening below and pacman is below, move down
	if ((dir == LEFT) && !wallCollide(xPos, yPos, DOWN, level) && y>yPos)
		while (!wallCollide(xPos, yPos, 4, level)) {
			//  cout << "trying to move through hole below!" << endl;
			dir = DOWN;
			yPos += 3;
			return;
		}


	if (dir == LEFT)//left
		while (!wallCollide(xPos, yPos, LEFT, level)) {
			xPos -= 3;
			return;
		}

	////////////////////////////////UP STATE (2)///////////////////////////////////////////////////////////////

	//if we're moving up and there's an opening left and pacman is left, move left
	if ((dir == UP) && !wallCollide(xPos, yPos, LEFT, level) && x<xPos)
		while (!wallCollide(xPos, yPos, LEFT, level)) {
			//   cout << "trying to move through hole to left!" << endl;
			dir = LEFT;
			xPos -= 3;
			return;
		}
	//if we're moving up and there's an opening right and pacman is right, move right
	if ((dir == UP) && !wallCollide(xPos, yPos, RIGHT, level) && x>xPos)
		while (!wallCollide(xPos, yPos, 3, level)) {
			// cout << "trying to move through hole to right!" << endl;
			dir = RIGHT;
			xPos += 3;
			return;
		}
	if (dir == UP)//up
		while (!wallCollide(xPos, yPos, 2, level)) {
			yPos -= UP;
			return;
		}

	/////////RIGHT CASE (3)/////////////////////////////////////////////////////////////////////////////////////////////////////
	//if we're moving right and there's an opening above and pacman is above, move up
	if ((dir == RIGHT) && !wallCollide(xPos, yPos, UP, level) && y<yPos)
		while (!wallCollide(xPos, yPos, 2, level)) {
			//   cout << "trying to move through hole above!" << endl;
			dir = UP;
			yPos -= 3;
			return;
		}

	//if we're moving right and there's an opening below and pacman is below, move down
	if ((dir == RIGHT) && !wallCollide(xPos, yPos, DOWN, level) && y>yPos)
		while (!wallCollide(xPos, yPos, DOWN, level)) {
			//  cout << "trying to move through hole below!" << endl;
			dir = DOWN;
			yPos += 3;
			return;
		}


	if (dir == RIGHT)//right
		while (!wallCollide(xPos, yPos, RIGHT, level)) {
			xPos += 3;
			return;
		}


	//////////////DOWN CASE (4)/////////////////////////////////////////////////////////////////////////////////////
	//if we're moving up and there's an opening left and pacman is left, move left
	if ((dir == DOWN) && !wallCollide(xPos, yPos, LEFT, level) && x<xPos)
		while (!wallCollide(xPos, yPos, LEFT, level)) {
			// cout << "trying to move through hole to left!" << endl;
			dir = LEFT;
			xPos -= 3;
			return;
		}
	//if we're moving up and there's an opening right and pacman is right, move right
	if ((dir == UP) && !wallCollide(xPos, yPos, RIGHT, level) && x>xPos)
		while (!wallCollide(xPos, yPos, RIGHT, level)) {
			//   cout << "trying to move through hole to right!" << endl;
			dir = RIGHT;
			xPos += 3;
			return;
		}

	if (dir == DOWN)//down
		while (!wallCollide(xPos, yPos, DOWN, level)) {
			yPos += 3;
			return;
		}





	///////////////////////////////////////////////////////////////////////////////////////
	//if pacman is above and there's no wall there, move up
	if ((y<yPos) && !wallCollide(xPos, yPos, UP, level)) {
		//  cout << "dir is up" << endl;
		dir = UP;
		return;
	}


	//if pacman is below and there's no wall there, move down
	if ((y>yPos) && !wallCollide(xPos, yPos, DOWN, level)) {
		dir = DOWN;
		//   cout << "dir is down" << endl;
		return;
	}

	//if pacman is right and there's no wall there, move right
	if ((x>xPos) && !wallCollide(xPos, yPos, RIGHT, level)) {
		dir = RIGHT;
		//  cout << "dir is right" << endl;
		return;
	}

	//if pacman is left and there's no wall there, move left
	if ((x<xPos) && !wallCollide(xPos, yPos, LEFT, level)) {
		dir = LEFT;
		// cout << "dir is left" << endl;
		return;

	}

	//if pacman is above and there's no wall there, move up
	if (!wallCollide(xPos, yPos, UP, level)) {
		// cout << "dir is up" << endl;
		dir = UP;
		return;
	}


	//if pacman is below and there's no wall there, move down
	if (!wallCollide(xPos, yPos, DOWN, level)) {
		dir = DOWN;
		//cout << "dir is down" << endl;
		return;
	}

	//if pacman is right and there's no wall there, move right
	if (!wallCollide(xPos, yPos, RIGHT, level)) {
		dir = RIGHT;
		//  cout << "dir is right" << endl;
		return;
	}

	//if pacman is left and there's no wall there, move left
	if (!wallCollide(xPos, yPos, LEFT, level)) {
		dir = LEFT;
		// cout << "dir is left" << endl;
		return;

	}

}
void ghost::chase2(int x, int y, int level[20][20]) {


	/* dirs
	1 = left
	2 = up
	3 = right
	4 = down*/

	//this is just for testing
	/*  cout << "state is " << dir << endl;
	if (wallCollide(xPos, yPos, 1, level))
	cout << "there's a wall to the left of me" << endl;
	if (wallCollide(xPos, yPos, 2, level))
	cout << "there's a wall above me" << endl;
	if (wallCollide(xPos, yPos, 3, level))
	cout << "there's a wall to the right of me" << endl;
	if (wallCollide(xPos, yPos, 4, level))
	cout << "there's a wall below me" << endl;*/

	/////////////////////////////////LEFT STATE (1)//////////////////////////////////////////////////////////////////

	//if we're moving left and there's an opening above and pacman is above, move up
	if ((dir == LEFT) && !wallCollide(xPos, yPos, UP, level) && y<yPos)
		while (!wallCollide(xPos, yPos, 2, level)) {
			// cout << "trying to move through hole above!" << endl;
			dir = UP;
			yPos -= 4;
			return;
		}

	//if we're moving left and there's an opening below and pacman is below, move down
	if ((dir == LEFT) && !wallCollide(xPos, yPos, DOWN, level) && y>yPos)
		while (!wallCollide(xPos, yPos, 4, level)) {
			//  cout << "trying to move through hole below!" << endl;
			dir = DOWN;
			yPos += 4;
			return;
		}


	if (dir == LEFT)//left
		while (!wallCollide(xPos, yPos, LEFT, level)) {
			xPos -= 4;
			return;
		}

	////////////////////////////////UP STATE (2)///////////////////////////////////////////////////////////////

	//if we're moving up and there's an opening left and pacman is left, move left
	if ((dir == UP) && !wallCollide(xPos, yPos, LEFT, level) && x<xPos)
		while (!wallCollide(xPos, yPos, LEFT, level)) {
			//   cout << "trying to move through hole to left!" << endl;
			dir = LEFT;
			xPos -= 4;
			return;
		}
	//if we're moving up and there's an opening right and pacman is right, move right
	if ((dir == UP) && !wallCollide(xPos, yPos, RIGHT, level) && x>xPos)
		while (!wallCollide(xPos, yPos, 3, level)) {
			// cout << "trying to move through hole to right!" << endl;
			dir = RIGHT;
			xPos += 4;
			return;
		}
	if (dir == UP)//up
		while (!wallCollide(xPos, yPos, 2, level)) {
			yPos -= UP;
			return;
		}

	/////////RIGHT CASE (3)/////////////////////////////////////////////////////////////////////////////////////////////////////
	//if we're moving right and there's an opening above and pacman is above, move up
	if ((dir == RIGHT) && !wallCollide(xPos, yPos, UP, level) && y<yPos)
		while (!wallCollide(xPos, yPos, 2, level)) {
			//   cout << "trying to move through hole above!" << endl;
			dir = UP;
			yPos -= 4;
			return;
		}

	//if we're moving right and there's an opening below and pacman is below, move down
	if ((dir == RIGHT) && !wallCollide(xPos, yPos, DOWN, level) && y>yPos)
		while (!wallCollide(xPos, yPos, DOWN, level)) {
			//  cout << "trying to move through hole below!" << endl;
			dir = DOWN;
			yPos += 4;
			return;
		}


	if (dir == RIGHT)//right
		while (!wallCollide(xPos, yPos, RIGHT, level)) {
			xPos += 4;
			return;
		}


	//////////////DOWN CASE (4)/////////////////////////////////////////////////////////////////////////////////////
	//if we're moving up and there's an opening left and pacman is left, move left
	if ((dir == DOWN) && !wallCollide(xPos, yPos, LEFT, level) && x<xPos)
		while (!wallCollide(xPos, yPos, LEFT, level)) {
			// cout << "trying to move through hole to left!" << endl;
			dir = LEFT;
			xPos -= 4;
			return;
		}
	//if we're moving up and there's an opening right and pacman is right, move right
	if ((dir == UP) && !wallCollide(xPos, yPos, RIGHT, level) && x>xPos)
		while (!wallCollide(xPos, yPos, RIGHT, level)) {
			//   cout << "trying to move through hole to right!" << endl;
			dir = RIGHT;
			xPos += 4;
			return;
		}

	if (dir == DOWN)//down
		while (!wallCollide(xPos, yPos, DOWN, level)) {
			yPos += 4;
			return;
		}





	///////////////////////////////////////////////////////////////////////////////////////
	//if pacman is above and there's no wall there, move up
	if ((y<yPos) && !wallCollide(xPos, yPos, UP, level)) {
		//  cout << "dir is up" << endl;
		dir = UP;
		return;
	}


	//if pacman is below and there's no wall there, move down
	if ((y>yPos) && !wallCollide(xPos, yPos, DOWN, level)) {
		dir = DOWN;
		//   cout << "dir is down" << endl;
		return;
	}

	//if pacman is right and there's no wall there, move right
	if ((x>xPos) && !wallCollide(xPos, yPos, RIGHT, level)) {
		dir = RIGHT;
		//  cout << "dir is right" << endl;
		return;
	}

	//if pacman is left and there's no wall there, move left
	if ((x<xPos) && !wallCollide(xPos, yPos, LEFT, level)) {
		dir = LEFT;
		// cout << "dir is left" << endl;
		return;

	}

	//if pacman is above and there's no wall there, move up
	if (!wallCollide(xPos, yPos, UP, level)) {
		// cout << "dir is up" << endl;
		dir = UP;
		return;
	}


	//if pacman is below and there's no wall there, move down
	if (!wallCollide(xPos, yPos, DOWN, level)) {
		dir = DOWN;
		//cout << "dir is down" << endl;
		return;
	}

	//if pacman is right and there's no wall there, move right
	if (!wallCollide(xPos, yPos, RIGHT, level)) {
		dir = RIGHT;
		//  cout << "dir is right" << endl;
		return;
	}

	//if pacman is left and there's no wall there, move left
	if (!wallCollide(xPos, yPos, LEFT, level)) {
		dir = LEFT;
		// cout << "dir is left" << endl;
		return;

	}

}
void ghost::chase3(int x, int y, int level[20][20]) {


	/* dirs
	1 = left
	2 = up
	3 = right
	4 = down*/

	//this is just for testing
	/*  cout << "state is " << dir << endl;
	if (wallCollide(xPos, yPos, 1, level))
	cout << "there's a wall to the left of me" << endl;
	if (wallCollide(xPos, yPos, 2, level))
	cout << "there's a wall above me" << endl;
	if (wallCollide(xPos, yPos, 3, level))
	cout << "there's a wall to the right of me" << endl;
	if (wallCollide(xPos, yPos, 4, level))
	cout << "there's a wall below me" << endl;*/

	/////////////////////////////////LEFT STATE (1)//////////////////////////////////////////////////////////////////

	//if we're moving left and there's an opening above and pacman is above, move up
	if ((dir == LEFT) && !wallCollide(xPos, yPos, UP, level) && y<yPos)
		while (!wallCollide(xPos, yPos, 2, level)) {
			// cout << "trying to move through hole above!" << endl;
			dir = UP;
			yPos -= 4;
			return;
		}

	//if we're moving left and there's an opening below and pacman is below, move down
	if ((dir == LEFT) && !wallCollide(xPos, yPos, DOWN, level) && y>yPos)
		while (!wallCollide(xPos, yPos, 4, level)) {
			//  cout << "trying to move through hole below!" << endl;
			dir = DOWN;
			yPos += 4;
			return;
		}


	if (dir == LEFT)//left
		while (!wallCollide(xPos, yPos, LEFT, level)) {
			xPos -= 4;
			return;
		}

	////////////////////////////////UP STATE (2)///////////////////////////////////////////////////////////////

	//if we're moving up and there's an opening left and pacman is left, move left
	if ((dir == UP) && !wallCollide(xPos, yPos, LEFT, level) && x<xPos)
		while (!wallCollide(xPos, yPos, LEFT, level)) {
			//   cout << "trying to move through hole to left!" << endl;
			dir = LEFT;
			xPos -= 4;
			return;
		}
	//if we're moving up and there's an opening right and pacman is right, move right
	if ((dir == UP) && !wallCollide(xPos, yPos, RIGHT, level) && x>xPos)
		while (!wallCollide(xPos, yPos, 3, level)) {
			// cout << "trying to move through hole to right!" << endl;
			dir = RIGHT;
			xPos += 4;
			return;
		}
	if (dir == UP)//up
		while (!wallCollide(xPos, yPos, 2, level)) {
			yPos -= UP;
			return;
		}

	/////////RIGHT CASE (3)/////////////////////////////////////////////////////////////////////////////////////////////////////
	//if we're moving right and there's an opening above and pacman is above, move up
	if ((dir == RIGHT) && !wallCollide(xPos, yPos, UP, level) && y<yPos)
		while (!wallCollide(xPos, yPos, 2, level)) {
			//   cout << "trying to move through hole above!" << endl;
			dir = UP;
			yPos -= 2;
			return;
		}

	//if we're moving right and there's an opening below and pacman is below, move down
	if ((dir == RIGHT) && !wallCollide(xPos, yPos, DOWN, level) && y>yPos)
		while (!wallCollide(xPos, yPos, DOWN, level)) {
			//  cout << "trying to move through hole below!" << endl;
			dir = DOWN;
			yPos += 2;
			return;
		}


	if (dir == RIGHT)//right
		while (!wallCollide(xPos, yPos, RIGHT, level)) {
			xPos += 2;
			return;
		}


	//////////////DOWN CASE (4)/////////////////////////////////////////////////////////////////////////////////////
	//if we're moving up and there's an opening left and pacman is left, move left
	if ((dir == DOWN) && !wallCollide(xPos, yPos, LEFT, level) && x<xPos)
		while (!wallCollide(xPos, yPos, LEFT, level)) {
			// cout << "trying to move through hole to left!" << endl;
			dir = LEFT;
			xPos -= 4;
			return;
		}
	//if we're moving up and there's an opening right and pacman is right, move right
	if ((dir == UP) && !wallCollide(xPos, yPos, RIGHT, level) && x>xPos)
		while (!wallCollide(xPos, yPos, RIGHT, level)) {
			//   cout << "trying to move through hole to right!" << endl;
			dir = RIGHT;
			xPos += 4;
			return;
		}

	if (dir == DOWN)//down
		while (!wallCollide(xPos, yPos, DOWN, level)) {
			yPos += 4;
			return;
		}





	///////////////////////////////////////////////////////////////////////////////////////
	//if pacman is above and there's no wall there, move up
	if ((y<yPos) && !wallCollide(xPos, yPos, UP, level)) {
		//  cout << "dir is up" << endl;
		dir = UP;
		return;
	}


	//if pacman is below and there's no wall there, move down
	if ((y>yPos) && !wallCollide(xPos, yPos, DOWN, level)) {
		dir = DOWN;
		//   cout << "dir is down" << endl;
		return;
	}

	//if pacman is right and there's no wall there, move right
	if ((x>xPos) && !wallCollide(xPos, yPos, RIGHT, level)) {
		dir = RIGHT;
		//  cout << "dir is right" << endl;
		return;
	}

	//if pacman is left and there's no wall there, move left
	if ((x<xPos) && !wallCollide(xPos, yPos, LEFT, level)) {
		dir = LEFT;
		// cout << "dir is left" << endl;
		return;

	}

	//if pacman is above and there's no wall there, move up
	if (!wallCollide(xPos, yPos, UP, level)) {
		// cout << "dir is up" << endl;
		dir = UP;
		return;
	}


	//if pacman is below and there's no wall there, move down
	if (!wallCollide(xPos, yPos, DOWN, level)) {
		dir = DOWN;
		//cout << "dir is down" << endl;
		return;
	}

	//if pacman is right and there's no wall there, move right
	if (!wallCollide(xPos, yPos, RIGHT, level)) {
		dir = RIGHT;
		//  cout << "dir is right" << endl;
		return;
	}

	//if pacman is left and there's no wall there, move left
	if (!wallCollide(xPos, yPos, LEFT, level)) {
		dir = LEFT;
		// cout << "dir is left" << endl;
		return;

	}

}
void ghost::chase4(int x, int y, int level[20][20]) {


	/* dirs
	1 = left
	2 = up
	3 = right
	4 = down*/

	//this is just for testing
	/*  cout << "state is " << dir << endl;
	if (wallCollide(xPos, yPos, 1, level))
	cout << "there's a wall to the left of me" << endl;
	if (wallCollide(xPos, yPos, 2, level))
	cout << "there's a wall above me" << endl;
	if (wallCollide(xPos, yPos, 3, level))
	cout << "there's a wall to the right of me" << endl;
	if (wallCollide(xPos, yPos, 4, level))
	cout << "there's a wall below me" << endl;*/

	/////////////////////////////////LEFT STATE (1)//////////////////////////////////////////////////////////////////

	//if we're moving left and there's an opening above and pacman is above, move up
	if ((dir == LEFT) && !wallCollide(xPos, yPos, UP, level) && y<yPos)
		while (!wallCollide(xPos, yPos, 2, level)) {
			// cout << "trying to move through hole above!" << endl;
			dir = UP;
			yPos -= 4;
			return;
		}

	//if we're moving left and there's an opening below and pacman is below, move down
	if ((dir == LEFT) && !wallCollide(xPos, yPos, DOWN, level) && y>yPos)
		while (!wallCollide(xPos, yPos, 4, level)) {
			//  cout << "trying to move through hole below!" << endl;
			dir = DOWN;
			yPos += 4;
			return;
		}


	if (dir == LEFT)//left
		while (!wallCollide(xPos, yPos, LEFT, level)) {
			xPos -= 4;
			return;
		}

	////////////////////////////////UP STATE (2)///////////////////////////////////////////////////////////////

	//if we're moving up and there's an opening left and pacman is left, move left
	if ((dir == UP) && !wallCollide(xPos, yPos, LEFT, level) && x<xPos)
		while (!wallCollide(xPos, yPos, LEFT, level)) {
			//   cout << "trying to move through hole to left!" << endl;
			dir = LEFT;
			xPos -= 4;
			return;
		}
	//if we're moving up and there's an opening right and pacman is right, move right
	if ((dir == UP) && !wallCollide(xPos, yPos, RIGHT, level) && x>xPos)
		while (!wallCollide(xPos, yPos, 3, level)) {
			// cout << "trying to move through hole to right!" << endl;
			dir = RIGHT;
			xPos += 4;
			return;
		}
	if (dir == UP)//up
		while (!wallCollide(xPos, yPos, 2, level)) {
			yPos -= UP;
			return;
		}

	/////////RIGHT CASE (3)/////////////////////////////////////////////////////////////////////////////////////////////////////
	//if we're moving right and there's an opening above and pacman is above, move up
	if ((dir == RIGHT) && !wallCollide(xPos, yPos, UP, level) && y<yPos)
		while (!wallCollide(xPos, yPos, 2, level)) {
			//   cout << "trying to move through hole above!" << endl;
			dir = UP;
			yPos -= 2;
			return;
		}

	//if we're moving right and there's an opening below and pacman is below, move down
	if ((dir == RIGHT) && !wallCollide(xPos, yPos, DOWN, level) && y>yPos)
		while (!wallCollide(xPos, yPos, DOWN, level)) {
			//  cout << "trying to move through hole below!" << endl;
			dir = DOWN;
			yPos += 2;
			return;
		}


	if (dir == RIGHT)//right
		while (!wallCollide(xPos, yPos, RIGHT, level)) {
			xPos += 2;
			return;
		}


	//////////////DOWN CASE (4)/////////////////////////////////////////////////////////////////////////////////////
	//if we're moving up and there's an opening left and pacman is left, move left
	if ((dir == DOWN) && !wallCollide(xPos, yPos, LEFT, level) && x<xPos)
		while (!wallCollide(xPos, yPos, LEFT, level)) {
			// cout << "trying to move through hole to left!" << endl;
			dir = LEFT;
			xPos -= 2;
			return;
		}
	//if we're moving up and there's an opening right and pacman is right, move right
	if ((dir == UP) && !wallCollide(xPos, yPos, RIGHT, level) && x>xPos)
		while (!wallCollide(xPos, yPos, RIGHT, level)) {
			//   cout << "trying to move through hole to right!" << endl;
			dir = RIGHT;
			xPos += 2;
			return;
		}

	if (dir == DOWN)//down
		while (!wallCollide(xPos, yPos, DOWN, level)) {
			yPos += 2;
			return;
		}





	///////////////////////////////////////////////////////////////////////////////////////
	//if pacman is above and there's no wall there, move up
	if ((y<yPos) && !wallCollide(xPos, yPos, UP, level)) {
		//  cout << "dir is up" << endl;
		dir = UP;
		return;
	}


	//if pacman is below and there's no wall there, move down
	if ((y>yPos) && !wallCollide(xPos, yPos, DOWN, level)) {
		dir = DOWN;
		//   cout << "dir is down" << endl;
		return;
	}

	//if pacman is right and there's no wall there, move right
	if ((x>xPos) && !wallCollide(xPos, yPos, RIGHT, level)) {
		dir = RIGHT;
		//  cout << "dir is right" << endl;
		return;
	}

	//if pacman is left and there's no wall there, move left
	if ((x<xPos) && !wallCollide(xPos, yPos, LEFT, level)) {
		dir = LEFT;
		// cout << "dir is left" << endl;
		return;

	}

	//if pacman is above and there's no wall there, move up
	if (!wallCollide(xPos, yPos, UP, level)) {
		// cout << "dir is up" << endl;
		dir = UP;
		return;
	}


	//if pacman is below and there's no wall there, move down
	if (!wallCollide(xPos, yPos, DOWN, level)) {
		dir = DOWN;
		//cout << "dir is down" << endl;
		return;
	}

	//if pacman is right and there's no wall there, move right
	if (!wallCollide(xPos, yPos, RIGHT, level)) {
		dir = RIGHT;
		//  cout << "dir is right" << endl;
		return;
	}

	//if pacman is left and there's no wall there, move left
	if (!wallCollide(xPos, yPos, LEFT, level)) {
		dir = LEFT;
		// cout << "dir is left" << endl;
		return;

	}

}





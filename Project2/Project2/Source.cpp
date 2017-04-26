#include<iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
using namespace std;
const int RIGHT = 1;
const int LEFT = 1;
const int UP = 1;
const int DOWN = 1;
const int PACSIZE = 32;
int main() {

	//these two variables hold the x and y positions of the square
	//initalize these variables to where you want your square to start
	int pacman_x = 45;
	int pacman_y = 45;
	float Dot_x = 3;
	float Dot_y = 3;
	int wallCollide(int x, int y, int dir, int level[20][20]);
	
	enum MYKEYS {
		KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
	};

	int level[20][20]{
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1,
		1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1,
		1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1,
		1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1,
		1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1,
		1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1,
		1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1,
		1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1,
		1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1,
		1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1,
		1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1,
		1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1,
		1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1,
		1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1,
		1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1,
		1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1,
		1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1,
		1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,




	};

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *pacman = NULL;
	ALLEGRO_BITMAP *Platform = NULL;
	ALLEGRO_BITMAP *Dot = NULL;

	//here's our key states. they're all starting as "false" because nothing has been pressed yet.
	//the first slot represents "up", then "down", "left" and "right"
	bool key[4] = { false, false, false, false };

	//don't redraw until an event happens
	bool redraw = true;

	//this controls our game loop
	bool doexit = false;

	al_init();
	al_init_primitives_addon();

	//get the keyboard ready to use
	al_install_keyboard();

	timer = al_create_timer(.02);

	display = al_create_display(800, 800);

	pacman = al_create_bitmap(32, 32);

	Platform = al_create_bitmap(40, 40);

	al_set_target_bitmap(pacman);

	al_clear_to_color(al_map_rgb(255, 255, 0));

	al_set_target_bitmap(Platform);

	al_clear_to_color(al_map_rgb(0, 0, 255));

	Dot = al_create_bitmap(3, 3);

	al_set_target_bitmap(Dot);

	al_clear_to_color(al_map_rgb(255, 255, 255));

	al_set_target_bitmap(al_get_backbuffer(display));

	event_queue = al_create_event_queue();

	//these lines tell teh event source what to look for
	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	//new! tell the event queue that it should take keyboard events, too 
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_flip_display();

	al_start_timer(timer);


	//so the game loop is set to act on "ticks" of the timer OR keyboard presses 
	//OR the mouse closing the display
	while (!doexit)
	{
		cout << "game loop start" << endl;
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//here's the movement algorithm

		if (ev.type == ALLEGRO_EVENT_TIMER) {
			//if the up button is pressed AND we're still below the top wall,
			//move the box "up" by 4 pixels
			if (key[0] && wallCollide(pacman_x, pacman_y, UP, level) != 1) {
				pacman_y -= 4.0;
			}

			//if the down button is pressed AND we're still above the bottom wall
			//move the box "down" by 4 pixels
			if (key[1] && wallCollide(pacman_x, pacman_y, DOWN, level) != 1) {
				pacman_y += 4.0;
			}
			//if the left button is pressed AND we're still right of the left wall
			//move the box left by 4 pixels
			if (key[2] && wallCollide(pacman_x, pacman_y, RIGHT, level) != 1) {
				pacman_x -= 4.0;
			}

			//if the left button is pressed AND we're still left of the right wall
			//move the box right by 4 pixels
			if (key[3] && wallCollide(pacman_x, pacman_y, LEFT, level) != 1) {
				pacman_x += 4.0;
			}

			{
				if ((level[pacman_y / 40][pacman_x / 40]) == 0)

					((level[pacman_y / 40][pacman_x / 40]) = 2);


			}
			cout << "redraw is true" << endl;
			//redraw at every tick of the timer
			redraw = true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//here's the algorithm that turns key presses into events
		//a "key down" event is when a key is pushed
		//while a "key up" event is when a key is released

		//has something been pressed on the keyboard?
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {

			//"keycode" holds all the different keys on the keyboard
			switch (ev.keyboard.keycode) {

				//if the up key has been pressed
			case ALLEGRO_KEY_UP:
				key[0] = true;
				break;

				//if the down key has been pressed
			case ALLEGRO_KEY_DOWN:
				key[1] = true;
				break;

				//if the left key has been pressed
			case ALLEGRO_KEY_LEFT:
				key[2] = true;
				break;

				//if the right key has been pressed
			case ALLEGRO_KEY_RIGHT:
				key[3] = true;
				break;
			}
		}
		//has something been released on the keyboard?
		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				key[0] = false;
				break;

			case ALLEGRO_KEY_DOWN:
				key[1] = false;
				break;

			case ALLEGRO_KEY_LEFT:
				key[2] = false;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[3] = false;
				break;

				//kill the program if someone presses escape
			case ALLEGRO_KEY_ESCAPE:
				doexit = true;
				break;
			}
		}


		//RENDER SECTION
		//if the clock ticked but no other events happened, don't bother redrawing
		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;
			cout << "render section!" << endl;

			//paint black over the old screen, so the old square dissapears
			al_clear_to_color(al_map_rgb(0, 0, 0));
			cout << "flag1" << endl;
			//the algorithm above just changes the x and y coordinates
			//here's where the bitmap is actually drawn somewhere else
			al_draw_bitmap(pacman, pacman_x, pacman_y, 0);
			cout << "flag2" << endl;
			for (int i = 0; i < 20; i++) {
				for (int j = 0; j < 20; j++)
					if (level[j][i] == 1)
						al_draw_bitmap(Platform, i * 40, j * 40, 0);
			}
			for (int i = 0; i < 20; i++) 
				for (int j = 0; j < 20; j++){
					if (level[j][i] == 0)
						al_draw_bitmap(Dot, i * 40 + 20, j * 40 + 20, 0);

			}
			cout << "flag3" << endl;
			al_flip_display();
		}//end render section
	}//end main

	cout << "end game!" << endl;
	al_destroy_bitmap(pacman);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}
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
		new_x1 = x + 5 + PACSIZE;
		new_x2 = x + 5 + PACSIZE;
		new_x3 = x + 5 + PACSIZE;
		// Check at three point along that edge
		new_y1 = y;
		new_y2 = y + PACSIZE / 2;
		new_y3 = y + PACSIZE;

		if ((level[new_y1 / 40][new_x1 / 40] == 1) || level[new_y2 / 40][new_x2 / 40] == 1 || (level[new_y3 / 40][new_x3 / 40] == 1))//add in for middle and bottom point
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
		new_y2 = y + PACSIZE / 2;
		new_y3 = y + PACSIZE;

		if ((level[new_y1 / 40][new_x1 / 40] == 1) || level[new_y2 / 40][new_x2 / 40] == 1 || (level[new_y3 / 40][new_x3 / 40] == 1))//add in for middle and bottom point
		{
			cout << "LEFT collision!" << endl;
			return 1; //collision!
		}
	}
	if (dir == UP) { 		// Moving LEFT
							// Check along the far right side of the sprite, plus 3 (the amount we’re moving)
		new_x1 = x;
		new_x2 = x + PACSIZE / 2;
		new_x3 = x + PACSIZE;

		// Check at three point along that edge
		new_y1 = y - 5;
		new_y2 = y - 5;
		new_y3 = y - 5;

		if ((level[new_y1 / 40][new_x1 / 40] == 1) || level[new_y2 / 40][new_x2 / 40] == 1 || (level[new_y3 / 40][new_x3 / 40] == 1))//add in for middle and bottom point
		{
			cout << "UP collision!" << endl;
			return 1; //collision!
		}
	}
	if (dir == DOWN) { 		// Moving LEFT
							// Check along the far right side of the sprite, plus 3 (the amount we’re moving)
		new_x1 = x;
		new_x2 = x + PACSIZE / 2;
		new_x3 = x + PACSIZE;

		// Check at three point along that edge
		new_y1 = y + 5 + PACSIZE;
		new_y2 = y + 5 + PACSIZE;
		new_y3 = y + 5 + PACSIZE;

		if ((level[new_y1 / 40][new_x1 / 40] == 1) || level[new_y2 / 40][new_x2 / 40] == 1 || (level[new_y3 / 40][new_x3 / 40] == 1))//add in for middle and bottom point
		{
			cout << "DOWN collision!" << endl;
			return 1; //collision!
		}
	}
	return 0;
} //end collision function definition​
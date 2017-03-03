#include <stdio.h>
#include <allegro5/allegro.h>
#include <iostream>
#include <allegro5/allegro_primitives.h>

int main()
{
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *padle = NULL;
	ALLEGRO_BITMAP *padle2 = NULL;
	ALLEGRO_BITMAP *ball = NULL;
	ALLEGRO_DISPLAY *display = NULL;
	//here's the bouncer's x and y coordinates on the screen
	float padle_x = 30;
	float padle_y = 30;
	float padle2_x = 400;
	float padle2_y = 400;
	float ball_x = 90;
	float ball_y = 90;
	float ball_dx = -4.0, ball_dy = 4.0;
	int bounding_box_collision(int b1_x, int b1_y, int b1_w, int b1_h, int b2_x, int b2_y, int b2_w, int b2_h);


	al_init();

	al_init_primitives_addon();

	timer = al_create_timer(.02);


	display = al_create_display(640, 480);

	al_clear_to_color(al_map_rgb(0, 0, 0));


	//here's our key states. they're all starting as "false" because nothing has been pressed yet.
	//the first slot represents "up", then "down", "left" and "right"
	//key2 represents"w","a","s","d"
	bool key[2] = { false, false, };

	bool key2[2] = { false, false, };
	bool redraw = true;




	//this controls our game loop
	bool doexit = false;


	al_init();


	//get the keyboard ready to use
	al_install_keyboard();

	timer = al_create_timer(.02);

	/////
	display = al_create_display(640, 480);
	////////ball
	ball = al_create_bitmap(25, 25);

	al_set_target_bitmap(ball);

	al_clear_to_color(al_map_rgb(150, 0, 150));
	////
	padle = al_create_bitmap(150, 32);

	al_set_target_bitmap(padle);

	al_clear_to_color(al_map_rgb(0, 200, 255));
	////2nd box green
	padle2 = al_create_bitmap(150, 32);

	al_set_target_bitmap(padle2);

	al_clear_to_color(al_map_rgb(0, 255, 100));

	al_set_target_bitmap(al_get_backbuffer(display));

	event_queue = al_create_event_queue();

	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	//tell the event queue that it should take keyboard events, too 
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_flip_display();

	al_start_timer(timer);

	//so the game loop is set to act on "ticks" of the timer OR keyboard presses 
	//OR the mouse closing the display
	while (!doexit)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//here's the movement algorithm

		if (ev.type == ALLEGRO_EVENT_TIMER) {

			//if the left button is pressed AND we're still right of the left wall
			//move the box left by 4 pixels
			if (key[0] && padle_x >= 0) {
				padle_x -= 5.0;
			}

			//if the right button is pressed AND we're still left of the right wall
			//move the box right by 4 pixels
			if (key[1] && padle_x <= 640 - 32) {
				padle_x += 5.0;
			}



			if (ball_x < 0 || ball_x > 640 - 32) {
				//flip the x direction
				ball_dx = -ball_dx;
			}

			//redraw at every tick of the timer
			redraw = true;



			if (key2[0] && padle2_x >= 0) {
				padle2_x -= 5.0;
			}


			if (key2[1] && padle2_x <= 640 - 32) {
				padle2_x += 5.0;
			}
			//if the box hits the top wall OR the bottom wall
			if (ball_y < 0 || ball_y > 480 - 32) {
				//flip the y direction
				ball_dy = -ball_dy;
			}

			//really important code!
			//move the box in a diagonal
			ball_x += ball_dx;
			ball_y += ball_dy;

			redraw = true;
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////collsion

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



				//if the left key has been pressed
			case ALLEGRO_KEY_LEFT:
				key[0] = true;
				break;

				//if the right key has been pressed
			case ALLEGRO_KEY_RIGHT:
				key[1] = true;
				break;



			case ALLEGRO_KEY_A:
				key2[0] = true;
				break;

			case ALLEGRO_KEY_D:
				key2[1] = true;
				break;
			}
		}
		//has something been released on the keyboard?
		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {


			case ALLEGRO_KEY_LEFT:
				key[0] = false;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[1] = false;
				break;



			case ALLEGRO_KEY_A:
				key2[0] = false;
				break;

			case ALLEGRO_KEY_D:
				key2[1] = false;
				break;

				//kill the program if someone presses escape
			case ALLEGRO_KEY_ESCAPE:
				doexit = true;
				break;

			}

		}
	al_clear_to_color(al_map_rgb(0, 0, 0));
		//if the clock ticked but no other events happened, don't bother redrawing
	if (redraw && al_is_event_queue_empty(event_queue)) {
		redraw = false;

		//paint black over the old screen, so the old square dissapears
		al_clear_to_color(al_map_rgb(0, 0, 0));

		//the algorithm above just changes the x and y coordinates
		//here's where the bitmap is actually drawn somewhere else
		al_draw_bitmap(padle, padle_x, padle_y, 0);

		al_draw_bitmap(padle2, padle2_x, padle2_y, 0);

		al_draw_bitmap(ball, ball_x, ball_y, 0);


		if (bounding_box_collision(padle_x, padle_y, 150, 30, ball_x, ball_y, 50, 50)) {
			if (ball_y < ball_y || ball_y > padle_y - 32) {
				//flip the y direction
				ball_dy = -ball_dy;
			}
		}
		if (bounding_box_collision(padle2_x, padle2_y, 150, 30, ball_x, ball_y, 50, 50)) {
			if (ball_y < padle2_y || ball_y > padle2_y - 32) {
				//flip the y direction
				ball_dy = -ball_dy;
			}
			al_flip_display();

		}
	}
	
}//end game loop

	al_destroy_bitmap(padle);
	al_destroy_bitmap(padle2);
	al_destroy_bitmap(ball);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}
int bounding_box_collision(int b1_x, int b1_y, int b1_w, int b1_h, int b2_x, int b2_y, int b2_w, int b2_h)
{
	if ((b1_x > b2_x + b2_w - 1) || // is b1 on the right side of b2?
		(b1_y > b2_y + b2_h - 1) || // is b1 under b2?
		(b2_x > b1_x + b1_w - 1) || // is b2 on the right side of b1?
		(b2_y > b1_y + b1_h - 1))   // is b2 under b1?
	{
		// no collision
		return 0;
	}

	// collision
	return 1;
}
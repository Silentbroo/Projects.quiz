#include <stdio.h>
#include <allegro5/allegro.h>
#include <iostream>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <time.h>

int main()
{
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *padle = NULL;
	ALLEGRO_BITMAP *ball = NULL;
	ALLEGRO_DISPLAY *display = NULL;


	//here's the bouncer's x and y coordinates on the screen

	float padle_x = 450;
	float padle_y = 450;

	float ball_x = 100;
	float ball_y = 100;
	float ball_dx = -4.0, ball_dy = 4.0;
	int bounding_box_collision(int b1_x, int b1_y, int b1_w, int b1_h, int b2_x, int b2_y, int b2_w, int b2_h);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	al_init();

	al_init_primitives_addon();

	timer = al_create_timer(.02);

	display = al_create_display(640, 480);
	//al_clear_to_color(al_map_rgb(0, 0, 0));

	//get the keyboard ready to use
	al_install_keyboard();



	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//here's our key states. they're all starting as "false" because nothing has been pressed yet.
	//the first slot represents "up", then "down", "left" and "right"
	//key2 represents"w","a","s","d"
	bool key[2] = { false, false, };

	bool redraw = true;



	//this controls our game loop
	bool doexit = false;

	////////ball
	ball = al_create_bitmap(25, 25);

	al_set_target_bitmap(ball);

	al_clear_to_color(al_map_rgb(150, 0, 150));
	////
	padle = al_create_bitmap(150, 32);

	al_set_target_bitmap(padle);

	al_clear_to_color(al_map_rgb(0, 200, 255));




	al_set_target_bitmap(al_get_backbuffer(display));

	event_queue = al_create_event_queue();

	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	//tell the event queue that it should take keyboard events, too 
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_flip_display();

	al_start_timer(timer);





	while (!doexit)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);


	


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//here's the movement algorithm

		if (ev.type == ALLEGRO_EVENT_TIMER) { //timer section handles movement and physics

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

		
			//if the box hits the top wall OR the bottom wall
			if (ball_y > 480 - 32) {
				//flip the y direction
				ball_x = 250;
				ball_y = 250;
				ball_dx = -5.0, ball_dy = 5.0;
			}
			if (ball_y < 0 - 32) {
				//flip the y direction
				ball_x = 250;
				ball_y = 250;
				ball_dx = -5.0, ball_dy = 5.0;
			}


			//collision
			if (bounding_box_collision(padle_x, padle_y, 150, 32, ball_x, ball_y, 25, 25)) {

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

			//paint black over the old screen, so the old square dissapears
			al_clear_to_color(al_map_rgb(0, 0, 0));

			//the algorithm above just changes the x and y coordinates
			//here's where the bitmap is actually drawn somewhere else

			al_draw_bitmap(padle, padle_x, padle_y, 0);

		

			al_draw_bitmap(ball, ball_x, ball_y, 0);
		




			al_flip_display();


		}

	}//end game loop

	al_destroy_bitmap(padle);

	al_destroy_bitmap(ball);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);


	return 0;
}
int bounding_box_collision(int b1_x, int b1_y, int b1_w, int b1_h, int b2_x, int b2_y, int b2_w, int b2_h)
{
	if ((b1_x > b2_x + b2_w) || // is b1 on the right side of b2?
		(b1_y > b2_y + b2_h) || // is b1 under b2?
		(b2_x > b1_x + b1_w) || // is b1 on the left side of b2?
		(b2_y > b1_y + b1_h))   // is b1 uabove b2?
	{
		// no collision
		return 0;
	}

	// collision
	return 1;
}

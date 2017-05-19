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

class Brick {
private:
	int width, height;
	int xPos, yPos;
	bool dead;
public:
	void set_values(int x, int y, int w, int h);
	void drawBrick();
	bool isDead();//checks if something is dead
	void killBrick();//kills brick
};


int main()
{
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *padle = NULL;
	ALLEGRO_BITMAP *ball = NULL;
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_BITMAP *image1 = NULL;
	ALLEGRO_BITMAP *image2 = NULL;

	//here's the bouncer's x and y coordinates on the screen
	int lives1 = 6;
	int score = 0;
	float padle_x = 250;
	float padle_y = 867;
	float ball_x = 90;
	float ball_y = 90;
	int bounding_box_collision(int b1_x, int b1_y, int b1_w, int b1_h, int b2_x, int b2_y, int b2_w, int b2_h);
	int speed1 = 12;
	int speed2 = 10;
	float ball_dx = -speed2, ball_dy = speed2;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	al_init();

	al_init_primitives_addon();

	timer = al_create_timer(.02);
	al_init_image_addon();
	display = al_create_display(1000, 900);
	//al_clear_to_color(al_map_rgb(0, 0, 0));
	void al_set_new_window_title(const char *hello);
	//get the keyboard ready to use
	al_install_keyboard();

	//hook up fonts for on-screen text
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_audio();

	ALLEGRO_FONT *font1 = al_load_ttf_font("sleep.ttf", 100, 0);
	ALLEGRO_FONT *font2 = al_load_ttf_font("alice.ttf", 60, 0);
	ALLEGRO_FONT *font3 = al_load_ttf_font("wah.ttf", 80, 0);
	//sound
	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(10);



	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//here's our key states. they're all starting as "false" because nothing has been pressed yet.
	//the first slot represents "up", then "down", "left" and "right"
	//key2 represents"w","a","s","d"
	bool key[2] = { false, false, };

	bool key2[2] = { false, false, };
	bool redraw = true;

	image1 = al_load_bitmap("alexis.jpg");


	//this controls our game loop
	bool doexit = false;

	////////ball
	ball = al_create_bitmap(25, 25);

	al_set_target_bitmap(ball);

	al_clear_to_color(al_map_rgb(255, 255, 255));
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
	
		al_draw_text(font3, ALLEGRO_COLOR(al_map_rgb(0, 0, 255)), 500, 350, 1, "Ready To Play!");
		al_flip_display();
		al_rest(4);
	
	//so the game loop is set to act on "ticks" of the timer OR keyboard presses 
	//OR the mouse closing the display
	ALLEGRO_SAMPLE *sample1 = NULL; //variable to hold the audio file
	ALLEGRO_SAMPLE *sample2 = NULL;
	ALLEGRO_SAMPLE *sample3 = NULL;
	ALLEGRO_SAMPLE *sample4 = NULL;
	ALLEGRO_SAMPLE *sample5 = NULL;
	ALLEGRO_SAMPLE_INSTANCE *instance1 = NULL;
	ALLEGRO_SAMPLE_INSTANCE *instance2 = NULL;
	ALLEGRO_SAMPLE_INSTANCE *instance3 = NULL;
	ALLEGRO_SAMPLE_INSTANCE *instance4 = NULL;
	ALLEGRO_SAMPLE_INSTANCE *instance5 = NULL;
	


	//these two functions initalize the two included audio libraries
	al_install_audio();

	al_init_acodec_addon();

	al_reserve_samples(5);//decides how many samples we're using, creates a default mixer

						  //returns a memory address to the music file. make sure it's in your project folder!
	sample1 = al_load_sample("ded.wav");
	sample2 = al_load_sample("sc.wav");
	sample3 = al_load_sample("tj.wav");
	sample4 = al_load_sample("win.wav");
	sample5 = al_load_sample("hip.wav");


	instance1 = al_create_sample_instance(sample1);
	instance2 = al_create_sample_instance(sample2);
	instance3 = al_create_sample_instance(sample3);
	instance4 = al_create_sample_instance(sample4);
	instance5 = al_create_sample_instance(sample5);
	

	al_set_sample_instance_playmode(instance5, ALLEGRO_PLAYMODE_LOOP);

	al_attach_sample_instance_to_mixer(instance1, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(instance2, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(instance3, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(instance4, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(instance5, al_get_default_mixer());
	

	//end audio stuff

	//instantiate (stamp) your bricks
	/////////////////////////////ROW 1//////////////////
	Brick Brick1;
	Brick1.set_values(0, 0, 100, 40);
	Brick Brick2;
	Brick2.set_values(110, 0, 100, 40);
	Brick Brick3;
	Brick3.set_values(220, 0, 100, 40);
	Brick Brick4;
	Brick4.set_values(330, 0, 100, 40);
	Brick Brick5;
	Brick5.set_values(440, 0, 100, 40);
	Brick Brick6;
	Brick6.set_values(550, 0, 100, 40);
	Brick Brick7;
	Brick7.set_values(660, 0, 100, 40);
	Brick Brick8;
	Brick8.set_values(770, 0, 100, 40);
	Brick Brick9;
	Brick9.set_values(880, 0, 100, 40);
	/////////////////////////////ROW 2//////////////////
	Brick Brick10;
	Brick10.set_values(0, 50, 100, 40);
	Brick Brick11;
	Brick11.set_values(110, 50, 100, 40);
	Brick Brick12;
	Brick12.set_values(220, 50, 100, 40);
	Brick Brick13;
	Brick13.set_values(330, 50, 100, 40);
	Brick Brick14;
	Brick14.set_values(440, 50, 100, 40);
	Brick Brick15;
	Brick15.set_values(550, 50, 100, 40);
	Brick Brick16;
	Brick16.set_values(660, 50, 100, 40);
	Brick Brick17;
	Brick17.set_values(770, 50, 100, 40);
	Brick Brick18;
	Brick18.set_values(880, 50, 100, 40);
	/////////////////////////////ROW 3//////////////////
	Brick Brick19;
	Brick19.set_values(0, 100, 100, 40);
	Brick Brick20;
	Brick20.set_values(110, 100, 100, 40);
	Brick Brick21;
	Brick21.set_values(220, 100, 100, 40);
	Brick Brick22;
	Brick22.set_values(330, 100, 100, 40);
	Brick Brick23;
	Brick23.set_values(440, 100, 100, 40);
	Brick Brick24;
	Brick24.set_values(550, 100, 100, 40);
	Brick Brick25;
	Brick25.set_values(660, 100, 100, 40);
	Brick Brick26;
	Brick26.set_values(770, 100, 100, 40);
	Brick Brick27;
	Brick27.set_values(880, 100, 100, 40);


	while (!doexit)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		al_play_sample_instance(instance5);
		al_play_sample(sample5, 0, 0.0, 0.0, ALLEGRO_PLAYMODE_ONCE, NULL);

		if (lives1 == 0) {
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_flip_display();
			al_draw_text(font2, al_map_rgb(0, 255, 255), 754, 450, ALLEGRO_ALIGN_CENTRE, "Score:");
			al_draw_textf(font2, al_map_rgb(0, 255, 255), 890, 450, ALLEGRO_ALIGN_CENTRE, "%d", score);
			al_draw_text(font1, al_map_rgb(255, 0, 0), 500, 350, ALLEGRO_ALIGN_CENTRE, "Game Over");
			al_flip_display();
			al_rest(3);
			return 0; //kill program

		}

		if (Brick1.isDead() && Brick2.isDead() && Brick3.isDead() && Brick4.isDead() && Brick5.isDead() && Brick6.isDead() && Brick7.isDead() && Brick8.isDead() && Brick9.isDead() && Brick10.isDead() && Brick11.isDead() && Brick12.isDead() && Brick13.isDead() && Brick14.isDead() && Brick15.isDead() && Brick16.isDead() && Brick17.isDead() && Brick18.isDead() && Brick19.isDead() && Brick20.isDead() && Brick21.isDead() && Brick22.isDead() && Brick23.isDead() && Brick24.isDead() && Brick25.isDead() && Brick26.isDead() && Brick27.isDead()) {
			al_rest(1);
			al_play_sample_instance(instance3);
			al_clear_to_color(al_map_rgb(0, 200, 255));
			al_flip_display();
			al_draw_text(font1, al_map_rgb(255, 255, 255), 500, 350, ALLEGRO_ALIGN_CENTRE, "You Finally Won !!");
			al_flip_display();
			al_rest(3);
			doexit = true;

		}

		if (!Brick1.isDead() && bounding_box_collision(0, 0, 100, 40, ball_x, ball_y, 50, 50)) {
			if (ball_y < padle_y || padle_y > padle_y - 32) {
				//flip the y direction
				al_play_sample_instance(instance3);
				ball_dy = -ball_dy;
				Brick1.killBrick(); //when you hit it, kill it
				score += 10;

			}
		}
		if (!Brick2.isDead() && bounding_box_collision(110, 0, 100, 40, ball_x, ball_y, 50, 50)) {
			if (ball_y < padle_y || ball_y > padle_y - 32) {
				//flip the y direction
				al_play_sample_instance(instance3);
				ball_dy = -ball_dy;
				Brick2.killBrick();
				score += 10;
			}
		}
		if (!Brick3.isDead() && bounding_box_collision(220, 0, 100, 40, ball_x, ball_y, 50, 50)) {
			if (ball_y < padle_y ||ball_y >padle_y - 32) {
				//flip the y direction
				al_play_sample_instance(instance3);
				ball_dy = -ball_dy;
				Brick3.killBrick();
				score += 10;
			}
		}
		if (!Brick4.isDead() && bounding_box_collision(330, 0, 100, 40, ball_x, ball_y, 50, 50)) {
			if (ball_y < padle_y || ball_y > padle_y - 32) {
				//flip the y direction
				al_play_sample_instance(instance3);
				ball_dy = -ball_dy;
				Brick4.killBrick();
				score += 10;
			}
		}
		if (!Brick5.isDead() && bounding_box_collision(440, 0, 100, 40, ball_x, ball_y, 50, 50)) {
			if (ball_y < padle_y || ball_y > padle_y - 32) {
				//flip the y direction
				al_play_sample_instance(instance3);
				ball_dy = -ball_dy;
				Brick5.killBrick();
				score += 10;
			}
		}
		if (!Brick6.isDead() && bounding_box_collision(550, 0, 100, 40, ball_x, ball_y, 50, 50)) {
			if (ball_y < padle_y || ball_y > padle_y - 32) {
				//flip the y direction
				al_play_sample_instance(instance3);
				ball_dy = -ball_dy;
				Brick6.killBrick();
				score += 10;
			}
		}
		if (!Brick7.isDead() && bounding_box_collision(660, 0, 100, 40, ball_x, ball_y, 50, 50)) {
			if (ball_y < padle_y || ball_y > padle_y - 32) {
				//flip the y direction
				al_play_sample_instance(instance3);
				ball_dy = -ball_dy;
				Brick7.killBrick();
				score += 10;
			}
		}
		if (!Brick8.isDead() && bounding_box_collision(770, 0, 100, 40, ball_x, ball_y, 50, 50)) {
			if (ball_y < padle_y || ball_y > padle_y - 32) {
				//flip the y direction
				al_play_sample_instance(instance3);
				ball_dy = -ball_dy;
				Brick8.killBrick();
				score += 10;
			}
		}
		if (!Brick9.isDead() && bounding_box_collision(880, 0, 100, 40, ball_x, ball_y, 50, 50)) {
			if (ball_y < padle_y || ball_y > padle_y - 32) {
				//flip the y direction
				al_play_sample_instance(instance3);
				ball_dy = -ball_dy;
				Brick9.killBrick();
				score += 10;
			}
		}
		///////////////////////////////ROW 2//////////////////

		if (!Brick10.isDead() && bounding_box_collision(0, 50, 100, 40, ball_x, ball_y, 50, 50)) {
			if (ball_y < padle_y || ball_y > padle_y - 32) {
				//flip the y direction
				al_play_sample_instance(instance2);
				ball_dy = -ball_dy;
				Brick10.killBrick();
				score += 10;
			}
		}
		if (!Brick11.isDead() && bounding_box_collision(110, 50, 100, 40, ball_x, ball_y, 50, 50)) {
			if (ball_y < padle_y || ball_y > padle_y - 32) {
				//flip the y direction
				al_play_sample_instance(instance2);
				ball_dy = -ball_dy;
				Brick11.killBrick();
				score += 10;
			}
		}
		if (!Brick12.isDead() && bounding_box_collision(220, 50, 100, 40, ball_x, ball_y, 50, 50)) {
			if (ball_y < padle_y || ball_y > padle_y - 32) {
				//flip the y direction
				al_play_sample_instance(instance2);
				ball_dy = -ball_dy;
				Brick12.killBrick();
				score += 10;
			}
		}
		if (!Brick13.isDead() && bounding_box_collision(330, 50, 100, 40, ball_x, ball_y, 50, 50)) {
			if (ball_y < padle_y || ball_y > padle_y - 32) {
				//flip the y direction
				al_play_sample_instance(instance2);
				ball_dy = -ball_dy;
				Brick13.killBrick();
				score += 10;
			}
		}
		if (!Brick14.isDead() && bounding_box_collision(440, 50, 100, 40, ball_x, ball_y, 50, 50)) {
			if (ball_y < padle_y || ball_y > padle_y - 32) {
				//flip the y direction
				al_play_sample_instance(instance2);
				ball_dy = -ball_dy;
				Brick14.killBrick();
				score += 10;
			}
		}
		if (!Brick15.isDead() && bounding_box_collision(550, 50, 100, 40, ball_x, ball_y, 50, 50)) {
			if (ball_y < padle_y || ball_y > padle_y - 32) {
				//flip the y direction
				al_play_sample_instance(instance2);
				ball_dy = -ball_dy;
				Brick15.killBrick();
				score += 10;
			}
		}
		if (!Brick16.isDead() && bounding_box_collision(660, 50, 100, 40, ball_x, ball_y, 50, 50)) {
			if (ball_y < padle_y || ball_y > padle_y - 32) {
				//flip the y direction
				al_play_sample_instance(instance2);
				ball_dy = -ball_dy;
				Brick16.killBrick();
				score += 10;
			}
		}
		if (!Brick17.isDead() && bounding_box_collision(770, 50, 100, 40, ball_x, ball_y, 50, 50)) {
			if (ball_y < padle_y || ball_y > padle_y - 32) {
				//flip the y direction
				al_play_sample_instance(instance2);
				ball_dy = -ball_dy;
				Brick17.killBrick();
				score += 10;
			}
		}
		if (!Brick18.isDead() && bounding_box_collision(880, 50, 100, 40, ball_x, ball_y, 50, 50)) {
			if (ball_y < padle_y || ball_y > padle_y - 32) {
				//flip the y direction
				al_play_sample_instance(instance2);
				ball_dy = -ball_dy;
				Brick18.killBrick();
				score += 10;
			}
		}
		///////////////////////////////ROW 3//////////////////

		if (!Brick19.isDead() && bounding_box_collision(0, 100, 100, 40, ball_x, ball_y, 50, 50)) {
			if (ball_y < padle_y || ball_y > padle_y - 32) {
				//flip the y direction
				al_play_sample_instance(instance1);
				ball_dy = -ball_dy;
				Brick19.killBrick();
				
			}
		}
		if (!Brick20.isDead() && bounding_box_collision(110, 100, 100, 40, ball_x, ball_y, 50, 50)) {
			if (ball_y < padle_y || ball_y > padle_y - 32) {
				//flip the y direction
				al_play_sample_instance(instance1);
				ball_dy = -ball_dy;
				Brick20.killBrick();
			
			}
		}
		if (!Brick21.isDead() && bounding_box_collision(220, 100, 100, 40, ball_x, ball_y, 50, 50)) {
			if (ball_y < padle_y || ball_y > padle_y - 32) {
				//flip the y direction
				al_play_sample_instance(instance1);
				ball_dy = -ball_dy;
				Brick21.killBrick();
				score += 10;
			}
		}
		if (!Brick22.isDead() && bounding_box_collision(330, 100, 100, 40, ball_x, ball_y, 50, 50)) {
			if (ball_y < padle_y || ball_y > padle_y - 32) {
				//flip the y direction
				al_play_sample_instance(instance1);
				ball_dy = -ball_dy;
				Brick22.killBrick();
				score += 10;
			}
		}
		if (!Brick23.isDead() && bounding_box_collision(440, 100, 100, 40, ball_x, ball_y, 50, 50)) {
			if (ball_y < padle_y || ball_y > padle_y - 32) {
				//flip the y direction
				al_play_sample_instance(instance1);
				ball_dy = -ball_dy;
				Brick23.killBrick();
				score += 10;
			}
		}
		if (!Brick24.isDead() && bounding_box_collision(550, 100, 100, 40, ball_x, ball_y, 50, 50)) {
			if (ball_y < padle_y || ball_y > padle_y - 32) {
				//flip the y direction
				al_play_sample_instance(instance1);
				ball_dy = -ball_dy;
				Brick24.killBrick();
				score += 10;
			}
		}
		if (!Brick25.isDead() && bounding_box_collision(660, 100, 100, 40, ball_x, ball_y, 50, 50)) {
			if (ball_y < padle_y || ball_y > padle_y - 32) {
				//flip the y direction
				al_play_sample_instance(instance1);
				ball_dy = -ball_dy;
				Brick25.killBrick();
				score += 10;
			}
		}
		if (!Brick26.isDead() && bounding_box_collision(770, 100, 100, 40, ball_x, ball_y, 50, 50)) {
			if (ball_y < padle_y || ball_y > padle_y - 32) {
				//flip the y direction
				al_play_sample_instance(instance1);
				ball_dy = -ball_dy;
				Brick26.killBrick();
				score += 10;
			}
		}
		if (!Brick27.isDead() && bounding_box_collision(880, 100, 100, 40, ball_x, ball_y, 50, 50)) {
			if (ball_y < padle_y || ball_y > padle_y - 32) {
				//flip the y direction
				al_play_sample_instance(instance1);
				ball_dy = -ball_dy;
				Brick27.killBrick();
				score += 10;
			}
		}
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
			if (key[1] && padle_x <= 1000 - 32) {
				padle_x += 6.0;
			


			}



			if (ball_x < 0 || ball_x > 1000 - 32) {
				//flip the x direction
				ball_dx = -ball_dx;


			}

			//if the box hits the top wall OR the bottom wall
			if (ball_y > 900 - 32) {
				//flip the y direction
				ball_x = 150;
				ball_y = 250;
			 ball_dx = -speed2, ball_dy = speed2;
			
				lives1--;
			
			}
			if (ball_y < 0 - 32) {
				//flip the y direction
				ball_dy = -ball_dy;
			
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

		//RENDER SECTION
		//if the clock ticked but no other events happened, don't bother redrawing
		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;

			//paint black over the old screen, so the old square dissapears
			al_clear_to_color(al_map_rgb(0, 0, 0));

			//the algorithm above just changes the x and y coordinates
			//here's where the bitmap is actually drawn somewhere else
			al_draw_bitmap(image1, 250, 0, 0);
			al_draw_bitmap(padle, padle_x, padle_y, 0);

			
			al_draw_bitmap(ball, ball_x, ball_y, 0);
			//call the bounding box function. if it returns 1, print out your collision message
			al_draw_text(font2, al_map_rgb(0, 255, 255), 50, 450, ALLEGRO_ALIGN_CENTRE, "lives");
			al_draw_textf(font2, al_map_rgb(0, 255, 255), 120, 450, ALLEGRO_ALIGN_CENTRE, "%d", lives1);
			////score
			al_draw_text(font2, al_map_rgb(0, 255, 255), 754, 450, ALLEGRO_ALIGN_CENTRE, "Score:");
			al_draw_textf(font2, al_map_rgb(0, 255, 255), 890, 450, ALLEGRO_ALIGN_CENTRE, "%d", score);
			/////bricks
			if (Brick1.isDead() == false) {
				Brick1.drawBrick();
			}
			if (Brick2.isDead() == false) {
				Brick2.drawBrick();
			}
			if (Brick3.isDead() == false) {
				Brick3.drawBrick();
			}
			if (Brick4.isDead() == false) {
				Brick4.drawBrick();
			}
			if (Brick5.isDead() == false) {
				Brick5.drawBrick();
			}
			if (Brick6.isDead() == false) {
				Brick6.drawBrick();
			}
			if (Brick7.isDead() == false) {
				Brick7.drawBrick();
			}
			if (Brick8.isDead() == false) {
				Brick8.drawBrick();
			}
			if (Brick9.isDead() == false) {
				Brick9.drawBrick();
			}

			if (Brick10.isDead() == false) {
				Brick10.drawBrick();
			}
			if (Brick11.isDead() == false) {
				Brick11.drawBrick();
			}
			if (Brick12.isDead() == false) {
				Brick12.drawBrick();
			}
			if (Brick13.isDead() == false) {
				Brick13.drawBrick();
			}
			if (Brick14.isDead() == false) {
				Brick14.drawBrick();
			}
			if (Brick15.isDead() == false) {
				Brick15.drawBrick();
			}
			if (Brick16.isDead() == false) {
				Brick16.drawBrick();
			}
			if (Brick17.isDead() == false) {
				Brick17.drawBrick();
			}
			if (Brick18.isDead() == false) {
				Brick18.drawBrick();
			}

			if (Brick19.isDead() == false) {
				Brick19.drawBrick();
			}
			if (Brick20.isDead() == false) {
				Brick20.drawBrick();
			}
			if (Brick21.isDead() == false) {
				Brick21.drawBrick();
			}
			if (Brick22.isDead() == false) {
				Brick22.drawBrick();
			}
			if (Brick23.isDead() == false) {
				Brick23.drawBrick();
			}
			if (Brick24.isDead() == false) {
				Brick24.drawBrick();
			}
			if (Brick25.isDead() == false) {
				Brick25.drawBrick();
			}
			if (Brick26.isDead() == false) {
				Brick26.drawBrick();
			}
			if (Brick27.isDead() == false) {
				Brick27.drawBrick();
			}

		




			al_flip_display();


		}

	}//end game loop

	al_destroy_bitmap(padle);
	al_destroy_bitmap(ball);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_sample(sample3);

	return 0;
}
void Brick::set_values(int x, int y, int w, int h) {
	xPos = x;
	yPos = y;

	width = w;
	height = h;
	dead = false;
}

void Brick::drawBrick() {
	al_draw_filled_rectangle(xPos, yPos, xPos + width, yPos + height, al_map_rgb(255, 60, 255));


}
bool Brick::isDead() {
	return dead;
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

void Brick::killBrick() {
	dead = true;
}
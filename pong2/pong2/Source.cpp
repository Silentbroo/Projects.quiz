#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
//function to detect collision
//paramaters are x & y coordinates of top left corner of first box, then its width and height
//then same for other box
int bounding_box_collision(int b1_x, int b1_y, int b1_w, int b1_h, int b2_x, int b2_y, int b2_w, int b2_h);

//using constants in this game so you have sensible words, not unknown numbers in code
const float FPS = 60;
const int SCREEN_W = 600;
const int SCREEN_H = 800;
const int BOUNCER_W = 150;
const int BOUNCER_H = 30;
//an enumeration is like a user-defined variable, with all the given values it can hold
enum MYKEYS {
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};
enum MYKEYS2 {
	KEY_W, KEY_S, KEY_A, KEY_D
};


int main()
{
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *bouncer = NULL;
	int lives1 = 3;
	int lives2 = 3;
	int speed = 4;
	float padle_x = 0;
	float padle_y = 770;
	bool key[4] = { false, false, false, false };
	ALLEGRO_BITMAP *bouncer2 = NULL;
	float padle2_x = 0;
	float padle2_y = 0;
	bool key2[4] = { false, false, false, false };
	ALLEGRO_BITMAP *bouncer3 = NULL;
	float ball_x = 400;
	float ball_y = 200;
	float ball_dx = -8.0, bounce_dy = 8.0;
	bool redraw = true;
	bool doexit = false;

	//including some error messages to help debug
	al_init();
	al_init_primitives_addon();
	al_install_keyboard();
	timer = al_create_timer(1.0 / FPS);
	display = al_create_display(SCREEN_W, SCREEN_H);
	event_queue = al_create_event_queue();


	//hook up fonts for on-screen text
	al_init_font_addon(); 
	al_init_ttf_addon();
	ALLEGRO_FONT *font1 = al_load_ttf_font("sleep.ttf",100,0 );
	ALLEGRO_FONT *font2 = al_load_ttf_font("alice.ttf",30,0 );


	//set up the two squares
	bouncer = al_create_bitmap(BOUNCER_W, BOUNCER_H);

	al_set_target_bitmap(bouncer);
	/////////padle2
	bouncer2 = al_create_bitmap(BOUNCER_W, BOUNCER_H);
	al_clear_to_color(al_map_rgb(125, 0, 125));

	al_set_target_bitmap(bouncer2);

	al_clear_to_color(al_map_rgb(0, 255, 255));
	/////ball
	bouncer3 = al_create_bitmap(25, 25);

	al_set_target_bitmap(bouncer3);

	al_clear_to_color(al_map_rgb(255, 255, 255));

	al_set_target_bitmap(al_get_backbuffer(display));

	//set up your event queue
	event_queue = al_create_event_queue();
	
	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	al_register_event_source(event_queue, al_get_keyboard_event_source());

	//draw the initial screen, start up the timer
	al_clear_to_color(al_map_rgb(0,0,0));

	al_flip_display();

	al_start_timer(timer);

	while(!doexit)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		//if a clock ticks, check if we should be moving a square (by checking key or key2)
		if(ev.type == ALLEGRO_EVENT_TIMER) {
			if (lives1 == 0 || lives2 == 0){
				al_clear_to_color(al_map_rgb(0,0,0));
				al_flip_display();
				al_draw_text(font1, al_map_rgb(255,255,255), 300, 400,ALLEGRO_ALIGN_CENTRE, "Game Over");
				al_flip_display();
				al_rest(3);
				al_destroy_bitmap(bouncer);
				al_destroy_bitmap(bouncer2);
				al_destroy_bitmap(bouncer3);
				al_destroy_timer(timer);
				al_destroy_display(display);
				al_destroy_event_queue(event_queue);
			}
			//if the box hits the left wall OR the right wall
			if(bouncer3_x < 0 || bouncer3_x > 600 -32) {
				//flip the x direction
				bouncer3_dx = -bouncer3_dx;
			}
			//if the box hits the top wall OR the bottom wall
			if(bouncer3_y > 800 - 32) {
				//flip the y direction
				bouncer3_x = 400;
				bouncer3_y = 200;
				bouncer3_dx = -8.0, bouncer3_dy = 8.0;
				lives2--;
			}
			if(bouncer3_y < 0 - 32) {
				//flip the y direction
				bouncer3_x = 400;
				bouncer3_y = 200;
				bouncer3_dx = -8.0, bouncer3_dy = 8.0;
				lives1--;
			}

			//really important code!
			//move the box in a diagonal
			bouncer3_x += bouncer3_dx;
			bouncer3_y += bouncer3_dy;

			if(key[KEY_LEFT] && bouncer_x >= 4.0) {
				bouncer_x -= 4.0;
			}

			if(key[KEY_RIGHT] && bouncer_x <= SCREEN_W - BOUNCER_H - 120) {
				bouncer_x += speed;
			}


			if(key2[KEY_A] && bouncer2_x >= 4.0) {
				bouncer2_x -= speed;
			}

			if(key2[KEY_D] && bouncer2_x <= SCREEN_W - BOUNCER_H - 120) {
				bouncer2_x += speed;
			}

			redraw = true;
		}

		//kill program if window "X" has been clicked
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}

		//update key/key2 if a key has been pressed
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch(ev.keyboard.keycode) {

			case ALLEGRO_KEY_LEFT: 
				key[KEY_LEFT] = true;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT] = true;
				break;

			case ALLEGRO_KEY_A: 
				key2[KEY_A] = true;
				break;

			case ALLEGRO_KEY_D:
				key2[KEY_D] = true;
				break;


			}
		}
		//update key/key2 if a key has been released
		else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch(ev.keyboard.keycode) {

			case ALLEGRO_KEY_LEFT: 
				key[KEY_LEFT] = false;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT] = false;
				break;

			case ALLEGRO_KEY_A: 
				key2[KEY_A] = false;
				break;

			case ALLEGRO_KEY_D:
				key2[KEY_D] = false;
				break;


			}
		}
		//take all the above information and update screen
		al_clear_to_color(al_map_rgb(0,0,0));

		al_draw_bitmap(bouncer, bouncer_x, bouncer_y, 0);

		al_draw_bitmap(bouncer2, bouncer2_x, bouncer2_y, 0);

		al_draw_bitmap(bouncer3, bouncer3_x, bouncer3_y, 0);
		//call the bounding box function. if it returns 1, print out your collision message
		al_draw_text(font2, al_map_rgb(255,255,255),50,350,ALLEGRO_ALIGN_CENTRE,"lives");
		al_draw_textf(font2, al_map_rgb(255,255,255),90,350,ALLEGRO_ALIGN_CENTRE, "%d", lives1);

		al_draw_text(font2, al_map_rgb(255,255,255),50,450,ALLEGRO_ALIGN_CENTRE,"lives");
		al_draw_textf(font2, al_map_rgb(255,255,255),90,450,ALLEGRO_ALIGN_CENTRE, "%d", lives2);

		if (bounding_box_collision(bouncer_x, bouncer_y, 150, 30, bouncer3_x, bouncer3_y, 50, 50)){
			if(bouncer3_y < bouncer_y || bouncer3_y > bouncer_y - 32) {
				//flip the y direction
			
				bouncer3_dy = -bouncer3_dy;
			}
		}
		if (bounding_box_collision(bouncer2_x, bouncer2_y, 150, 30, bouncer3_x, bouncer3_y, 50, 50)){
			if(bouncer3_y < bouncer2_y || bouncer3_y > bouncer2_y - 32) {
				//flip the y direction
		
				bouncer3_dy = -bouncer3_dy;
			}
		}
		al_flip_display();

	}

	al_destroy_bitmap(bouncer);
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
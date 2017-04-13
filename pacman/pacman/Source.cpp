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
const int SCREEN_W = 1000;
const int SCREEN_H = 800;
const int BOUNCER_SIZE = 30;
const int WALLSIZE = 40;
const int DOT = 4;



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
	float bouncer_x = SCREEN_W / 2.0 - BOUNCER_SIZE / 2.0;
	float bouncer_y = SCREEN_H / 2.0 - BOUNCER_SIZE / 2.0;
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

	while (!doexit)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		//if a clock ticks, check if we should be moving a square (by checking key or key2)
		if (ev.type == ALLEGRO_EVENT_TIMER) {
			if (key[KEY_UP] && bouncer_y >= 4.0) {
				bouncer_y -= 4.0;
			}

			if (key[KEY_DOWN] && bouncer_y <= SCREEN_H - BOUNCER_SIZE - 4.0) {
				bouncer_y += 4.0;
			}

			if (key[KEY_LEFT] && bouncer_x >= 4.0) {
				bouncer_x -= 4.0;
			}

			if (key[KEY_RIGHT] && bouncer_x <= SCREEN_W - BOUNCER_SIZE - 4.0) {
				bouncer_x += 4.0;
			}
			
			redraw = true;
		}
		//kill program if window "X" has been clicked
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
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

		//take all the above information and update screen
		al_clear_to_color(al_map_rgb(0, 0, 0));

		al_draw_bitmap(pacman, bouncer_x, bouncer_y, 0);
		/////////Wall/////
		for (int i = 0; i<20; i++)
			for (int j = 0; j<20; j++) {
				if (level[i][j] == 1)
					al_draw_bitmap(platform, j * 40, i * 40, 0);
			}
		////dots
		for (int i = 0; i<20; i++)
			for (int j = 0; j < 20; j++) {
				if (level[i][j] == 0)
					al_draw_bitmap(dot, j * 40+20, i * 40+20, 0);

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
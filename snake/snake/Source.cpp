#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include<iostream>
#include <allegro5/allegro_acodec.h>
#include <Windows.h>
#include <ctime>
#include <cstdlib>
#include <allegro5/allegro_image.h>
#include <vector>
using namespace std;
int main() {
	vector<int>::const_iterator iter;
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *snake = NULL;
	ALLEGRO_BITMAP*platform = NULL;
	
	al_init();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_audio();
	al_init_acodec_addon();
	al_install_keyboard();
	al_init_image_addon();

	//sound
	ALLEGRO_SAMPLE *sample = NULL;
	al_reserve_samples(2);
	sample = al_load_sample("background.wav");
	al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
	//CREATE GAME 
	timer = al_create_timer(CLOCK);
	display = al_create_display(800, 800);

	int grid[20][20];
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 20; j++) {
			if (grid[i][j] == 0);
				
		}

}
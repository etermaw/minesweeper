#include "window.h"
#include "resource.h"
#include "dialogs.h"

struct window
{
	ALLEGRO_DISPLAY* display;
	ALLEGRO_EVENT_QUEUE* msg_queue;

	bool destroyed;
};

window* create_window(unsigned width, unsigned height, const char* title)
{
	window* tmp = (window*)malloc(sizeof(window));

	tmp->display = al_create_display(width, height);
	tmp->msg_queue = al_create_event_queue();

	al_register_event_source(tmp->msg_queue, al_get_display_event_source(tmp->display));
	al_register_event_source(tmp->msg_queue, al_get_keyboard_event_source());
	al_register_event_source(tmp->msg_queue, al_get_mouse_event_source());

	tmp->destroyed = false;

	HMENU m = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU1));
	SetMenu(al_get_win_window_handle(tmp->display), m);
	
	al_set_window_title(tmp->display, title);
	al_win_add_window_callback(tmp->display, menu_func,NULL);

	return tmp;
}

void destroy_window(window* obj)
{
	al_destroy_display(obj->display);
	al_destroy_event_queue(obj->msg_queue);
	
	free(obj);
}

void resize_window(window* obj, unsigned width, unsigned height)
{
	al_resize_display(obj->display, width, height);
}

bool is_window_destroyed(window* obj)
{
	return obj->destroyed;
}

//must be no inline, or vc++ will make some weird optimizations resulting in bugs
__declspec(noinline) bool get_input(window* obj, mouse_click* click)
{
	ALLEGRO_EVENT msg;

	al_get_next_event(obj->msg_queue, &msg);

	switch (msg.type)
	{
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			obj->destroyed = true;
			break;

		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			click->x = msg.mouse.x;
			click->y = msg.mouse.y;
			click->left_key = (msg.mouse.button == 1);
			return true;
	}

	return false;
}

void msg_box(window* obj, const char* title, const char* text)
{
	MessageBox(al_get_win_window_handle(obj->display), text, title, MB_OK | MB_TOPMOST);
}
#ifndef TEST_CLIENT_H
#define TEST_CLIENT_H

//client
#define init test_init
#define cli_get_msg test_cli_get_msg
#define cli_send test_cli_send
#define cli_close test_cli_close

//client_graphics
#define init_sdl test_init_sdl
#define close_window test_close_window
#define clear_window test_clear_window
#define color_pixel test_color_pixel
#define update_window test_update_window
#define get_event test_get_event
#define change_points test_change_points

//other
#define fgets test_fgets

char *test_fgets(char* str, int n, FILE *stream);

#endif

#ifndef test_server_h
#define test_server_h

#define serv_listen test_serv_listen
#define serv_accept test_serv_accept
#define serv_send test_serv_send
#define serv_get_msg test_serv_get_msg
#define serv_disconnect test_serv_disconnect
#define serv_stop test_serv_stop
#define delay test_delay
#define srand test_srand

void test_delay(int ms);
void test_srand(unsigned int seed);

#endif

#ifndef SIG_H_
#define SIG_H_


typedef void (*sig_handler_pt)(int signo);


int sig_mask_print(void);
sig_handler_pt sig_sethandler(int signo, sig_handler_pt func);


#endif


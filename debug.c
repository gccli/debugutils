#include "debug.h"







#ifdef _TEST
int main(int argc, char *argv[])
{
    char buf[1024] = "hello world\r\n";
    print_string(buf, strlen(buf));
    debug_puts("%s hello world", argv[0]);
    debug_puts(FMT_G("%s hello world"), argv[0]);
    debug_puts("%s hello world", argv[0]);
    debug_puts(FMT_R("%s hello world"), argv[0]);
    return 0;
}
#endif

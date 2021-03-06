//Struttura per l'input da tastiera
typedef enum
{
    KP_ECHO_OFF,
    KP_ECHO_ON,
} kp_echo_t;

//Handler input tasto premuto
int keypress(const kp_echo_t echo)
{
    struct termios savedState, newState;
    unsigned char echo_bit; // flag
    int c;
    if (-1 == tcgetattr(STDIN_FILENO, &savedState))
    {
        return EOF;
    }; // error
    newState = savedState;
    if (KP_ECHO_OFF == echo)
    {
        echo_bit = ECHO;
    }
    else
    {
        echo_bit = 0;
    };
    /* canonical input + set echo with minimal input as 1. */
    newState.c_lflag &= ~(echo_bit | ICANON);
    newState.c_cc[VMIN] = 1;
    if (-1 == tcsetattr(STDIN_FILENO, TCSANOW, &newState))
    {
        return EOF;
    };             // error
    c = getchar(); /* block until key press */
    if (-1 == tcsetattr(STDIN_FILENO, TCSANOW, &savedState))
    {
        return EOF;
    }; // error
    return c;
}

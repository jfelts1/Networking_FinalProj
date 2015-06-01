
//using this function REQUIRES you to write a function named cleanUp()
void handle_signal(int signal);

void handle_signal(int signal)
{
    const char * signal_name;
    sigset_t pending;

    switch(signal)
    {
        case SIGTERM:
            killed = true;
            cleanUp();
            exit(0);
            break;
        case SIGINT:
            killed = true;
            cleanUp();
            exit(0);
            break;
        case SIGQUIT:
            killed = true;
            cleanUp();
            exit(0);
            break;
        default:
            fprintf(stderr,"Caught wrong signal: %d\n",signal);
            return;
    }
}

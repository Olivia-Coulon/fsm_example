#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdio.h>

typedef enum {SX, S1R, S2R, S3R, S4R, S1W, S2W, S3W, S4W} passcode_state_t;

#define INPUT_ARRAY_SIZE 15

// In this example the PIN for unlocking is 9876
#define PIN1st_DIGIT 9
#define PIN2nd_DIGIT 8
#define PIN3rd_DIGIT 7
#define PIN4th_DIGIT 6


bool passcodeFSM(int guess);
int getGuessInput();
void playUnlockJingle();
void openLock();
void closeLock();

int main(void) {
    printf ("FSM simulation of the lock with a 4-digit pin:\n");

    while (1) {
        // the getGuessInput() function will halt the program
        // until an input is received
        int input = getGuessInput();

        // passcodeFSM() is the function containing the FSM
        bool unlock = passcodeFSM(input);

        if (unlock)
            playUnlockJingle();
    }
}

int getGuessInput()
{
    // since we have not written any code that reads input from a keypad (we even don't have a keypad on this kit)
    // we use a synthetic input. In other words, we make up some input to test our fsm
    int guesses[INPUT_ARRAY_SIZE] = {9, 8, 7, 6, 0,
                                     9, 8, 7, 5, 0,
                                     1, 8, 7, 6, 0};

    // using a static variable helps us to retain i so that we get a new input every time we return
    static int i = 0;
    int guess = guesses[i%INPUT_ARRAY_SIZE];
    i++;
    return (guess);
}

// An empty function. Let's imagine this function plays a jingle!
void playUnlockJingle() {

}

bool passcodeFSM(int guess) {
    static passcode_state_t currentState = SX;

    bool unlock = false; // if you assign you will have to type each time if something is false or true
                         // but bc we stated this, we only have to state when something is truw

    printf ("current state is %d, input is %d", currentState, guess);

    switch (currentState) { //programming the FSM, this initalizes all states and their requirements/outputs
    case SX://initial state
        if (guess == PIN1st_DIGIT) //prompting for 1st guess
            currentState = S1R;
        else
            currentState = S1W;
        break;


    case S1R:
        if (guess == PIN2nd_DIGIT) //when 1st guess was correct, now ready for 2nd guess
            currentState = S2R;
        else
            currentState = S2W;
        break;

    case S2R:
        if (guess == PIN3rd_DIGIT) //2nd guess was correct, prompt for 3rd guess
            currentState = S3R;
        else
            currentState = S3W;
        break;

    case S3R:
        if (guess == PIN4th_DIGIT) //3rd guess was correct, prompt for 4th digits
        {
            currentState = S4R; //automatically unlocks
            unlock = true;
        }
        else
            currentState = S4W;
        break;

    case S4R:
        currentState = SX;      //since it unlocked, the next output goes back to default
        break;

    case S1W:
        currentState = S2W;//when the 1st digit was wrong, NO MATTER the input the over all passcode is still wrong
        break; //therefore the only option is to continue to the wrong options

    case S2W:
        currentState = S3W;
        break;

    case S3W:
        currentState = S4W;
        break;

    case S4W:
        currentState = SX;
        break;

    }

    // the currentState is now reflecting the next state when we enter this function
    printf (", next state is %d, output is %d\n", currentState, unlock);

    if (unlock)
        openLock();
    else
        closeLock();

    return unlock;
}

void openLock()
{

}
void closeLock(){

}

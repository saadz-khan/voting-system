#include <reg51.h>

// connected pins
// keypad rows
sbit keyrow1 = P2 ^ 0;				// 4x3 keypad Row 1
sbit keyrow2 = P2 ^ 1;				// 4x3 keypad Row 2
sbit keyrow3 = P2 ^ 2;				// 4x3 keypad Row 3
sbit keyrow4 = P2 ^ 3;				// 4x3 keypad Row 4

//keypad column	
sbit keycolumn1 = P3 ^ 0;			// 4x3 keypad column 1
sbit keycolumn2 = P3 ^ 1;			// 4x3 keypad column 2
sbit keycolumn3 = P3 ^ 2;			// 4x3 keypad column 3

// motor pins
sbit motorpin1 = P3 ^ 3;			// Controlling Lock System Motor 1
sbit motorpin2 = P3 ^ 4;			// Controlling Lock System Motor 2

// led pins
sbit rs = P3 ^ 5;					// 0 : Insturction code input , 1 : Data input
sbit rw = P3 ^ 6;					// Data Write Pin for LCD Display
sbit en = P3 ^ 7;					// Enable Signal pin for LCD Display

// Vote variables
party1 = 0;
party2 = 0;
party3 = 0;
party4 = 0;

unsigned char pin[] = {"12345"};
unsigned char Epin[9];
unsigned char vote[] = {"1234"};
char vote_no[4];

// All functions declarations
void lcdcmd(unsigned char);
void lcddat(unsigned char);
void lcddisplay(unsigned char *q);
char keypad();
int check();
void delay(unsigned int);
void line_disp(unsigned char *);
void count();
void thank_func();
void display_vote(unsigned int party);

// main function
int main()
{
    lcdcmd(0x0F); //decimal value: 15
    lcdcmd(0x38); //decimal value: 56
    lcdcmd(0x01); //decimal value: 1

    while (1)
    {
        unsigned int i = 0, j = 0;      // Temporary Variable Declaration
        if (j == 1){
            break;
        }
        lcdcmd(0x01); // Clear screen
        lcdcmd(0x80); //decimal value: 128
        lcddisplay("ENTER PIN NUMBER");
        delay(500);	
        lcdcmd(0xc0); //decimal value: 192
        while (pin[i] != '\0')
        {
            Epin[i] = keypad();
            delay(1000);
            i++;
        }
        check();
		if (check() == 1)
		{
            lcdcmd(0x80);
            lcddisplay("Welcome User!");
            delay(500);
            lcdcmd(0x80);
            line_disp("PTI PPP PML JUI");
            count();
            delay(1000);
	        lcdcmd(0xc0); //decimal value: 193
            display_vote(party1);
            lcdcmd(0xc4);
            display_vote(party2);
            lcdcmd(0xc8);
            display_vote(party3);
            lcdcmd(0xcc);
            display_vote(party4);
        }
    thank_func(); 
    j++;
    i++;
    }
    return 0;
}


void display_vote(unsigned int party)  // send 0-9 character values
{ 
int k,p;
for (k=0;k<=2;k++)
{
  vote_no[k] = party % 10;
  party = party/10;
}
for (p=2;p>=0;p--)
{
lcddat(vote_no[p] + 48);
}
}


// Countintg votes function
void count(){
    unsigned int m = 0;
    while (vote[m] != '\0'){
        Epin[m] = keypad();
        if (vote[0] == Epin[0]){
            party1 = party1 + 1;
            break;
        }
        if (vote[1] == Epin[1]){
            party2 = party2 + 1;
            break;
        }
        if (vote[2] == Epin[2]){
            party3 = party3 + 1;
            break;
        }
        if (vote[3] == Epin[3]){
            party4 = party4 + 1;
            break;
        }
    }
    m++;
}

// password check function and run the door motor
int check()
{
    //  compare the input value with the assign password value
    if (pin[0] == Epin[0] && pin[1] == Epin[1] && pin[2] == Epin[2] && pin[3] == Epin[3] && pin[4] == Epin[4])
    {
        delay(500);
        lcdcmd(0x01); //decimal value: 1
        lcdcmd(0x81); //decimal value: 129
        // show pin is correct
        lcddisplay("PIN CORRECT");
        delay(500);
        // door motor will run
        motorpin1 = 1;
        motorpin2 = 0;
        lcdcmd(0xc1); //decimal value: 193
        // show the door is unlocked
        lcddisplay("AUTHENTICATED");
        delay(10000);
        motorpin1 = 1;
        motorpin2 = 0;
        lcdcmd(0x01); //decimal value: 1
		return 1;
    }
    else
    {
        lcdcmd(0x01); //decimal value: 1
        lcdcmd(0x80); //decimal value: 128
        lcddisplay("WRONG PIN");
        delay(500);
        lcdcmd(0x01); //decimal value: 1
		return 0;
    }
}


void line_disp(unsigned char *c)
{
		
		lcdcmd(0x01); //decimal value: 1
	    lcdcmd(0x80); //decimal value: 129
 	    // show pin is correct
  	    lcddisplay(c);
}

// Thank You function
void thank_func(){
    lcdcmd(0x01);
    lcdcmd(0x80);
    lcddisplay("Thank You");
    lcdcmd(0xc0);
    lcddisplay("Response Collected");
}

//delay function
void delay(unsigned int j)
{
    int a, b;
    for (a = 0; a < j; a++)
    {
        for (b = 0; b < 10; b++)
            ;
    }
}

// lcd commands functions
void lcdcmd(unsigned char A)
{
    P1 = A;
    rs = 0;
    rw = 0;
    en = 1;
    delay(1000);
    en = 0;
}

//lcd data function

void lcddat(unsigned char i)
{
    P1 = i;
    rs = 1;
    rw = 0;
    en = 1;
    delay(1000);
    en = 0;
}

//lcd display charecters function

void lcddisplay(unsigned char *q)
{
    int k;
    for (k = 0; q[k] != '\0'; k++)
    {
        lcddat(q[k]);
    }
    delay(10000);
}

// assign keypad character value function

char keypad()
{
    int x = 0;
    while (x == 0)
    {
		// Algorithm for 4 x 3 keypad
        // assign values for first row
        keyrow1 = 0;
        keyrow2 = 1;
        keyrow3 = 1;
        keyrow4 = 1;
        if (keycolumn1 == 0)
        {
            lcddat('*');
            delay(1000);
            x = 1;
            return '1';
        }
        if (keycolumn2 == 0)
        {
            lcddat('*');
            delay(1000);
            x = 1;
            return '2';
        }
        if (keycolumn3 == 0)
        {
            lcddat('*');
            delay(1000);
            x = 1;
            return '3';
        }
        // assign values for second row
        keyrow1 = 1;
        keyrow2 = 0;
        keyrow3 = 1;
        keyrow4 = 1;

        if (keycolumn1 == 0)
        {
            lcddat('*');
            delay(1000);
            x = 1;
            return '4';
        }
        if (keycolumn2 == 0)
        {
            lcddat('*');
            delay(1000);
            x = 1;
            return '5';
        }
        if (keycolumn3 == 0)
        {
            lcddat('*');
            delay(1000);
            x = 1;
            return '6';
        }

        // assign values for third row
        keyrow1 = 1;
        keyrow2 = 1;
        keyrow3 = 0;
        keyrow4 = 1;
        if (keycolumn1 == 0)
        {
            lcddat('*');
            delay(1000);
            x = 1;
            return '7';
        }
        if (keycolumn2 == 0)
        {
            lcddat('*');
            delay(1000);
            x = 1;
            return '8';
        }
        if (keycolumn3 == 0)
        {
            lcddat('*');
            delay(1000);
            x = 1;
            return '9';
        }

        // assign values for forth row
        keyrow1 = 1;
        keyrow2 = 1;
        keyrow3 = 1;
        keyrow4 = 0;

        if (keycolumn1 == 0)
        {
            lcddat('*');
            delay(1000);
            x = 1;
            return '*';
        }
        if (keycolumn2 == 0)
        {
            lcddat('*');
            delay(1000);
            x = 1;
            return '0';
        }
        if (keycolumn3 == 0)
        {
            lcddat('*');
            delay(1000);
            x = 1;
            return '#';
        }
    }
}

end;

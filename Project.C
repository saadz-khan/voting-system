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

unsigned char pin1[] = {"12345"}, pin2[] = {"98765"}, pin3[] = {"25801"}, pin4[] = {"24568"};
unsigned char Epin[9];
unsigned char vote[] = {"1234"};
char vote_no[4];
unsigned int j,r = 0;

// All functions declarations
void lcdcmd(unsigned char);
void lcddat(unsigned char);
void lcddisplay(unsigned char *q);
char keypad();
int keygive();
int check();
void delay(unsigned int);
void line_disp(unsigned char *);
void count();
void thank_func();
void display_vote(unsigned int party);
void result();
void if_func();
void else_func();
// main function
int main()
{
    lcdcmd(0x0F); //decimal value: 15
    lcdcmd(0x38); //decimal value: 56
    lcdcmd(0x01); //decimal value: 1	lcd
	lcdcmd(0x80);
    line_disp("ENTER NO OF VOTES ");
    delay(500);
    lcdcmd(0xc0); //decimal value: 193
	r = keygive();
    a:
    while (1)
    {
        unsigned int i = 0;      // Temporary Variable Declaration
        if (j == r){
            break;
        }
        lcdcmd(0x01); // Clear screen
        lcdcmd(0x80); //decimal value: 128
        lcddisplay("ENTER PIN NUMBER");
        delay(500);	
        lcdcmd(0xc0); //decimal value: 192
        while (pin1[i] != '\0' || pin2[i] != '\0' || pin3[i] != '\0' || pin4[i] != '\0')
        {
            Epin[i] = keypad();
            delay(1000);
            i++;
        }
		if (check() == 1)
		{
            lcdcmd(0x80);
            lcddisplay("Welcome User!");
            delay(500);
            lcdcmd(0x80);
            line_disp("PTI PPP PML JUI ");
            count();
            delay(500);
	        lcdcmd(0xc0); //decimal value: 193
            display_vote(party1);
            lcdcmd(0xc4);
            display_vote(party2);
            lcdcmd(0xc8);
            display_vote(party3);
            lcdcmd(0xcc);
            display_vote(party4);
        }
        else goto a;
    j++;      
    thank_func(); 
    }
    delay(500);
    result();  
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
lcddat(vote_no[p] + 48);
}


// Countintg votes function
void count(){
    unsigned int m = 0;
    while (vote[m] != '\0'){
        Epin[m] = keypad();
        if (vote[0] == Epin[m]){
            party1 = party1 + 1;
            break;
        }
        if (vote[1] == Epin[m]){
            party2 = party2 + 1;
            break;
        }
        if (vote[2] == Epin[m]){
            party3 = party3 + 1;
            break;
        }
        if (vote[3] == Epin[m]){
            party4 = party4 + 1;
            break;
        }
    }
    m++;
}

void result(){
    lcdcmd(0x01); //decimal value: 1
        if (party1 > party2 && party1 > party3 && party1 > party4){
            lcdcmd(0x80);
            lcddisplay("PTI Won Election");
        }
        else if (party2 > party1 && party2 > party3 && party2 > party4){
            lcdcmd(0x80);
            lcddisplay("PPP Won Election");
        }
        else if (party3 > party1 && party3 > party2 && party3 > party4){
            lcdcmd(0x80);
            lcddisplay("PML Won Election");
        }
        else if (party4 > party1 && party4 > party3 && party4 > party2){
            lcdcmd(0x80);
            lcddisplay("JUI Won Election");
        }
        // All Parties Tie Situation
		else if (party1 == party2 && party1 == party3 && party2 != 0 && party3 != 0 && party4 != 0   ){
            lcdcmd(0x80);
            lcddisplay("Tie in Election");
            lcdcmd(0xc0);
            lcddisplay("All parties");
        }
		// Three Parties Tie Situations
		else if (party1 == party2 && party1 == party3 && party2 != 0 && party3 != 0  ){
            lcdcmd(0x80);
            lcddisplay("Tie in Election");
            lcdcmd(0xc0);
            lcddisplay("PTI PPP PML");
        }
		else if (party1 == party3 && party1 == party4 && party3 != 0 && party4 != 0  ){
            lcdcmd(0x80);
            lcddisplay("Tie in Election");
            lcdcmd(0xc0);
            lcddisplay("PTI PML JUI");
        }
		else if (party2 == party3 && party2 == party4 && party3 != 0 && party4 != 0  ){
            lcdcmd(0x80);
            lcddisplay("Tie in Election");
            lcdcmd(0xc0);
            lcddisplay("PPP PML JUI");
        }
		// Party 1 Tie Situation
        else if (party1 == party2 && party2 != 0){
            lcdcmd(0x80);
            lcddisplay("Tie in Election");
            lcdcmd(0xc0);
            lcddisplay("PTI and PPP");
        }
		 
        else if (party1 == party3 && party3 != 0){
            lcdcmd(0x80);
            lcddisplay("Tie in Election");
            lcdcmd(0xc0);
            lcddisplay("PTI and PML");
        }
        else if (party1 == party4 && party4 != 0){
            lcdcmd(0x80);
            lcddisplay("Tie in Election");
            lcdcmd(0xc0);
            lcddisplay("PTI and JUI");
        }
        // Party2 Tie Situations
        else if (party2 == party3 && party3 != 0){
            lcdcmd(0x80);
            lcddisplay("Tie in Election");
            lcdcmd(0xc0);
            lcddisplay("PPP and PML");
        }
        else if (party2 == party4 && party4 != 0){
            lcdcmd(0x80);
            lcddisplay("Tie in Election");
            lcdcmd(0xc0);
            lcddisplay("PPP and JUI");
        }
        // Party 3 Tie Situations
        else if (party3 == party4 && party4 != 0){
            lcdcmd(0x80);
            lcddisplay("Tie in Election");
            lcdcmd(0xc0);
            lcddisplay("PML and JUI");
        }
}
void if_func(){
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
}
void else_func(){
    lcdcmd(0x01); //decimal value: 1
    lcdcmd(0x80); //decimal value: 129
    // show pin is correct
    lcddisplay("WRONG PIN");
}

int check()
{
    unsigned int i;
    //  compare the input value with the assign password value
    if (pin1[0] == Epin[0] && pin1[1] == Epin[1] && pin1[2] == Epin[2] && pin1[3] == Epin[3] && pin1[4] == Epin[4])
    {        
        if_func();
        for (i = 0; i<=4 ; i++)
        pin1[i] = " ";
        return 1;
    }
    else if (pin2[0] == Epin[0] && pin2[1] == Epin[1] && pin2[2] == Epin[2] && pin2[3] == Epin[3] && pin2[4] == Epin[4])
    {
        if_func();
        for (i = 0; i<=4 ; i++)
            pin2[i] = " ";
        return 1;
    }
    else if (pin3[0] == Epin[0] && pin3[1] == Epin[1] && pin3[2] == Epin[2] && pin3[3] == Epin[3] && pin3[4] == Epin[4])
    {
        if_func();
        for (i = 0; i<=4 ; i++)
            pin3[i] = " ";
        return 1;
    }
    else if (pin4[0] == Epin[0] && pin4[1] == Epin[1] && pin4[2] == Epin[2] && pin4[3] == Epin[3] && pin4[4] == Epin[4])
    {
        if_func();
        for (i = 0; i<=4 ; i++)
            pin4[i] = " ";
        return 1;
    }
    else{ 
        else_func();
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
    delay(1000);
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

int keygive()
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
            lcddat('1');
            delay(1000);
            x = 1;
            return 1;
        }
        if (keycolumn2 == 0)
        {
            lcddat('2');
            delay(1000);
            x = 1;
            return 2;
        }
        if (keycolumn3 == 0)
        {
            lcddat('3');
            delay(1000);
            x = 1;
            return 3;
        }
        // assign values for second row
        keyrow1 = 1;
        keyrow2 = 0;
        keyrow3 = 1;
        keyrow4 = 1;

        if (keycolumn1 == 0)
        {
            lcddat('4');
            delay(1000);
            x = 1;
            return 4;
        }
        if (keycolumn2 == 0)
        {
            lcddat('5');
            delay(1000);
            x = 1;
            return 5;
        }
        if (keycolumn3 == 0)
        {
            lcddat('6');
            delay(1000);
            x = 1;
            return 6;
        }

        // assign values for third row
        keyrow1 = 1;
        keyrow2 = 1;
        keyrow3 = 0;
        keyrow4 = 1;
        if (keycolumn1 == 0)
        {
            lcddat('7');
            delay(1000);
            x = 1;
            return 7;
        }
        if (keycolumn2 == 0)
        {
            lcddat('8');
            delay(1000);
            x = 1;
            return 8;
        }
        if (keycolumn3 == 0)
        {
            lcddat('9');
            delay(1000);
            x = 1;
            return 9;
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
            lcddat('0');
            delay(1000);
            x = 1;
            return 0;
        }
        if (keycolumn3 == 0)
        {
            lcddat('#');
            delay(1000);
            x = 1;
            return '#';
        }
    }
}
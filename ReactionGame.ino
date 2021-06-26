/* *********LIT UP!************
 ****  REACTION TIME GAME  ****
    Created by Holly Bedard
    Student #200443133
    For COMP 1045
    Submitted:  Dec 11, 2019 */

//Setting pin locations on Arduino Leonardo using constant int variables that won't change during program:
//Buttons are analog and lights are digital due to space constraints on Arduino board.

const int red_light =  8; // the number of the RED LED pin
const int red_button =   A0;  //the number of the RED BUTTON pin
const int blue_light =  9; // the number of the BLUE LED pin
const int blue_button =   A1;  //the number of the BLUE BUTTON pin
const int white_light =  6; // the number of the WHITE LED pin
const int white_button =   A4;  //the number of the WHITE BUTTON pin
const int green_light =  4; // the number of the GREEN LED pin
const int green_button =   A2;  //the number of the GREEN BUTTON pin
const int yellow_light =  5; // the number of the YELLOW LED pin
const int yellow_button =   A3;  //the number of the YELLOW BUTTON pin

//declare/initialize other variables and arrays :
int score = 0;  //sets the score to start at 0
int light[] = {red_light, blue_light, green_light, yellow_light, white_light};    //array with all the possible light selections
int button[] = {red_button, blue_button, green_button, yellow_button, white_button};   //array with all the possible button selections, indexed to match the corresponding light array

//Load libraries
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

//Define address for the Serial LCD display.
#define I2C_ADDR  0x27  //If 0x3f doesn't work change this to 0x27
#define BACKLIGHT_PIN  3

//Initialise the Serial LCD.
LiquidCrystal_I2C lcd(I2C_ADDR, 2, 1, 0, 4, 5, 6, 7); //These pin numbers are hard coded in on the serial backpack board.

void setup() {
  // set the button pins as input, and the led pins as output:
  pinMode(red_button, INPUT);
  pinMode(red_light, OUTPUT);
  pinMode(blue_button, INPUT);
  pinMode(blue_light, OUTPUT);
  pinMode(green_button, INPUT);
  pinMode(green_light, OUTPUT);
  pinMode(yellow_button, INPUT);
  pinMode(yellow_light, OUTPUT);
  pinMode(white_button, INPUT);
  pinMode(white_light, OUTPUT);

  //set up the lcd screen for start of game:
  lcd.begin (16, 2);    //Initalize the LCD.
  lcd.setBacklightPin(3, POSITIVE); //Setup the backlight.
  lcd.setBacklight(HIGH); //Switch on the backlight.
  lcd.clear();  //clear the LCD
  lcd.setCursor(0, 0);  //goto first column and first line (0,0)
  lcd.print("Let's Play");     //Print at cursor Location
  lcd.setCursor(0, 1);  //goto first column and second line (0,1)
  lcd.print("Lit Up!!!");   //Print at cursor Location
  delay(5000);  //Hold at start screen for 5 seconds
}

void loop() {  //main loop for game
  score = 0;       //starts score at 0, clearing any previous score.
  selectGame();  //starts game countdown and timer, then plays game.
  endGame();     //end of game display and exit
}



void selectGame() {  //this is the beginning of the game.  It counts down, then plays game for 30 seconds.
  lcd.clear();          //clear screen
  lcd.setCursor(7, 0);  //goto 8th column and first line (7,0)
  lcd.print("3");     //Start countdown, Print at cursor Location
  delay(1000);        //wait 1 sec
  lcd.clear();
  lcd.setCursor(7, 0);  
  lcd.print("2");     
  delay(1000);
  lcd.clear();
  lcd.setCursor(7, 0);  
  lcd.print("1");     
  delay(1000);
  lcd.clear();
  lcd.setCursor(6, 0);  //goto 7th column and first line (6,0)
  lcd.print("GO!");     //Print at cursor Location
  delay(1000);

  uint32_t gameEnd = 30000;  //This is the length of the game in milliseconds (30sec).
  //Uses an unsigned 32 bit int type to store the game time value because when using millis to define time an int variable isn't big enough

  for ( uint32_t gameStart = millis();  (millis() - gameStart) < gameEnd;  ) {    // This 'for' statement was adapted from code found at https://arduino.stackexchange.com/questions/22272/how-do-i-run-a-loop-for-a-specific-amount-of-time/22278
    playGame();                                                            //this loop measures how long the game has been playing by comparing the current time the programs been running (millis) to the game start time (millis recorded at start of game).
  }                                                                        //If the difference between current time and the start of game is less than the time period alloted for game play (gameend = 30s), the loop executes, until time period runs out.
}


void playGame() {   //This is the game sequence.
  int const random_index = random(0, 5);  //chooses a random index number to allow a matching random light and button to be selected from the arrays
  int const random_delay = random(1000, 2000);  //chooses a random delay between 1 and 2 seconds for the time the light will be turned on

  digitalWrite(light[random_index], HIGH);   //turns on a random light
  uint32_t lightStart = millis();  //this is the current amount of time the program has been running
  uint32_t lightEnd = lightStart + random_delay;  // sets the random on-time a light will be displayed for and available to hit to score a point

  while ((millis()) < lightEnd) {   //While the current time is less than the random time alloted before the light turns off, do this:
    if (digitalRead(light[random_index]) == HIGH && analogRead(button[random_index]) == 0 ) {  //if the corresponding button is pressed while the light is on,
      digitalWrite(light[random_index], LOW);                                                     //turn off the light
      score++;                                                                                    //increase the score by 1
      updateScore();                                                                              // update the score on the lcd screen
      return;                                                                                     //return to where this playGame function was called in the previous function (selectGame)
    }
  }

  digitalWrite(light[random_index], LOW);  //turns the light off
  delay(random_delay);   //waits a random amount of time before displaying the next light
}


void updateScore() {    //this prints the new score to the lcd screen each time a button is correctly pressed while a light is on
  lcd.clear();  //clear the lcd screen
  lcd.setCursor(0, 0);  //goto first column and first line (0,0)
  lcd.print(String("Score = ") + String(score));   //Print at cursor Location
}


void endGame() {   //this is what happens at the end of the game
  lcd.clear();
  lcd.setCursor(0, 0);  //goto first column  and first line (0,0)
  lcd.print("Game Over!");     //Print at cursor Location
  lcd.setCursor(0, 1);  //goto first column and 2nd line (0,1)
  lcd.print(String("Final Score: ") + String(score));   //Print at cursor Location
  delay(8000);  //wait 8 seconds
  lcd.clear(); //clear screen
  lcd.setCursor(0, 0);  //goto first column  and first line (0,0)
  lcd.print("Next Game?");     //Print at cursor Location
  lcd.setCursor(0, 1);  //goto first column and 2nd line (0,1)
  lcd.print("Starts in 5 sec");     //Print at cursor Location
  delay(5000);  //wait 5 seconds until game loops back to start
}

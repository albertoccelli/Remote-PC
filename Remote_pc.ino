/*
  Remote control for PC
  Copyright (c) 2019 Alberto Occelli
  email: albertoccelli@gmail.com
  
  Description: turns any AtMega32U4-based board (Arduino Leonardo, Arduino Pro Micro)
  into an IR receiver for PC.  DOES NOT WORK with AtMega328P (Arduino UNO, Nano, Mini, ...)
  
  Version: 2.0
  Created: 22/12/2019
  Last modified: 23/12/2019

  Version history:
  - 1.0 -> basic control
  - 1.1 -> mouse mode added: program a button (default: POWER) to switch 
  from normal mode to mouse mode, in order to control the mouse with key buttons;
  - 2.0 -> 3x4 standard numeric keypad added (from 2 to 0, ITU E.161)

  TO BE ADDED:
  - More media buttons
  - '1' key
  - T9

  Remote_pc is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
*/

//Needed libraries
#include <IRremoteInt.h>
#include <IRremote.h>
#include "HID-Project.h"

#define N 27  //Number of buttons on the remote controller
#define debounceDelay 150   //Debounce of each button
#define keyDelay 750
#define receiver 9 // pin 1 of IR receiver to Arduino digital pin 9

//Global variables
int pResult = 0;
int pRead = 0;
int timer = 0;
int ptime = 0;

boolean mouse = false;

/*
  On the first time, run the code and open serial monitor, 
  in order to read the code corresponding to each button.
  Write each code into the buttonC array.
*/

//Button codes
const int buttonC[N] = {255,    //MOUSE-ARROW MODE
                        4845,   //MEDIA_VOLUME_UP
                      -28051,   //MEDIA_VOLUME_DOWN
                      -32641,   //MEDIA_VOLUME_MUTE
                       21165,   //SCROLL UP
                       -11731,  //SCROLL DOWN
                       8925,    //KEY_BACKSPACE
                       -20401,  //KEY_TAB
                       -12241,  //KEY_LEFT_SHIFT + KEY_TAB
                       -14281,  //KEY_UP_ARROW
                       10455,   //KEY_DOWN_ARROW
                       -30601,  //KEY_LEFT_ARROW
                       18615,   //KEY_RIGHT_ARROW
                       2295,    //KEY_ENTER       
                       26775,   //KEY_LEFT_GUI       
                       -22441,  //KEY_LEFT_ALT + FEY_F4
                       -4081,   //KEY_LEFT_CTRL + KEY_LEFT_ALT + KEY_TAB
                       16575,   //1  
                       -16321,  //2  
                       8415,    //3
                       -24481,  //4
                       24735,   //5
                       -8161,   //6
                       4335,    //7
                       -28561,  //8
                       20655,   //9
                       12495    //0                                                                                                                                    
                      };

//Numeric keypad layout
char two[7] = {'a', 'b', 'c', 'A', 'B', 'C', '2'};
char three[7] = {'d', 'e', 'f', 'D', 'E', 'F', '3'};
char four[7] = {'g', 'h', 'i', 'G', 'H', 'I', '4'};
char five[9] = {'j', 'k', 'l', 'm', 'J', 'K', 'L', 'M', '5'};
char six[7] = {'n', 'o', 'p', 'N', 'O', 'P', '6'};
char seven[7] = {'q', 'r', 's', 'Q', 'R', 'S', '7'};
char eight[7] = {'t', 'u', 'v', 'T', 'U', 'V', '8'};
char nine[9] = {'w', 'x', 'y', 'z', 'W', 'X', 'Y', 'Z', '9'};
char zero[2] = {' ', '0'};


IRrecv irrecv(receiver); // create instance of 'irrecv'
decode_results results;

void setup(){
  Serial.begin(9600); // for serial monitor output
  irrecv.enableIRIn(); // Start the receiver
  pinMode(10, OUTPUT); // Pin 9 output

  //Initialize consumer, keyboard and mouse
  Consumer.begin();
  Keyboard.begin();
  AbsoluteMouse.begin();
}
void loop(){
  int i;
  int result;
  
  if (irrecv.decode(&results)) // have we received an IR signal?
  {    
    if(results.value != -1) //Check if the button is still pressed
      result = results.value;
    
    timer = millis();  
    int delayB = timer - ptime; //debounce
    
    if (delayB >= debounceDelay){
      Serial.println(result);
      if (result == buttonC[0])
        mouse = !mouse;    //MOUSE-ARROW MODE
      else if (result == buttonC[1])
        Consumer.write(MEDIA_VOLUME_UP);
      else if (result == buttonC[2])
        Consumer.write(MEDIA_VOLUME_DOWN);
      else if (result == buttonC[3]) 
        Consumer.write(MEDIA_VOLUME_MUTE);
      else if (result == buttonC[4]) 
        Mouse.move(0, 0, -20);
      else if (result == buttonC[5]) 
        Mouse.move(0, 0, 20);
      else if (result == buttonC[6]) 
        Keyboard.write(KEY_BACKSPACE);
      else if (result == buttonC[7]) 
        Keyboard.write(KEY_TAB);
      else if (result == buttonC[8]){
        Keyboard.press(KEY_LEFT_SHIFT);
        Keyboard.press(KEY_TAB);       
        Keyboard.releaseAll();
      }
      else if (result == buttonC[9])         
        Keyboard.write(KEY_UP_ARROW);
      else if (result == buttonC[10])         
        Keyboard.write(KEY_DOWN_ARROW);
      else if (result == buttonC[11])         
        Keyboard.write(KEY_LEFT_ARROW);
      else if (result == buttonC[12])         
        Keyboard.write(KEY_RIGHT_ARROW);
      else if (result == buttonC[13])         
        Keyboard.write(KEY_ENTER);
      else if (result == buttonC[14])         
        Keyboard.write(KEY_LEFT_GUI);             
      else if (result == buttonC[15]){
        Keyboard.press(KEY_LEFT_ALT); 
        Keyboard.press(KEY_F4);       
        Keyboard.releaseAll();       
      }               
      else if (result == buttonC[16]){
        Keyboard.press(KEY_LEFT_CTRL);                      
        Keyboard.press(KEY_LEFT_ALT);  
        Keyboard.press(KEY_TAB);       
        Keyboard.releaseAll();       
      }
      
//------------------------------------------------------------------
//------------------------Numeric Keypad ---------------------------
//------------------------------------------------------------------

//2-----------------------------------------------------------------
      else if (result == buttonC[18]){
        if (pResult != result){
          i = 0;
          Keyboard.write(two[i]);
          i++;
        }
        else if (pResult == result){
          if (delayB <=keyDelay){
            Keyboard.write(KEY_BACKSPACE);
            Keyboard.write(two[i]);
            i++;
            if (i == 7)
              i = 0; 
          }
          else{
            i = 0;
            Keyboard.write(two[i]);
            i++;
          }
        }
      }
//3-----------------------------------------------------------------
       else if (result == buttonC[19]){
        if (pResult != result){
          i = 0;
          Keyboard.write(three[i]);
          i++;
        }
        else if (pResult == result){
          if (delayB <=keyDelay){
            Keyboard.write(KEY_BACKSPACE);
            Keyboard.write(three[i]);
            i++;
            if (i == 7)
              i = 0; 
          }
          else{
            i = 0;
            Keyboard.write(three[i]);
            i++;
          }
        }
      }  
//4-----------------------------------------------------------------
       else if (result == buttonC[20]){
        if (pResult != result){
          i = 0;
          Keyboard.write(three[i]);
          i++;
        }
        else if (pResult == result){
          if (delayB <=keyDelay){
            Keyboard.write(KEY_BACKSPACE);
            Keyboard.write(four[i]);
            i++;
            if (i == 7)
              i = 0; 
          }
          else{
            i = 0;
            Keyboard.write(four[i]);
            i++;
          }
        }
      }     
//5-----------------------------------------------------------------
       else if (result == buttonC[21]){
        if (pResult != result){
          i = 0;
          Keyboard.write(three[i]);
          i++;
        }
        else if (pResult == result){
          if (delayB <=keyDelay){
            Keyboard.write(KEY_BACKSPACE);
            Keyboard.write(five[i]);
            i++;
            if (i == 9)
              i = 0; 
          }
          else{
            i = 0;
            Keyboard.write(five[i]);
            i++;
          }
        }
      }    
//6-----------------------------------------------------------------
       else if (result == buttonC[22]){
        if (pResult != result){
          i = 0;
          Keyboard.write(six[i]);
          i++;
        }
        else if (pResult == result){
          if (delayB <=keyDelay){
            Keyboard.write(KEY_BACKSPACE);
            Keyboard.write(six[i]);
            i++;
            if (i == 7)
              i = 0; 
          }
          else{
            i = 0;
            Keyboard.write(six[i]);
            i++;
          }
        }
      }   
//7-----------------------------------------------------------------
       else if (result == buttonC[23]){
        if (pResult != result){
          i = 0;
          Keyboard.write(seven[i]);
          i++;
        }
        else if (pResult == result){
          if (delayB <=keyDelay){
            Keyboard.write(KEY_BACKSPACE);
            Keyboard.write(seven[i]);
            i++;
            if (i == 7)
              i = 0; 
          }
          else{
            i = 0;
            Keyboard.write(seven[i]);
            i++;
          }
        }
      }   
//8-----------------------------------------------------------------
       else if (result == buttonC[24]){
        if (pResult != result){
          i = 0;
          Keyboard.write(eight[i]);
          i++;
        }
        else if (pResult == result){
          if (delayB <=keyDelay){
            Keyboard.write(KEY_BACKSPACE);
            Keyboard.write(eight[i]);
            i++;
            if (i == 7)
              i = 0; 
          }
          else{
            i = 0;
            Keyboard.write(eight[i]);
            i++;
          }
        }
      }  
//9-----------------------------------------------------------------
       else if (result == buttonC[25]){
        if (pResult != result){
          i = 0;
          Keyboard.write(nine[i]);
          i++;
        }
        else if (pResult == result){
          if (delayB <=keyDelay){
            Keyboard.write(KEY_BACKSPACE);
            Keyboard.write(nine[i]);
            i++;
            if (i == 9)
              i = 0; 
          }
          else{
            i = 0;
            Keyboard.write(nine[i]);
            i++;
          }
        }
      }
//0-----------------------------------------------------------------
       else if (result == buttonC[26]){
        if (pResult != result){
          i = 0;
          Keyboard.write(zero[i]);
          i++;
        }
        else if (pResult == result){
          if (delayB <=keyDelay){
            Keyboard.write(KEY_BACKSPACE);
            Keyboard.write(zero[i]);
            i++;
            if (i == 2)
              i = 0; 
          }
          else{
            i = 0;
            Keyboard.write(zero[i]);
            i++;
          }
        }
      }  
    }
    irrecv.resume();// receive the next value
    digitalWrite(10, mouse); //Mouse mode led
  }   

  //Update values
  pRead = results.value;
  pResult = result;
  ptime = timer;
}

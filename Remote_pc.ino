#include <IRremoteInt.h>
#include <IRremote.h> // use the library
#include "HID-Project.h"
int receiver = 9; // pin 1 of IR receiver to Arduino digital pin 11
int pResult = 0;
int pRead = 0;
int timer = 0;
int ptime = 0;
const int debounce = 150;
boolean mouse = false;
IRrecv irrecv(receiver); // create instance of 'irrecv'
decode_results results;

void setup()
{
  Serial.begin(9600); // for serial monitor output
  irrecv.enableIRIn(); // Start the receiver
  pinMode(10, OUTPUT); // Pin 9 output
  Consumer.begin();
  Keyboard.begin();
  AbsoluteMouse.begin();
}
void loop()
{
  int result;
  if (irrecv.decode(&results)) // have we received an IR signal?
  {
    timer = millis();
    
    if(results.value != -1)
      result = results.value;

/*    
    if(results.value == -1){
      if(pRead == -1){
        result = pResult;
      }
      else if(pRead != -1)
        delay(500);
        result = pResult;
    }
    
    
    
*/
if (timer - ptime >= debounce){
    Serial.println(result);
     // display it on serial monitor in hexadecimal        
        switch (result) {
      case 255:
        // mouse mode
        mouse = !mouse;
        delay(50);
        break;
      case 21165:
        // scroll up
        Mouse.move(0, 0, 2);
        break;
      case -11731:
        // scroll down
        Mouse.move(0, 0, -2);
        break;
      case 8925:
        // backspace
        Keyboard.write(KEY_BACKSPACE);
        break;
      case -20401:
        // tab
        Keyboard.write(KEY_TAB);
        break; 
      case -12241:
        // tab
        Keyboard.press(KEY_LEFT_SHIFT);
        Keyboard.press(KEY_TAB);
        Keyboard.releaseAll();  
        break;       
      case -32641:
        // mute
        Consumer.write(MEDIA_VOLUME_MUTE);
        break; 
      case 4845:
        Consumer.write(MEDIA_VOLUME_UP);
        break; 
      case -28051:
        // vol down
        Consumer.write(MEDIA_VOLUME_DOWN);
        break;
      case -14281:
        // up
        if(!mouse)
        Keyboard.write(KEY_UP_ARROW);
        else if(mouse)
        Mouse.move(0, -20);
        break;
      case 10455:
        // down
        if(!mouse)
        Keyboard.write(KEY_DOWN_ARROW);
        else if(mouse)
        Mouse.move(0, 20);
        break;
      case -30601:
        // left
        if(!mouse)
        Keyboard.write(K
        EY_LEFT_ARROW);
        else if(mouse)
        Mouse.move(-20, 0);
        break;    
      case 18615:
        // right
        if(!mouse)
        Keyboard.write(KEY_RIGHT_ARROW);
        else if(mouse)
        Mouse.move(20, 0);
        break;
      case 2295:
        // enter
        if(!mouse)
        Keyboard.write(KEY_ENTER);
        else if(mouse)
        Mouse.click(MOUSE_LEFT);
        break; 
      case 26775:
        // vol down
        Keyboard.write(KEY_LEFT_GUI);
        break; 
      case -22441:
        // exit
        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press(KEY_F4);
        Keyboard.releaseAll();
        break;
      case -4081:
        // multitasking
        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press(KEY_TAB);
        Keyboard.releaseAll();
        break;
        }
    }
    irrecv.resume();// receive the next value

    digitalWrite(10, mouse);
  }    
  pRead = results.value;
  pResult = result;
  ptime = timer;
}

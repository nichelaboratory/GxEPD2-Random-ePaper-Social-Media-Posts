//Randomised Social Media Posts display on Waveshare ePaper display
//See this video for a code walkthrough and demonstration of finished version: https://youtu.be/toWgL52biFA
//Based on the GxEPD2_HelloWorld.ino example in the GxEPD2 library by Jean-Marc Zingg
//This code assumes you're using the Waveshare 3.7 inch e-Paper HAT (480x280 pixels)
//If you're using another display then you'll have to comment out the 3.7 inch and
//uncomment your display in GxEPD2_display_selection_new_style
//You will also need to alter the co-ordinates of what's rendered on the screen
//This code is intended for the ESP32 microcontroller but could be adapted for other
//devices running Arduino C. Note the Arduino Uno doesn't have enough RAM for handling the images.
//Note: DO NOT use delay() in this code due to blocking. See:
//https://stackoverflow.com/questions/60940835/esp32-guru-meditation-error-core-0-paniced-loadprohibited-when-using-ble

#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <GxEPD2_4C.h>
#include <GxEPD2_7C.h>

//Fonts are normally (on Windows) in C:\Users\YOUR_WINDOWS_USERNAME\Documents\Arduino\libraries\Adafruit_GFX_Library\Fonts
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoOblique12pt7b.h>
#include <Fonts/FreeMono18pt7b.h>
#include <Fonts/FreeMono12pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>

//The image data is in images.h. Tutorial for converting bitmap images to the correct format: https://youtu.be/lzvnwu5lNvo
#include "images.h"

//ESP32 specific libraries for generating random numbers
#include <bootloader_random.h>
#include <esp_random.h>

// select the display class and display driver class in the following file (new style):
#include "GxEPD2_display_selection_new_style.h"

// or select the display constructor line in one of the following files (old style):
//#include "GxEPD2_display_selection.h"
//#include "GxEPD2_display_selection_added.h"

#define Display_X_Start_Pos 2 
#define Display_Y_Start_Pos 2
#define Avatar_Width 128
#define Avater_Height 128
#define LINE_LENGTH 24

#define uS_TO_S_FACTOR 1000000  //Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  3600 //3600 //Seconds 3600 seconds in one hour 3600*2 = 7200

String line1String = "";
String line2String = "";
String line3String = "";

//Class objects for characters. Add new properties as appropriate.
class Character {
  public:
  String name;
  String socialMediaHandle;
    Character() { }
    Character(String n, String h) { // Constructor with parameters
        name = n;
       socialMediaHandle = h; 
      }
};

//Instantiate the array of characters. If you add or remove items be sure to update the array's size.
Character characters[9] {{"Towa", "@theOriginalTowa"}, {"Rena", "@whenTheyCry"}, {"Kazuha", "@meekoo.n25"}, {"Rin", "@oleBlueEyes"}, {"Frieren", "@1000yoBaby"}, {"Kizuna AI", "@aiaiaichan"}, {"Ciel", "@i'mWayCuterThanArcueid"}, {"Asuna", "@Asuna_Yuuki"}, {"Yor", "@tennis_ace_yoru"}};
Character character1;
Character character2;

//Initialse the two random status updates. If you're using a smaller display panel you might only want to display a single message.
String message1;
String message2;

//Templates for random status updates.
char *messages[] = { "%N will shower and attempt to act normally.",
"%P is the best friend you could ever hope to have.",
"%P makes me forget about the world and all its problems.",
"%N will not be leaving the house today.",
"%N might quit %S for good.",
"It's looking like %F for dinner tonite.",
"Is depressed and will consume tonnes of %F tonight.",
"Wonders why %P always eats %F for breakfast...",
"%P is eating leftover %F again!!!",
"%N is in agony after standing on %P's %I :(",
"%P sold my %I to buy black market %B",
"%N is off to buy %B from the guy on the corner...",
"Needs to buy plenty of %F for tonight's party",
"I'm so bored of %S right now.",
"I'm so bored of %S now that %P has closed their account :(",
"Can't live without %F!",
"Is so shocked you can buy %B on %S!!!",
"I might not post much today my pet %A just died.",
"I went to buy %F but a crazy %A was attacking people!!!"
"I realised I shouldn't feed %F to my pet %A.",
"%P made my %A very sick after feeding it %F!",
"%N is definitely gonna quiet quit today.",
"It's not really doll weather today.",
"%P cried so much watching %M but I thought it sucked.",
"I thought %M was boring and overrated.",
"I just can't get over my %S ban.",
"If I had to do a nude scene with %C I wouldn't turn it down.",
"I don't wanna gossip but I saw %P hanging out with %C!",
"I saw %C %T but I don't want to brag about it.",
"Nothing can dim the light that shines from within.",
"I am in the right place at the right time, doing the right thing.",
"Am I good enough? Yes I am!!!",
"The perfect moment is this one.",
"Your life is about to be incredible.",
"Nothing can dim the light that shines from within.",
"You must do the things you think you cannot do.",
"If you think big, your world will be big.",
"Embrace the glorious mess that you are.",
"Failure is just another way to learn how to do something right.",
"Busy looking at photos of %L on %S.",
"%L was so much fun %T",
"%C should really move to %L",
"Just watching %M and eating %F"
"Watching %M and eating %F with %P"
"Off to %L tomorrow with %P",
"Meeting with %C tomorrow in %L",
"Got beat by %P at rock paper scissors :(",
"The ending of %M had me in tears ;`(",
"%S is just way too competitive these days.",
"Would a %A stand a chance against %C?",
"%C is trending for all the wrong reasons.",
"%P ate all the %F - again!!!!",
"I'm gonna smash %P with my %I"
};

//Arrays of random substitutions for status updates.
//%N = name, %P = protagonist, %F = foods, %I = items, %B = bad items, %S = social media sites, %A = animals, %M = movies, %C = celebs, %T = time, %L = location(city/country)
char *foods[] = { "pizza", "fruit", "pecan pie", "sushi", "pulled pork", "katsu chicken", "cheese", "ramen noodles", "soba noodles", "noodles", "ice cream", "candy", "potato chips", "wasabi peas", "fish and chips", "soup", "chicken soup", "deep fried squid", "Monster Munch"};
char *items[] = { "hairbrush", "gripping hands", "tennis shoes", "parasol", "umbrella", "iPad", "iPhone", "Lego", "teddy bear", "spare eyes", "sunglasses", "3d printed gun", "passport"};
char *badItems[] = { "drugs", "crack cocaine", "gravure", "idol DVDs", "ammunition", "cigarettes", "tobacco", "guns", "teddy bears", "spare eyes", "sunglasses", "3d printed guns", "bitcoins", "memecoins", "dogecoins", "crypto"};
char *socialMediaSites[] = { "social media", "Facebook", "X", "TikTok", "YouTube", "Reddit", "Instagram", "LINE", "Fortnite"};
char *animals[] = { "praying mantis", "cat", "dog", "giraffe", "monkey", "llama", "alpaca", "donkey"};
char *movies[] = { "Avatar", "The Lego Movie", "Tron", "Dune", "Schindler's List", "Star Wars", "Mannequin", "John Wick", "Toy Story"};
char *celebs[] = { "Barbie", "Elon Musk", "Donald Trump", "Ryan Gosling", "Hatsune Miku", "Metaverse Polar", "Misa Amane"};
char *times[] = { "yesterday", "last week", "last Sunday", "last Tuesday", "the day before yesterday", "recently", "a while back", "the other day"};
char *locations[] = { "Tokyo", "Akihabara", "Shinjuku", "Yokohama", "New York", "Kyoto", "Osaka", "Hokkaido"};


void setup()
{

  //delay(5000); //Slow ESP32 down after sleeping
  
  //A call to bootloader_random_enable initialises the ESP32 random generator if wifi/bluetooth is disabled.
  //If you're using wifi/bluetooth then random numbers are available AFTER the wifi system has been enabled.
  //See https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/system/random.html
  bootloader_random_enable();
  randomSeed(esp_random());

  //NOTE: I tried making the ESP32 sleep then wake up and refresh the display every couple of hours.
  //Unfortunately there's either something wrong with my code, or one of the ESP32 libraries or maybe
  //I just have a faulty microcontroller. If you can solve this issue then let me know.
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

  //Select a random character
  character1 = characters[random(sizeof(characters) / sizeof(Character))];
  character2 = characters[random(sizeof(characters) / sizeof(Character))];
  
  //Keep choosing a second character until they're different to the first character
  do {
    character2 = characters[random(sizeof(characters) / sizeof(Character))];
  } while (character1.name == character2.name);
  
  //Initialise the Waveshare ePaper display panel
  //display.init(115200); // default 10ms reset pulse, e.g. for bare panels with DESPI-C02
  display.init(115200, true, 2, false); // USE THIS for Waveshare boards with "clever" reset circuit, 2ms reset pulse

  //Retrieve the random messages
  message1 = getRandomMessage();
  message2 = getRandomMessage();

  updateDisplay(); //Update the display
  display.hibernate();

  //delay(1000);
  //Serial.flush();
  //Serial.println("ESP32 is going to sleep...");
  //delay(5000);

  //Again I couldn't get this code to work when the ESP32 sleeps
  esp_deep_sleep_start();

}

//Updates the ePaper display panel
static void updateDisplay()
{
  display.setRotation(1);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.fillScreen(GxEPD_WHITE);
  
  display.setFullWindow();
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);

    //Draw character1's avatar on the display
    renderCharacterAvatar(character1.name, Display_X_Start_Pos, Display_Y_Start_Pos, 128, 128);

    //Name 1
    display.setCursor(Display_X_Start_Pos + Avatar_Width + 5, Display_Y_Start_Pos + 32);
    display.setFont(&FreeMono18pt7b);
    display.print(character1.name);

    //Hashtag 1
    display.setCursor(Display_X_Start_Pos + Avatar_Width + 5, Display_Y_Start_Pos + 53);
    display.setFont(&FreeMonoOblique12pt7b);
    display.print(character1.socialMediaHandle);

    //Call the function to wordwrap the message onto 3 lines
    splitMessageIntoLines(message1);

    display.setCursor(Display_X_Start_Pos + Avatar_Width + 14, Display_Y_Start_Pos + 78);
    display.setFont(&FreeMono12pt7b);
    display.print(line1String);
    
    display.setCursor(Display_X_Start_Pos + Avatar_Width + 14, Display_Y_Start_Pos + 100);
    display.setFont(&FreeMono12pt7b);
    display.print(line2String);

    display.setCursor(Display_X_Start_Pos + Avatar_Width + 14, Display_Y_Start_Pos + 122);
    display.setFont(&FreeMono12pt7b);
    display.print(line3String);

    //Draw character1's avatar on the display
    renderCharacterAvatar(character2.name, Display_X_Start_Pos, Display_Y_Start_Pos + 128 + 15, 128, 128);

    //Name 2
    display.setCursor(Display_X_Start_Pos + 128 + 5, Display_Y_Start_Pos + 31 + 128 + 5);
    display.setFont(&FreeMono18pt7b);
    display.print(character2.name);

    //Hashtag 2
    display.setCursor(Display_X_Start_Pos + 128 + 5, Display_Y_Start_Pos + 31 + 128 + 5 + 20);
    display.setFont(&FreeMonoOblique12pt7b);
    display.print(character2.socialMediaHandle);

    //Message 2
    splitMessageIntoLines(message2);
    display.setCursor(Display_X_Start_Pos + Avatar_Width + 14, Display_Y_Start_Pos + 209);
    display.setFont(&FreeMono12pt7b);
    display.print(line1String);

    display.setCursor(Display_X_Start_Pos + Avatar_Width + 14, Display_Y_Start_Pos + 231);
    display.setFont(&FreeMono12pt7b);
    display.print(line2String);

    display.setCursor(Display_X_Start_Pos + Avatar_Width + 14, Display_Y_Start_Pos + 253);
    display.setFont(&FreeMono12pt7b);
    display.print(line3String);

  }
  while (display.nextPage());
  

}

void loop() {}; //Loop not required

//Gets a random status update
static String getRandomMessage() {

  //To select a random item from an array use size of array divided by size of what type the array contains
  uint8_t messageNumber = random(sizeof(messages)/sizeof(char*)); 
  String message = messages[messageNumber];
  String food = foods[random(sizeof(foods)/sizeof(char*))];
  String item = items[random(sizeof(items)/sizeof(char*))];
  String badItem = badItems[random(sizeof(badItems)/sizeof(char*))];
  String socialMediaSite = socialMediaSites[random(sizeof(socialMediaSites)/sizeof(char*))];
  String movie = movies[random(sizeof(movies)/sizeof(char*))];
  String celeb = celebs[random(sizeof(celebs)/sizeof(char*))];
  String animal = animals[random(sizeof(animals)/sizeof(char*))];
  String time = times[random(sizeof(times)/sizeof(char*))];
  String location = locations[random(sizeof(locations)/sizeof(char*))];
  Character character = characters[random(sizeof(characters) / sizeof(Character))];
  Character protagonist = characters[random(sizeof(characters) / sizeof(Character))];

  //Don't get the same character if we're already using them
    do {
    protagonist = characters[random(sizeof(characters) / sizeof(Character))];
  } while (character.name == protagonist.name);

  //Substitutions to make the messages appear much more random
  message.replace("%N", character.name);
  message.replace("%P", protagonist.name);
  message.replace("%F", food);
  message.replace("%I", item);
  message.replace("%B", badItem);
  message.replace("%S", socialMediaSite);
  message.replace("%A", animal);
  message.replace("%M", movie);
  message.replace("%C", celeb);
  message.replace("%T", time);
  message.replace("%L", location);

  return message;

}

//Display a bitmap on the display.
//How to draw bitmaps on ePaper displays: https://youtu.be/lzvnwu5lNvo
static void renderCharacterAvatar(String name, uint8_t xPos, uint8_t yPos, uint8_t width, uint8_t height) {

  static unsigned char *characterData;

  if (name == "Towa")
    characterData = towaData;
  if (name == "Rena")
    characterData = renaData;
  if (name == "Kazuha")
    characterData = kazuhaData;
  if (name == "Rin")
    characterData = rinData;
  if (name == "Len")
    characterData = lenData;
  if (name == "Frieren")
    characterData = frierenData;
  if (name == "Kizuna AI")
    characterData = kizunaData;
  if (name == "Ciel")
    characterData = cielData;
  if (name == "Asuna")
    characterData = asunaData;
  if (name == "Yor")
    characterData = yorData;    

  display.drawXBitmap(xPos, yPos, characterData, width, height, GxEPD_BLACK);

}

//My second attempt at splitting a message so it word wraps onto 3 lines. There's definitely
//a more elegant way of achieving this but I'm still a C noob. My original attempt using
//char arrays and strtok went horribly wrong. Incidentally I think the GxEPD2 library will
//wordwrap into a box but it doesn't split words elegantly.
static void splitMessageIntoLines(String originalMessage) {

    int currentLineNumber = 1;
    int currentLinePosition = 0;
    String currentWord = "";
    String currentLine = "";
    String lineToStore = "";
    bool haveFinishedLine = false;
    line1String = "";
    line2String = "";
    line3String = "";

    for(int i = 0; i < originalMessage.length(); ++i) {

        if (originalMessage.substring(i, i + 1) == " " || (i - 1) == originalMessage.length()) {
          //New word
          
          //Does it fit on current line?
          if (currentWord.length() < (LINE_LENGTH - currentLinePosition + 1)) {
              currentLine += currentWord + " ";
              currentLinePosition += currentWord.length() + 1;

          } else {
            //Make new line and put word on new line
            lineToStore = currentLine;
            haveFinishedLine = true;
            currentLine = currentWord + " ";
            currentLinePosition = currentLine.length() + 1;
          }

          currentWord = "";

        } else {
          currentWord += originalMessage.substring(i, i + 1);
        }

        //Handle the end of the message gracefully
        if (i == (originalMessage.length() - 1)) {
          
          if (currentWord.length() < (LINE_LENGTH - currentLinePosition + 1)) {
            lineToStore = currentLine + currentWord;
            currentWord = "";
          } else {
            lineToStore = currentLine;
          }
          
          haveFinishedLine = true;
        }

      if (haveFinishedLine) {

         switch(currentLineNumber) {
              case 1:
                line1String = lineToStore;
                break;
              case 2:
                line2String = lineToStore;
                break;
              case 3:
                line3String = lineToStore;
                break;                                
          }
          ++currentLineNumber;
          haveFinishedLine = false;
      }

    }

    if (currentWord.length() > 0) {
        switch(currentLineNumber) {
          case 2:
            line2String = currentWord;
            break;
          case 3:
            line3String = currentWord;
            break;                                
        }
    }
   
}
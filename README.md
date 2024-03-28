# GxEPD2-Random-ePaper-Social-Media-Posts
 GxEPD2 demo project for ESP32 and Waveshare ePaper Display
 
 I thought I'd share this little beginner project I made using the ESP32 microcontroller and a Waveshare ePaper display. What you will learn: basic C++ classes, Arduino String class, randomised data including string construction, displaying of bitmaps on ePaper panels, sleep and wake up ESP32. It's a nice idea for a school or university microcontroller project. You don't have to use anime dolls - you could use anything else that has a social media presence.

Full code walkthrough in this video: https://youtu.be/toWgL52biFA

[![GxEPD2 demo project for ESP32 and Waveshare ePaper Display](https://img.youtube.com/vi/toWgL52biFA/0.jpg)](https://www.youtube.com/watch?v=toWgL52biFA)

All you need make this device is an ESP32, a Waveshare ePaper display and The Gimp freeware graphics package. Libraries used include the Adafruit Gfx library and the GxEDP2 ePaper library. The Waveshare display I'm using is the 3.7 inch 280x480 pixel monochrome panel.

Important: due to the extensive use of the String class DO NOT use delay() in this Sketch. I should really use char arrays rather than the String class but I'm a C++ noob. There's still a Guru Meditation issue with the ESP32 crashing when attempting to sleep. If I resolve the issue I'll post an update. In general embedded C isn't really the greatest programming language for doing a lot of string manipulation as it shreds the memory. It may (or may not) be better to use Python instead.

ESP32 random number seeding: https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/system/random.html

Easy ESP32 ePaper tutorial: https://youtu.be/El38zVmog14
How to draw bitmaps on ePaper displays: https://youtu.be/lzvnwu5lNvo

# ESP32 Numitron OTA Watch

<center>

MAIN            |
:-------------------------:|
|![](/images/photo_main.jpg)        

TOP            |  BOTTOM
:-------------------------:|:-------------------------:                           
![](/images/numitron.png)  |  ![](/images/numitron_b.png)

</center>

This is the first OTA capable Numitron watch!!! (At least the first one I have seen ;))

This simple example provides

- Gerber Files for a Numitron Watch and generated GCode (with flatcam) for milling the PCB (with grbl)
- Code provides a driver for the watch
- Watch will update its time and date from the internet via NTPClient
- A button press will switch between time and date (shown for 3 seconds)

Use this as a template for your projects it can be very handy in various ways ;). Please let me know if you have anything to improve or commit.

Cheers

## How to use example

### Configure the project

First of all clone the project (In this project I used PlatformIO for development)

```
git clone
cd
```

### Build and Flash

Build the project and flash it to the board, then run monitor tool to view serial output:

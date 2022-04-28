# IoT Dimmer with ESP32

This is an IoT dimmer project for controlling light brightness through a mobile app. For this, it is used an ESP32 microcontroller, that connects to the Wi-Fi and communicates with the smartphone to receive the brightness parameters from the app.

![Alt text](https://github.com/lucasmazz/iot-dimmer/blob/main/schematic.jpg?raw=true)

In short, ESP32 is used to receive brightness parameters, detect the zero crossing of the power supply waveform and trigger the TRIAC, which is responsible for setting the brightness of the light.

## Components:
The complete list of components includes:
- 1x ESP32 microcontroler
- 1x TRIAC
- 1x 4N25 optocoupler
- 1x MOC3021 optocoupler
- 1x transformer
- 4x diodes
- 2x 100 Ω resistors
- 1x 330 Ω resistor
- 1x 10K Ω resistor

## How to use it:
Be careful. This circuit uses voltages that can cause serious injury. If you don't have experience with electricity, look for someone who can help you.

Connect the circuit as shown in the schematic. The optocouplers are being used to isolate the ESP32 from the rest of the circuit. 

The 4N25 is used for zero crossing detection. In this configuration, it is connected to **V5**, **GPIO 12** and **GND**. In the ESP32 code, the reference to GPIO 12 is called **INPUT_PIN**. If you want to change the zero detection GPIO pin, just change the pin number in the code. 

The MOC3021 is used to trigger the TRIAC. It is connected to **GPIO 14** and **GND**. If you want to change the TRIAC trigger GPIO pin, it is necessary to change the **OUTPUT_PIN** number in the ESP32 code.

Also, change in the ESP32 code the **SSID** value for your Wi-Fi network name and the **PASSWORD** for your Wi-Fi password, before compiling and running code on the microcontroller.

It is important to look at the reference frequency of the power supply. In the country where I live the electrical grid has a frequency of 60Hz; if where you live the electrical grid works at 50Hz, it is also necessary to change the value of the FREQUENCY on the ESP32 code.

After that, you can compile and upload the app to your Android smartphone. For the mobile app, it is necessary to enter the IP address of the ESP32 on the network. If the microcontroller is connected to the computer through USB, the ESP32 program will print the local IP at startup on the serial monitor. You can usually also check this address from the Wi-Fi router's configuration panel.

Finally, you can change the value of the seek bar on the app to change the brightness of the light connected to the TRIAC.

## Compiling

####  ESP32 
To simplify development, I used [PlatformIO](https://platformio.org/) plugin on [Visual Studio Code](https://code.visualstudio.com/) to develop, compile and deploy the code on the microcontroller. You can also use the default Arduino's development platforms and libraries to compile and deploy the code if you want. Read more on the [ESPRESSIF official documentation](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html) .

If you are using [PlatformIO](https://platformio.org/), just select "Open Project" and open the ESP32 folder, then set the **SSID** and **PASSWORD** on the code, and you are good to go, build it and upload it to ESP32. 

####  Android
For the development, compilation and deployment of the app code, I used [Android Studio](https://developer.android.com/studio), the conventionally used IDE for android application development. So, just open the Android folder as a project and the mobile app can now be compiled and deployed to your smartphone with Android Studio.
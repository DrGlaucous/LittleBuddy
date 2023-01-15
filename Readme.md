

## Little Buddy
---
![image](https://github.com/DrGlaucous/LittleBuddy/raw/main/Images/AyeSkipper.jpg)

---
This repo contains the tools needed to construct the LittleBuddy, an STM32 device that can emulate an iClicker by sending and receiving multiple-choice answers built on the [iSkipper](https://github.com/wizard97/iSkipper) reverse-engineering project.

*Note: The creators of this repo are ***not*** responsible for improper use of this project.*

### Why?
---
iClickers are commonly employed in the educational environment and have gained a reputation for being a limiting, overly-priced, and yet a disappointingly essential part of lectures everywhere.

Due to the hard work of others who felt the same way, it was realized that the iClicker uses some fairly standard off-the-shelf parts to do its dirty work. If you want to learn more about this background, read these articles:
* [Academic Report by Daniel Wisner III, Jacob Glueck](https://github.com/wizard97/iSkipper/releases/download/v1.0.0/iskipper.pdf)
* [Blog Post by Ammar Askar](https://blog.ammaraskar.com/iclicker-reverse-engineering/)

<figure>
<img src="https://github.com/DrGlaucous/LittleBuddy/raw/main/Images/SDRCapture.PNG" alt="Trulli" style="width:100%">
<figcaption align = "center"><i>LittleBuddy running a "random answer" routine on channel AA, which shifts the radio frequency to 917MHz</i></figcaption>
</figure>

From the respective iSkipper pages, setting up and running one of these devices should be fairly straightforward, but I ran into some documentation gaps when trying to create it for myself.
The original repo is now a few years old, and somewhat out of date. I ported the firmware to PlatformIO and set it up to be used with the STM32 BluePill, which was not without several hitches.
For one, there wasn't any sort of official wiring diagram to go by (The original maintainer used an [Adafruit feather](https://learn.adafruit.com/adafruit-feather-m0-radio-with-rfm69-packet-radio), which has a built-in radio transceiver). Additionally, the diagrams on the internet all seemed to vary in one way or another. It was difficult to determine the correct number of pins for this application.


### Building the LittleBuddy
---

As stated earlier, I used an STM32 BluePill for this project. Why the BluePill?
For one, the STM32 natively uses a 3.3 volt interface, perfectly matched to the RFM69's requirements, as the radio cannot take the 5 volts many Arduinos and clones put out. This means the only active components needed to have a fully functioning iSkipper is the BluePill and the radio.
The other reason:  BluePills are ***Cheap!*** Depending on where you look or how long you are willing to wait, these devices can be picked up for around 2 dollars a pop, impressive considering what they are capable of.

Here is a list of the parts needed to build a LitteBuddy:
  * **STM32 BluePill** *(other devices may work, but check the pinouts*)
  * **RFM69W or RFM69HW** radio transceiver, **915MHz frequency** *(These radios come in several frequency ranges, be sure to get the right one. Also, they come in two different package types. Both should work, but you will have to change where the pins go if you have the other type)*
  * **Fine gauge wire or leads** *(Leads or solid-core wire are good for some of the pins. They'll help hold the radio firmly in place)*

To stitch the radio to the BluePill, I followed this [guide from glidernet.org](http://wiki.glidernet.org/stm32-ogn-tracker). They did a great job of explaining the wiring process. Of course, the only part that is needed for the LittleBuddy is the RFM69, *unless you ***really*** want to add a GPS!*

For those who are too lazy to go to that website, here are a few important resources:

| STM32 pin | RFM69(H)W pin     | function  | signal direction  |
| --------- | -------------     | --------  | ----------------  |
|3.3V       |3.3V               |power      |LDO -> RF chip     |
|G	        |GND                |common     |ground             |	
|A4         |NSS	            |SPI select	|CPU -> RF chip     |
|A5	        |SCK	            |SPI clock	|CPU -> RF chip
|A6         |MISO	            |SPI MISO	|RF chip -> CPU
|A7         |MOSI	            |SPI MOSI	|CPU-> RF chip
|B5	|RESET	|chip reset	        |CPU -> RF chip
|B4	|DIO0	|interrupt request	|RF chip -> CPU
|B3	|DIO4	|aux. status	    |RF chip -> CPU
<figcaption align = "center"><i>Pinout table</i></figcaption>
<br>

<figure>
<img src="http://ognproject.wdfiles.com/local--resized-images/stm32-ogn-tracker/RF_wiring.png/medium.jpg" alt="Trulli" style="width:100%">
<figcaption align = "center"><i>Where those pins should go</i></figcaption>
</figure>
<br>

I've also included a Frtizing schematic of the wiring:

<figure>
<img src="https://github.com/DrGlaucous/LittleBuddy/raw/main/Images/LittleBuddy_Graphic.svg" alt="Trulli" style="width:50%"align = "center">
<img src="https://github.com/DrGlaucous/LittleBuddy/raw/main/Images/LittleBuddy_Schem.svg" alt="Trulli" style="width:50%" align = "right">
<br><br>
</figure>


The finished product should look something like this:
<figure>
<img src="https://github.com/DrGlaucous/LittleBuddy/raw/main/Images/ViewLeft.jpg" alt="Trulli" style="width:50%"align = "left">
<img src="https://github.com/DrGlaucous/LittleBuddy/raw/main/Images/ViewRight.jpg" alt="Trulli" style="width:50%" align = "right">
<br><br><br><br><br><br>
</figure>

---
As for the antenna, I followed the radio [hookup guide here](https://www.mysensors.org/build/connect_radio). The antenna should be cut to roughly 80mm (leaving extra for soldering in place). *(Note: it is important to use solid-core wire for this step. Braided wire significantly reduces the effectiveness of the antenna)* 
**You need to include an antenna or else you risk burning out the radio.**


| Frequency | Length (in)     | Length (mm)  |
| --------- | -------------   | --------     | 
| 915 MHz | 3.06"     | 77.9 mm  |

To reduce the space of the antenna, I coiled it around a pencil. Although this reduces the range, it should still be well effective enough for any lecture hall, and then some.


### Programming the LittleBuddy
---
For this project, I used PlatformIO, an impressive development environment built on top of Microsoft's VisualCode IDE.

Because the STM32 doesn't have a built-in usb to serial converter, you'll need an ST-Link to flash the board. You can get one of those cheap "ST-Link v2" clones for about the same price as the BluePill, and in my experience, they seem to work just fine. Additionally, the ST-Link also allows the board to be debugged, a crucial addition if you run into any unforseen program issues.
(Some of these boards also have bootloaders that do this, too, but that adds an extra layer of complexity. I've found these bootloaders to be flaky and unreliable, the ST-link just makes the whole thing easier)

The code in this repository should be ready-configured to work with the STM-32, but the sample code in the original [iSkipper repo](https://github.com/wizard97/iSkipper) should work with minimal alteration as well.
Here are a few things to note when configuring any of this original code or making your own firmware:

* The `IRQ_PIN` is the interrupt pin, or `D0` on the radio. This pin is set to `HIGH` when the RFM69 receives a packet. if you followed the assembly instructions above, this should be pin `PB4`

* The `CSN` pin is `NSS` on the radio. This one is `PA4` on the BluePill. This pin serves as the chip select pin, telling the radio that the processor is trying to talk to it.

* The reason that the other pins don't need to be specified is because the STM32 uses hardware SPI to talk to the RFM69, so those pins will always have a fixed location.

* Some of the Chineese BluePill clones won't use authentic STM32 chips. This doesn't affect anything functionally, but these fake chips have a different `CPUTAPID`. That means that the upload will fail with an error. To fix this, un-comment the following line of code in `platformio.ini`:
`upload_flags = -c set CPUTAPID 0x2ba01477`
if needed, change the hex value to match that of your device.

* If uploading with the ST-link fails, try using [zadig](https://zadig.akeo.ie/) to update the driver. You will need to go to `Options>List All Devices` and enable it to see the ST-Link. Install the latest WinUSB driver.

---
Happy Quizzing!

![image](https://github.com/DrGlaucous/LittleBuddy/raw/main/Images/Posed.jpg)
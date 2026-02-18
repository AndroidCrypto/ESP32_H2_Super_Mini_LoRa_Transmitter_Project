These measures are done, the values are in mA with the ESP32-H2 board
NOT mounted on an Expansion board.

The data in the first line each is measured with 96 MHz CPU frequency, the second line
has the data for a CPU frequency of 48 MHz. The CPU frequency is toggling between each
deep sleep wake up.

The first row is the "Battery powered", the second row the "USB powered" current.
The first value is the "96 MHz", the second value the "48 MHz" current.

01) get the wakeup reason and toggle the CPU frequency, blinking the RGB LED
9.6 / 7.9
10.5 / 9.6 mA

02) put the onboard RGB LED on with RED-GREEN-BLUE
9.6 /  8.4
10.7 / 9.5 mA

03) wait for Deep Sleep
9.3 / 7.9
10.6 / 9.2 mA

04) Blink the onboard LED twice, followed by a Deep Sleep
0.42 / 0.42
1.0 / 1.0 mA

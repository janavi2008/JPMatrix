# JPMatrix
a scalable led matrix display where 'tiles' can connect to each other in a row to allow for any size display. each tile has a 8x8 matrix. it runs on a rapberry pi pico that has to be connected externally, with shift registers for multiplexing, and a logic level shifter ic to convert the 3.3v pico outputs to 5v for the shift register inputs.
im writing the firmware to display conways game of life, because i think itd look super cool.

# How it works
multiplexing is a way to control a bunch of leds using fewer pins on the microcontroller. basically, instead of connecting each led to its own output pin, the leds are arranged in a matrix, and all anodes in one column are tied together, and the cathodes in one row are tied together. each row and column is connected to a microcontroller pin. the microcontroller passes current through one row at a time and simultaneously connecting the columns with whichever leds are supposed to be on for that row. it cycles through the rows super fast, way faster for the human eye to comprehend, so it looks as if the leds are all lit up at the same time, when in reality, only one row is on at a time- this is known as persitance of vision. however, this still would use one output pin for each row and column (so 16 per tile) and thats not feasable, which is why im using shift registers.
shift registers let you transmit the data serially over one data line rather than one for each row and column, drastically saving the pin count. it used 4 pins - data, latch, clock, and oe(output enable). the data pin sends the data to the shift registers, the clock pin handles the timing(when to read the data pin), latch saves the data once its been loaded, and the oe manages when the data is outputted to the leds. multiple shift registers can be used by daisy chaining without needing more pins - each chip has a data in and a data out, so the data out from one chip can connect to the data in to another. im using one shift register for columns, and one for the rows. the one for the rows is a power shift register, with internal mosfets on each line, so it can handle higher currents.

# Design
i desinged a pcb, using IC 74HCT595 as a shift register, TPIC6C595 as a power shift register, 74HCT125 for logic level shifting, 64 LEDs, and input header pins to connect to the previous tile or the raspberry pi and output header pins to connect to the next tile. i also put in some resistors and decoupling capacitors.

<img width="477" height="338" alt="image" src="https://github.com/user-attachments/assets/f8079e8b-5ef5-42dc-b1cd-fdda49830d24" />

<img width="212" height="359" alt="image" src="https://github.com/user-attachments/assets/51fc910c-15fc-4e18-b611-14eb6cfffb6a" />
<img width="224" height="360" alt="image" src="https://github.com/user-attachments/assets/53f4c04c-9c61-4140-8016-ad93b12987c2" />

<img width="260" height="409" alt="image" src="https://github.com/user-attachments/assets/933c7029-4ae0-44b2-9430-35d62299e807" />

# BOM

| Item | Qty | Supplier | Total Cost |
| :--- | :---: | :--- | ---: |
| raspberry pi pico | 1 | robu | $4.05 |
| TPIC6C595 | 2 | robu | $3.10 |
| 74HCT595 | 2 | robu | $1.00 |
| 74HCT125 | 2 | robu | $0.78 |
| 0805 smd LED | 128 | robu | $1.4 |
| 0603 smd 0.1uf capacitor | 6 | robu | $0.15 |
| 0603 smd 100uf electrolytic capacitor | 2 | robu | $0.22 |
| 0603 330 ohm resistor | 16 | robu | $0.15 |
| 0603 10k resistor | 2 | robu | $0.15 |
| 0603 100 ohm resistor | 2 | robu | $0.15 |
| male header pins | 16 | robu | $0.2 |
| pcb manufacture | 5 | zbotic | $10.00 |

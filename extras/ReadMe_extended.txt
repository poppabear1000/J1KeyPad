Greetings,

There is no .cpp file in this 
library, it is a very small code
library and as such it is all 
contained in the ".h" file.

The "example" program given, uses
the typical standard 4x4 bubble/button
matrix keypad.
like the one shown here: (Jameco, part#: 2333119)
https://www.jameco.com/z/27899-JVP-Jameco-ValuePro-Flat-4x4-Matrix-Membrane-Keypad-8-Pin-Interface_2333119.html?CID=GOOG&srsltid=AfmBOoryKk4KG2iKC4dc_s0oscDhIP3szTamkawN4RJB3zKdFeGVaV53eLA

The example uses I/0 pins, 0,1,2,3,4,5,6,7,8 (and I used an ESP 32 generic board).
if the board you plan to use, (like an uno), then you might want to start at pin 2-10 instead,
to not block rx/tx on the uno (and some other boards).

Enjoy,

Scott Shafer
This is an Arduino library for using Matrix Keys (or Keyboards).
It's advantage is:
	it has build in debounce (you can also specify a time), 
	does not block,
	will notify on changes only, 
	Multiple keys can be depressed at one time and be captured (upto 8 simultaneous key presses). 
	It can show key state (up or down),
	has up to 8 key scan "Histories",
	Can be used for up to a 8x8 Matrix Key board.
	
The scan time (in MS)
The scan count (how many times the scan runs for the state of the key remaining "pushed" for it to be considered a valid push)
A Keep-Alive value if you want the keys current status to be sent out to something at some time value in MS.

NOTE!: There is no .cpp file, it is short and all in the .h file

The "example" program given, uses
the typical standard 4x4 bubble/button
matrix keypad.
like the one shown here: (Jameco, part#: 2333119)
https://www.jameco.com/z/27899-JVP-Jameco-ValuePro-Flat-4x4-Matrix-Membrane-Keypad-8-Pin-Interface_2333119.html?CID=GOOG&srsltid=AfmBOoryKk4KG2iKC4dc_s0oscDhIP3szTamkawN4RJB3zKdFeGVaV53eLA

The example uses I/0 pins, 0,1,2,3,4,5,6,7,8 (and I used an ESP 32 generic board).
if the board you plan to use, (like an uno), then you might want to start at pin 2-10 instead,
to not block rx/tx on the uno (and some other boards).

Enjoy,

Scott

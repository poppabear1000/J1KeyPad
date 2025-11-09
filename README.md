NOTE!: There is no .cpp file, it is short and all in the .h file
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

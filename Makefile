main: main.cpp Keypad.cpp
	g++ main.cpp Keypad.cpp Key.cpp -o burglar_alarm -lwiringPi -lwiringPiDev -lADCDevice

run: burglar_alarm
	sudo ./burglar_alarm

clean:
	rm -f burglar_alarm
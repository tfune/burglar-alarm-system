CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude
LDFLAGS = -lwiringPi -lwiringPiDev -lADCDevice

SRC = src/main.cpp src/Key.cpp src/Keypad.cpp
OUT = burglar_alarm

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

run: all
	sudo ./$(OUT)

clean:
	rm -f $(OUT)
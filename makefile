makefile:
all: balldetector

clean:
	-rm main.o Serial.o ComProtocol.o balldetector.exe balldetector

balldetector: main.o Serial.o ComProtocol.o
	g++ -g -std=c++17 -o balldetector main.o Serial.o ComProtocol.o -lwiringPi `pkg-config --cflags --libs opencv`

main.o: main.cpp
	g++ -c -g -std=c++17 main.cpp `pkg-config --cflags --libs opencv`

Serial.o: Serial.cpp
	g++ -c -g -std=c++17 Serial.cpp -lwiringPi

ComProtocol.o: ComProtocol.cpp
	g++ -c -g -std=c++17 ComProtocol.cpp -lwiringPi

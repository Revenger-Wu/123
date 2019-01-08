objects = underrobot.o ComSocketSer.o ComSerial.o

underrobot : $(objects) 
	gcc -o underrobot $(objects) -lpthread -lwiringPi
	-rm $(objects)

%.o : %.c
	gcc -c $< -lpthread -lwiringPi

.PHONY : clean
clean:
	-rm underrobot
	

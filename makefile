make: LaWarre_Nathan_HW4.cpp
	g++ -g -Wall -o main LaWarre_Nathan_HW4.cpp -lmysqlcppconn

run: main
	./main

clean:
	rm -f *.o core main

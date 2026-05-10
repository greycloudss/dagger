CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20
LDFLAGS = -lpcap -lpthread

TARGET = dagger

SRC = main.cpp \
      features/sharking.cpp

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

re: clean all

.PHONY: all clean re
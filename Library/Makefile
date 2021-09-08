TARGET = zstack.a

SRC :=
SRC += $(wildcard ./Misc/*.cpp ./VSilicon/*.cpp ./3rdparty/*.cpp ./Algo/*.cpp)
SRC += $(wildcard ./Algo/BasicMath/*.cpp)
SRC += $(wildcard ./Algo/Statistics/*.cpp)
SRC += $(wildcard ./Codec/*.cpp)

OBJ = $(patsubst %.cpp, %.o, $(SRC))

INCLUDE = -I../../Include

CC = g++
CFLAGS = -Wall -c -lstdc++ -fpermissive

$(TARGET): $(OBJ)
	ar -crv $(TARGET) $(OBJ)
	ranlib $@

%.o: %.cpp
	$(CC) $(INCLUDE) $(CFLAGS) $< -o $@ -DDEBUG

%.o: %.c
	$(CC) $(INCLUDE) $(CFLAGS) $< -o $@ -DDEBUG

.PHONY: clean
 
clean:
	rm -rf $(OBJ) $(TARGET)

test:
	echo $(OBJ)
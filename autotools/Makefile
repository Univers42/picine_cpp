CC=
CXX=c++
CFLAGS=
CXXFLAGS=-Wall -Wextra -Werror
LDFLAGS=
SRC=srcs.c ex01.c ex02.c ex03.c ex04.c ex05.c ex06.c ex07.c ex08.c ex09.c ex10.c ex11.c ex12.c ex13.c ex14.c ex15.c ex16.c ex17.c ex18.c ex19.c ex20.c ex21.c ex22.c ex23.c ex24.c ex25.c ex26.c ex27.c ex28.c ex29.c ex30.c ex31.c ex32.c ex33.c ex34.c ex35.c ex36.c ex37.c ex38.c ex39.c ex40.c ex41.c ex42.c ex43.c ex44.c ex45.c ex46.c ex47.c ex48.c ex49.c ex50.c ex51.c ex52.c ex53.c ex54.c ex55.c ex56.c ex57.c ex58.c ex59.c ex60.c ex61.c ex62.c ex63.c ex64.c ex65.c ex66.c ex67.c ex68.c ex69.c ex70.c ex71.c ex72.c ex73.c ex74.c ex75.c ex76.c ex77.c ex78.c ex79.c ex80.c ex81.c ex82.c ex83.c ex84.c ex85.c ex86.c ex87.c ex88.c ex89.c ex90.c ex91.c ex92.c ex93.c ex94.c ex95.c ex96.c ex97.c ex98.c ex99.c 
OBJ=$(SRC:.cpp=.o)
TARGET=

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re
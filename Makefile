NAME = libftpp.a

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17

SRC_PATH = sources/
OBJ_PATH = objects/

SRCS = sources/dataStructures/data_buffer.cpp \
       sources/designPatterns/memento.cpp
OBJS = $(patsubst $(SRC_PATH)%.cpp, $(OBJ_PATH)%.o, $(SRCS))

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

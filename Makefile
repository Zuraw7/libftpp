NAME = libftpp.a

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17

SRC_PATH = sources/
OBJ_PATH = objects/

SRCS = sources/dataStructures/data_buffer.cpp \
	   sources/designPatterns/memento.cpp \
	   sources/iostream/thread_safe_iostream.cpp \
	   sources/mathematics/perlin_noise_2D.cpp \
	   sources/mathematics/random_2D_coordinate_generator.cpp \
<<<<<<< HEAD
	   sources/network/client.cpp \
	   sources/network/message.cpp \
	   sources/network/server.cpp \
=======
>>>>>>> 7975600 (Added Random2DCoordinateGenerator class)
	   sources/network/client.cpp \
	   sources/network/message.cpp \
	   sources/network/server.cpp \
	   sources/thread/persistent_worker.cpp \
	   sources/thread/thread.cpp \
	   sources/thread/worker_pool.cpp

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

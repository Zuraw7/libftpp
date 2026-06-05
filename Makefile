LIB_DIR = lib/
NAME = $(LIB_DIR)libftpp.a

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17

SRC_PATH = sources/
OBJ_PATH = objects/

SRCS = sources/dataStructures/data_buffer.cpp \
	   sources/designPatterns/memento.cpp \
	   sources/iostream/thread_safe_iostream.cpp \
	   sources/mathematics/perlin_noise_2D.cpp \
	   sources/mathematics/random_2D_coordinate_generator.cpp \
	   sources/network/client.cpp \
	   sources/network/message.cpp \
	   sources/network/net_utils.cpp \
	   sources/network/server.cpp \
	   sources/thread/persistent_worker.cpp \
	   sources/thread/thread.cpp \
	   sources/thread/worker_pool.cpp \
	   sources/time/chronometer.cpp

OBJS = $(patsubst $(SRC_PATH)%.cpp, $(OBJ_PATH)%.o, $(SRCS))

DOXYGEN   = doxygen
DOC_DIR   = docs
DOC_INDEX = $(DOC_DIR)/html/index.html

all: $(NAME) $(DOC_INDEX)

$(NAME): $(OBJS)
	@mkdir -p $(LIB_DIR)
	ar rcs $(NAME) $(OBJS)

$(DOC_INDEX):
	@$(MAKE) --no-print-directory docs

$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -rf $(LIB_DIR)

re: fclean all

docs:
	@if ! command -v $(DOXYGEN) >/dev/null 2>&1; then \
		echo "doxygen not found - documentation not generated"; \
		echo "  install with: sudo dnf install doxygen graphviz"; \
	elif command -v dot >/dev/null 2>&1; then \
		$(DOXYGEN) Doxyfile && \
		echo "Documentation generated in $(DOC_DIR)/html/index.html"; \
	else \
		echo "graphviz (dot) not found - generating documentation without graphs"; \
		( cat Doxyfile; echo "HAVE_DOT=NO" ) | $(DOXYGEN) - && \
		echo "Documentation generated in $(DOC_DIR)/html/index.html"; \
	fi

docs-clean:
	rm -rf $(DOC_DIR)

.PHONY: all clean fclean re docs docs-clean

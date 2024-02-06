NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread #-g -fsanitize=address -static-libsan 

SRC = main.c

includefolder = includes
INCLUDES = -I $(includefolder) -I $(LIBFT_DIR)
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
OBJ_DIR = obj
SRCS = $(addprefix src/, $(SRC))
OBJS = $(patsubst src/%, $(OBJ_DIR)/%, $(SRCS:%.c=%.o))

# Reset

Color_Off='\033[0m'       # Text Reset

IRed='\033[0;91m'         # Red
IGreen='\033[0;92m'       # Green
IYellow='\033[0;93m'      # Yellow
ICyan='\033[0;96m'        # Cyan

MSG1 = @echo ${IGreen}"Compiled Successfully ✔︎"${Color_Off}
MSG4 = @echo ${IGreen}"Libft Compiled Successfully ✔︎"${Color_Off}
MSG2 = @echo ${IYellow}"Cleaned Successfully ✔︎"${Color_Off}
MSG3 = @echo ${ICyan}"Cleaned ${NAME} Successfully ✔︎"${Color_Off}

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) $(INCLUDES) -L $(LIBFT_DIR) -lft $(SRCS) -o $(NAME)
	$(MSG1)

$(OBJ_DIR)/%.o: $(SRCS) $(LIBFT)
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

$(LIBFT):
	@echo hello
	@make -s -C $(LIBFT_DIR)
	$(MSG4)

clean:
	@/bin/rm -rf $(OBJ_DIR)
	@make -s -C $(LIBFT_DIR) clean
	$(MSG2)

fclean: clean
	@/bin/rm -rf $(NAME) ${OBJ_DIR}
	@make -s -C $(LIBFT_DIR) fclean
	$(MSG3)

re: fclean all
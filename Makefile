NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror $(INCLUDES) -g # -fsanitize=address -static-libsan 

SRC = main.c

includefolder = includes/
LIBFT_DIR = libft/
LIBFT = $(LIBFT_DIR)/libft.a
LINK_FLAGS = -L $(LIBFT_DIR) -lft -lreadline
INCLUDES = -I $(includefolder) -I $(LIBFT_DIR)

SRC_FOLDER = src/
OBJ_DIR = obj/

SRC_ROOT_FILES = main.c input.c mini.c utils.c free.c
PARSER_LEXER_FILES = lexer.c parser.c parser_helpers.c

FILES = \
	$(SRC_ROOT_FILES) \
	$(addprefix pl/, $(PARSER_LEXER_FILES))

OBJS = $(addprefix $(OBJ_DIR), $(FILES:%.c=%.o))


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

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LINK_FLAGS)
	$(MSG1)

$(OBJS): $(OBJ_DIR)%.o: $(SRC_FOLDER)%.c
	@mkdir -p $(OBJ_DIR) $(dir $@)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(LIBFT):
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
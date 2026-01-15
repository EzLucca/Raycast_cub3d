CC       := cc
CFLAGS 	 := -Wall -Wextra -Werror -O3 -MMD -MP
GDBFLAGS := -g -O0 -Wall -Wextra -Werror

LDFLAGS  := -ldl -lglfw -lm -lz

NAME     	:= cub3d

SRC_DIR     := srcs
BUILD_DIR   := build
OBJ_DIR     := $(BUILD_DIR)/srcs
INCLUDES    := -I includes -I lib/MLX42/include -I lib/libft/include

LIBFT_DIR   := lib/libft
LIBFT_A     := $(LIBFT_DIR)/libft.a
MLX_REPO	:= https://github.com/codam-coding-college/MLX42.git
MLX_DIR     := lib/MLX42
MLX_A       := $(BUILD_DIR)/mlx42/libmlx42.a

SRC      := $(SRC_DIR)/main.c \
			$(SRC_DIR)/player.c \
			$(SRC_DIR)/parse.c \
			$(SRC_DIR)/raycast.c \
			$(SRC_DIR)/hook.c \
			$(SRC_DIR)/dda.c \
			$(SRC_DIR)/map3d.c \
			$(SRC_DIR)/map2d.c \
			$(SRC_DIR)/minimap_utils.c \
			$(SRC_DIR)/movements.c \
			$(SRC_DIR)/text.c \
			$(SRC_DIR)/colors.c \
			$(SRC_DIR)/get_line.c \
			$(SRC_DIR)/map_flood.c \
			$(SRC_DIR)/map_read.c \
			$(SRC_DIR)/map_validate.c \
			$(SRC_DIR)/parse_meta.c \
			$(SRC_DIR)/parse_scene.c \
			$(SRC_DIR)/textures.c \
			$(SRC_DIR)/utils.c \
			$(SRC_DIR)/utils2.c \
			$(SRC_DIR)/utils_color.c \
			$(SRC_DIR)/utils_map_read.c \
			$(SRC_DIR)/utils_parse_meta.c \

OBJ      := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS 	 := $(OBJ:.o=.d)

all: $(LIBFT_A) $(MLX_A) $(NAME)

$(LIBFT_A):
	@make --no-print-directory -C $(LIBFT_DIR)

$(MLX_A):
	@if [ ! -d "$(MLX_DIR)" ]; then \
		echo "Cloning MLX42 repository..."; \
		git clone $(MLX_REPO) $(MLX_DIR); \
		cmake -S $(MLX_DIR) -B $(BUILD_DIR)/mlx42; \
		make --no-print-directory -C $(BUILD_DIR)/mlx42 -j4; \
		fi;

$(NAME): $(LIBFT_A) $(MLX_A) $(OBJ)
	@$(CC) $(OBJ) $(LIBFT_A) $(MLX_A) $(LDFLAGS) -o $@
	@clear
	@echo "✅ Build $(NAME) successfully! 🎉"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $@
	-include $(DEPS)

$(OBJ_DIR):
	@mkdir -p $@

OBJ_GDB := $(OBJ:$(OBJ_DIR)/%.o=$(OBJ_DIR)/gdb_%.o)

$(OBJ_DIR)/gdb_%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(GDBFLAGS) $(INCLUDES) -c $< -o $@

gdb: $(LIBFT_A) $(MLX_A) $(OBJ_GDB)
	$(CC) $(OBJ_GDB) $(LIBFT_A) $(MLX_A) $(LDFLAGS) -o $(NAME)
	@echo "✅ Debug build ready: ./$(NAME)"

# gdb: $(LIBFT_A) $(MLX_A) $(OBJ)
# 	@$(CC) $(OBJ) $(LIBFT_A) $(MLX_A) $(GDBFLAGS) $(INCLUDES) $(LDFLAGS) -o $@
# 	@clear
# 	@echo "✅ Build $(NAME) successfully! 🎉"

clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR)/src
	@make --no-print-directory -C $(LIBFT_DIR) clean

fclean: clean
	rm -rf $(NAME) $(NAME_BONUS) $(BUILD_DIR) $(MLX_DIR)
	@make --no-print-directory fclean -C $(LIBFT_DIR)

re: fclean all

run: all
	./$(NAME) maps/cheese_maze.cub

.SECONDARY: $(OBJ)
.SECONDARY: $(OBJ_BONUS)
.PHONY: all clean fclean re bonus run

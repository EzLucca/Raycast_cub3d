#ifndef GAME_H
#define GAME_H

# include "../lib/libft/include/libft.h"
# include "../lib/MLX42/include/MLX42/MLX42.h"
# include <math.h>
# include <limits.h>
// -----------------------------------------------------------------------------
// Codam Coding College, Amsterdam @ 2022-2023 by W2Wizard.
// See README in the root project for more information.
// -----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define WIDTH 1024 
#define HEIGHT 1024 

#define mapX  8	//map width
#define mapY  8	//map height
#define mapS  64     //map cube size
#define TILE_SIZE 125   // Size of each tile (in pixels)
#define MAX_DOF 8 

#define RED		255,   0,   0, 255
#define GREEN	  0, 255,   0, 255
#define BLUE	  0,   0, 255, 255
#define BLACK	  0,   0,   0, 255
#define WHITE	255, 255, 255, 255

typedef struct s_player {
	float	x;
	float	y;
	int		xPos;
	int		yPos;
	float	dx;
	float	dy;
	float	da;
	float	Size;
	float	Speed;
} t_player;

typedef struct s_game {
	mlx_t		*mlx;
	t_player	*player;
	mlx_image_t	*image;
	int			*map;
} t_game;

int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a);
void draw_line(mlx_image_t *img, int x0, int y0, int x1, int y1, uint32_t color);
void drawray(void *param);
void drawMap2D(void *param);
void drawPlayer(void *param);
void ft_hook(void* param);
void update(void* param);

#endif

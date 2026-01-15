#include "cub3d.h"

void fire_laser(t_game *game)
{
    game->laser.active = true;
    game->laser.angle = game->player.da;
    game->laser.max_dist = 8.0; // 8 tiles max
    game->laser.duration = 5;   // lasts 5 frames
}

void minimap_draw_laser(t_game *g)
{
    if (!g->laser.active || g->draw_mode != DRAW_3D)
        return;

    int end_x, end_y;

    // cast a ray in the laser direction to stop at wall
    cast_ray(g->laser.angle, g);

    double dist = g->ray.dist;
    if (dist > g->laser.max_dist)
        dist = g->laser.max_dist;

    end_x = (int)(MM_CENTER + g->ray.ray_dir_x * dist * g->map2d.scale_x);
    end_y = (int)(MM_CENTER + g->ray.ray_dir_y * dist * g->map2d.scale_y);

    draw_line(g->img_map, end_x, end_y, BLUE);

    // decrease duration each frame
    g->laser.duration--;
    if (g->laser.duration <= 0)
        g->laser.active = false;
}

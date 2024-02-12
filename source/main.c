
#define RSGLDEF
#include <RSGL.h>
#include <stdio.h>

#include "vector.h"

u8 map[] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    1, 0, 0, 1, 0, 0, 1, 0, 0, 1,
    1, 0, 0, 1, 0, 0, 1, 0, 0, 1,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

#define MAP_WIDTH 10
#define MAP_HEIGHT 10

#define MAP_CORD(x, y) map[(x) + (y) * MAP_WIDTH]
#define MAP_CORDV(v) MAP_CORD((size_t)v.x, (size_t)v.y)

#define HMAP_CORD(x, y) hMap[(x) + (y) * MAP_WIDTH]
#define HMAP_CORDV(v) HMAP_CORD((size_t)v.x, (size_t)v.y)

RSGL_window* win;
vector2D player;
float playerAngle;

u16 range = 360;
u32 lightIntensity = 255;

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

void castRay(vector2D ray, float rayAngle, u32 x) {
    vector2D rayComp = VECTOR2D(cos(DEG2RAD * rayAngle) / 64, sin(DEG2RAD * rayAngle) / 64);
    vector2D next = vector_add(ray, rayComp);

    do {
        ray = vector_add(ray, rayComp);
    } while (MAP_CORDV(ray) == 0);

    float distance = vector_dist(ray, player) * cos(DEG2RAD * (rayAngle - playerAngle));
    
    RSGL_drawLineF(vector_mul(player, VECTOR2D(20, 20)), vector_mul(ray, VECTOR2D(20, 20)), 1, RSGL_RGBA(255, 255, 0, 25));
    
    u32 brightness = max(0, min(lightIntensity * dot(normalize(player), normalize(ray)), 255)) + 150;

    RSGL_drawRectF(RSGL_RECTF(20 * ((ray.x < 1) ? 0 : ray.x), 20 * ((ray.y < 1) ? 0 : ray.y), 5, 5), RSGL_RGB(brightness, 0, 0));
}

int main() {
    win = RSGL_createWindow("name", RSGL_RECT(0, 0, 20 * MAP_WIDTH, (20 * MAP_HEIGHT) - 15), RGFW_CENTER);

    player = RSGL_POINTF(2, 2); 
    playerAngle = 90;

    float playerVelocity = 0.25;

    bool running = true;

    while (running) {
        while (RSGL_window_checkEvent(win)) {
            if (win->event.type == RSGL_quit && RSGL_isPressedI(win, RGFW_Escape)) {
                running = 0;
                break;
            }

            /* simular to rayComp but for player movement */
            vector2D playerComp = VECTOR2D(cos(DEG2RAD * playerAngle) * playerVelocity,  sin(DEG2RAD * playerAngle) * playerVelocity);                
            vector2D playerCompLR = VECTOR2D(cos(DEG2RAD * (playerAngle - 180)) * playerVelocity,  sin(DEG2RAD * (playerAngle - 180)) * playerVelocity);

            if (win->event.type != RGFW_keyPressed)
                continue;
            
            if ((RGFW_isPressedI(win, RGFW_w)) && MAP_CORDV(vector_add(player, playerComp)) == 0)
                player = vector_add(player, playerComp);
            else if ((RGFW_isPressedI(win, RGFW_s)) && MAP_CORDV(vector_sub(player, playerComp)) == 0)
                player = vector_sub(player, playerComp);
            else if (RGFW_isPressedI(win, RGFW_d) && MAP_CORDV(vector_add(player, playerCompLR)) == 0)
                player = vector_add(player, playerCompLR);
            else if (RGFW_isPressedI(win, RGFW_a) && MAP_CORDV(vector_sub(player, playerCompLR)) == 0)
                player = vector_sub(player, playerCompLR);

            else if (RSGL_isPressedI(win, RGFW_Left))
                playerAngle -= 5;
            else if (RSGL_isPressedI(win, RGFW_Right))
                playerAngle += 5;
        }
        
        for (int y = 0; y < MAP_HEIGHT; y++)
            for (int x = 0; x < MAP_WIDTH; x++) {
                u32 w = 20, h = 5;

                if (x + 1 >= MAP_WIDTH || MAP_CORD(x + 1, y) == 0) {
                    w = 5;
                    h = 20;
                }

                RSGL_drawRect(RSGL_RECT(20 * x, 20 * y, w, h), RSGL_RGB(MAP_CORD(x, y) ? 50 : 0, 0, 0));
            }

        RSGL_drawRect(RSGL_RECT(20 * player.x, 20 * player.y, 2, 2), RSGL_RGB(240, 220, 0));
        
        float rayAngle = playerAngle - ((float)range / 2.0f);

        for(size_t i = 0; i < win->r.w; i++) {
            castRay(player, rayAngle, i);

            rayAngle += ((float)range / win->r.w);
        }
        
        RSGL_window_clear(win, RSGL_RGB(0, 0, 0));
    }

    RSGL_window_close(win);
}
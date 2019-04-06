#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <cab202_graphics.h>
#include <cab202_sprites.h>
#include <cab202_timers.h>
#include <time.h>
#define DELAY (9) /* small delay between game updates */
#define PLAYER_W (3)
#define PLAYER_H (5)
#define PLAT_W (10)
#define PLAT_H (2)
// Game state.


//n10229825 Levi Breakwell

bool game_over = false;    /* Set this to true when game is over */
bool update_screen = true; /* Set to false to prevent screen update. */
sprite_id player;
sprite_id chest;
char *playerright_img =
    /**/ " o_0 "
         /**/ ".( |."
         /**/ " / | ";
char *playerfall_img =
    /**/ " 0o0 "
         /**/ "-{ }-"
         /**/ " | | ";
char *playerleft_img =
    /**/ " 0_o "
         /**/ ".| )."
         /**/ " | L ";
char *playerspawn_img =
    /**/ " O-O "
         /**/ "[( )]"
         /**/ "_| |_";
char *plat_safe_img =
    /**/ "=========="
         /**/ "==========";
char *plat_danger_img =
    /**/ "xxxxxxxxxx"
         /**/ "xxxxxxxxxx";
char *chest1_img =
    /**/ "$*$"
         /**/ "[_]";
char *chest2_img =
    /**/ "*$*"
         /**/ "[_]";
char *msg_img =
    /**/ "Goodbye and thank-you for playing"
    /**/ "    Press any key to exit...     ";
int keyCode;
timer_id game_timer;
int milliseconds = 0;
int seconds = 0;
int minutes = 0;
sprite_id platformsafe;
sprite_id platformdanger;
int max_columns;
int max_rows;
sprite_id plats[1000];
void update_game_timer(void)
{
    milliseconds++;
    if (milliseconds >= 100)
    {
        seconds++;
        milliseconds = 0;
    }
    if (seconds >= 60)
    {
        minutes++;
        seconds = 0;
    }
}
int lives = 10;
int score = 0;
void draw_display()
{

    int studentNolength = 9;
    draw_line(1, 1, screen_width() - 1, 1, '-');
    draw_string(1, 3, "n10229825");
    draw_formatted((screen_width() / 4) - studentNolength, 3, "Lives left : %d", lives);
    draw_formatted((screen_width() / 2) - 11, 3, "Timer: %02i:%02i", minutes, seconds);
    draw_formatted(((screen_width() / 4) * 3) - 6, 3, "Score: %d", score);
    draw_line(1, 5, screen_width() - 1, 5, '-');
}
void setup_sprites()
{
    player = sprite_create(20, 7, 5, 3, playerright_img);
    sprite_draw(player);
    chest = sprite_create(10, screen_height() - 4, 3, 2, chest1_img);
    sprite_turn(chest, 90);
    sprite_turn_to(chest, +0.1, 0);
    show_screen();
    game_timer = create_timer(10);
}

sprite_id make_platform(int x, int y, char *img)
{
    double cord_x = x * max_columns;
    double cord_y = y * max_rows;
    sprite_id block = sprite_create(cord_x, cord_y, 7, 2, img);
    return block;
}
int counter = 0;
void create_platforms()
{
    int safeornot = 0;
    int thereornot = 0;

    // sprite_id new_platsafe;
    // sprite_id new_platdanger;
    for (int y = 2; y < max_rows; y++)
    {
        for (int x = 1; x < max_columns - 5; x++)
        {
            if (thereornot != 2)

            {
                if (safeornot != 3)
                {
                    plats[counter] = make_platform(x, y, plat_safe_img);
                    counter++;
                }
                else
                {
                    plats[counter] = make_platform(x, y, plat_danger_img);
                    counter++;
                }
            }

            safeornot = rand() % 4;
            thereornot = rand() % 3;
        }
    }
}

void end_game(keyCode)
{
    
    sprite_id text;
    if (keyCode == 'q' || lives < 0)
    {
        clear_screen();
        text = sprite_create((screen_width() - 48) / 2, (screen_height() - 2) / 2, 33, 2, msg_img);
        sprite_draw(text);
        show_screen();
        wait_char();
        game_over = true;
        update_screen = false;
    }
}
void chest_movement()
{
    sprite_step(chest);
    int chestx = round(sprite_x(chest));
    double chestdx = sprite_dx(chest);

    // Test to see if chest hit the left or right border.
    if (chestx == 0 || chestx == screen_width() - sprite_width(chest))
    {
        chestdx = -chestdx;
        sprite_turn_to(chest, chestdx, 0);
    }
}
bool sprites_collision(sprite_id sprite1, sprite_id sprite2)
{
    //declaring variables for collision
    double sprite1bot = sprite_y(sprite1) + sprite_height(sprite1);
    double sprite1top = sprite_y(sprite1);
    double sprite1left = sprite_x(sprite1);
    double sprite1right = sprite_x(sprite1) + sprite_width(sprite1) - 1;
    double sprite2right = sprite_x(sprite2) + sprite_width(sprite2) - 1;
    double sprite2left = sprite_x(sprite2);
    double sprite2top = sprite_y(sprite2);
    double sprite2bot = sprite_y(sprite2) + sprite_height(sprite1) - 2;
    //testing if player has collided with a safe platform
    if (sprite1bot < sprite2top)
    {
        return false;
    }
    if (sprite1left > sprite2right)
    {
        return false;
    }
    if (sprite1right < sprite2left)
    {
        return false;
    }
    if (sprite1top > sprite2bot)
    {
        return false;
    }
    else
    {
        return true;
    }
}
sprite_id sprite_collision_any(sprite_id p, sprite_id platformss[], int num)
{
    sprite_id result = NULL;

    for (int i = 0; i < num; i++)
    {
        if (sprites_collision(p, platformss[i]))
        {
            result = platformss[i];
            score++;
            break;
        }
    }
    return result;
}
void setup(void)
{
    max_columns = screen_width() / 10;
    max_rows = 8;
    setup_sprites();
    draw_display();
    create_platforms();
    for (int i = 0; i < counter; i++)
    {
        sprite_draw(plats[i]);
    }
}
void player_movement(keyCode)
{
    sprite_step(player);
    int playerx = round(sprite_x(player));
    int playery = round(sprite_y(player));
    //double playerdy = 0;
    if (sprite_collision_any(player, plats, counter) == false)
    {
        sprite_move(player, 0, +0.1);
        sprite_set_image(player, playerfall_img);
    }
    if (sprites_collision(player, chest))
    {
        lives = lives + 2;
        player = sprite_create(20, 7, PLAYER_H, PLAYER_W, playerright_img);
    }
    if (playery > screen_height() - PLAYER_H)
    {
        lives = lives - 1;
        player = sprite_create(20, 7, PLAYER_H, PLAYER_W, playerright_img);
    }
    if (playery <=7 ){
        sprite_move(player, 0, +1);
    }
    if (keyCode == 'a' && playerx > 1)
    {
        sprite_move(player, -2, 0);
        sprite_set_image(player, playerleft_img);
    }
    if (keyCode == 'd' && playerx < (screen_width() - 1 - sprite_width(player)))
    {
        sprite_move(player, +2, 0);
        sprite_set_image(player, playerright_img);
    }
    if (keyCode == 'w' && playerx > 6 && (sprite_collision_any(player, plats, counter)))
    {
        sprite_turn_to(player, 0, -3);
        sprite_step(player);
        sprite_turn_to(player, 0, -2);
        sprite_step(player);
        sprite_turn_to(player, 0, -1);
        sprite_step(player);
        sprite_turn_to(player, 0, 0);
        sprite_step(player);
        sprite_move(player, 0, +0.1);
        sprite_step(player);
    }
}

void process()
{
    clear_screen();
    keyCode = get_char();
    player_movement(keyCode);
    draw_display();
    update_game_timer();
    chest_movement();
    sprite_step(chest);
    end_game(keyCode);

    for (int i = 0; i < counter; i++)
    {
        sprite_draw(plats[i]);
    }
    show_screen();
}

int main(void)
{
    setup_screen();

    setup();
    show_screen();
    while (!game_over)
    {
        process();
        if (update_screen)
        {
            show_screen();
        }
        timer_pause(DELAY);
    }
    cleanup();
    return 0;
}
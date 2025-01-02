import pygame
import sys
import random

# Initialize Pygame
pygame.init()

# Screen dimensions and tile size
SCREEN_WIDTH = 640
SCREEN_HEIGHT = 480
TILE_SIZE = 32

# Colors
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
BROWN = (139, 69, 19)  # For dirt
GRAY = (169, 169, 169)  # For stone
GREEN = (34, 139, 34)  # For grass
RED = (255, 0, 0)  # For enemies
YELLOW = (255, 255, 0)  # For stairs

# Initialize screen
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
pygame.display.set_caption("Magic Tower")

# Clock for controlling frame rate
clock = pygame.time.Clock()

# Load assets
player_sprite = pygame.Surface((TILE_SIZE, TILE_SIZE))
player_sprite.fill(WHITE)

dirt_sprite = pygame.Surface((TILE_SIZE, TILE_SIZE))
dirt_sprite.fill(BROWN)

grass_sprite = pygame.Surface((TILE_SIZE, TILE_SIZE))
grass_sprite.fill(GREEN)

stone_sprite = pygame.Surface((TILE_SIZE, TILE_SIZE))
stone_sprite.fill(GRAY)

enemy_sprite = pygame.Surface((TILE_SIZE, TILE_SIZE))
enemy_sprite.fill(RED)

stairs_sprite = pygame.Surface((TILE_SIZE, TILE_SIZE))
stairs_sprite.fill(YELLOW)

# Map layout (0 = dirt, 1 = stone, 2 = enemy, 3 = stairs, 4 = grass)
map_layout = [
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
    [1, 4, 4, 4, 0, 0, 4, 4, 3, 1],
    [1, 4, 1, 1, 1, 1, 1, 4, 4, 1],
    [1, 0, 0, 0, 0, 2, 0, 0, 0, 1],
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
]

# Player position
player_pos = [1, 1]

# Draw map
def draw_map():
    for row_index, row in enumerate(map_layout):
        for col_index, tile in enumerate(row):
            x, y = col_index * TILE_SIZE, row_index * TILE_SIZE
            if tile == 0:
                screen.blit(dirt_sprite, (x, y))
            elif tile == 1:
                screen.blit(stone_sprite, (x, y))
            elif tile == 2:
                screen.blit(enemy_sprite, (x, y))
            elif tile == 3:
                screen.blit(stairs_sprite, (x, y))
            elif tile == 4:
                screen.blit(grass_sprite, (x, y))

# Move player
def move_player(dx, dy):
    new_x = player_pos[0] + dx
    new_y = player_pos[1] + dy
    if map_layout[new_y][new_x] != 1:  # Check for walls
        player_pos[0] = new_x
        player_pos[1] = new_y

# Main game loop
running = True
while running:
    screen.fill(BLACK)

    # Event handling
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # Player movement
    keys = pygame.key.get_pressed()
    if keys[pygame.K_UP]:
        move_player(0, -1)
    if keys[pygame.K_DOWN]:
        move_player(0, 1)
    if keys[pygame.K_LEFT]:
        move_player(-1, 0)
    if keys[pygame.K_RIGHT]:
        move_player(1, 0)

    # Draw map and player
    draw_map()
    screen.blit(player_sprite, (player_pos[0] * TILE_SIZE, player_pos[1] * TILE_SIZE))

    # Update display
    pygame.display.flip()

    # Control frame rate
    clock.tick(60)

pygame.quit()
sys.exit()

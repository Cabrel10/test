import pygame
import sys

# Initialisation de Pygame
pygame.init()

# Dimensions de la fenêtre de jeu
WIDTH, HEIGHT = 600, 600
GRID_SIZE = 6
BLOCK_SIZE = WIDTH // GRID_SIZE

# Couleurs
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
RED = (255, 0, 0)
GREEN = (0, 255, 0)
GOLD = (255, 215, 0)

# Configuration de la fenêtre de jeu
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption('Hamster Kombat')

# Classe pour les blocs
class Block(pygame.sprite.Sprite):
    def __init__(self, color, pos, size, is_horizontal):
        super().__init__()
        self.image = pygame.Surface(size)
        self.image.fill(color)
        self.rect = self.image.get_rect(topleft=pos)
        self.color = color
        self.is_horizontal = is_horizontal
        self.dragging = False

    def move(self, dx, dy, blocks):
        if self.is_horizontal:
            self.rect.x += dx
        else:
            self.rect.y += dy

        # Vérification des limites de la fenêtre
        if self.rect.left < 0:
            self.rect.left = 0
        elif self.rect.right > WIDTH:
            self.rect.right = WIDTH

        if self.rect.top < 0:
            self.rect.top = 0
        elif self.rect.bottom > HEIGHT:
            self.rect.bottom = HEIGHT

        # Vérification de collision avec d'autres blocs
        for block in blocks:
            if block != self and self.rect.colliderect(block.rect):
                if self.is_horizontal:
                    if dx > 0:  # Se déplace vers la droite
                        self.rect.right = block.rect.left
                    else:  # Se déplace vers la gauche
                        self.rect.left = block.rect.right
                else:
                    if dy > 0:  # Se déplace vers le bas
                        self.rect.bottom = block.rect.top
                    else:  # Se déplace vers le haut
                        self.rect.top = block.rect.bottom

# Création des blocs et de la clé
blocks = pygame.sprite.Group()
blocks.add(Block(RED, (0 * BLOCK_SIZE, 3 * BLOCK_SIZE), (BLOCK_SIZE, 3 * BLOCK_SIZE), False))#R1
blocks.add(Block(RED, (1 * BLOCK_SIZE, 0 * BLOCK_SIZE), (BLOCK_SIZE, 2 * BLOCK_SIZE), False))#R2
blocks.add(Block(RED, (2 * BLOCK_SIZE, 3 * BLOCK_SIZE), (BLOCK_SIZE, 2 * BLOCK_SIZE), False))#R3
blocks.add(Block(RED, (3 * BLOCK_SIZE, 2 * BLOCK_SIZE), (BLOCK_SIZE, 2 * BLOCK_SIZE), False))#R4
blocks.add(Block(RED, (4 * BLOCK_SIZE, 0 * BLOCK_SIZE), (BLOCK_SIZE, 3 * BLOCK_SIZE), False))#R5
blocks.add(Block(RED, (5 * BLOCK_SIZE, 1 * BLOCK_SIZE), (BLOCK_SIZE, 2 * BLOCK_SIZE), False))#R6
blocks.add(Block(GREEN, (1 * BLOCK_SIZE, 5 * BLOCK_SIZE), (2 * BLOCK_SIZE, BLOCK_SIZE), True))#V1
blocks.add(Block(GREEN, (2 * BLOCK_SIZE, 1 * BLOCK_SIZE), (2 * BLOCK_SIZE, BLOCK_SIZE), True))#V2
blocks.add(Block(GREEN, (3 * BLOCK_SIZE, 4 * BLOCK_SIZE), (2 * BLOCK_SIZE, BLOCK_SIZE), True))#V3
blocks.add(Block(GREEN, (4 * BLOCK_SIZE, 3 * BLOCK_SIZE), (2 * BLOCK_SIZE, BLOCK_SIZE), True))#V4
blocks.add(Block(GREEN, (4 * BLOCK_SIZE, 5 * BLOCK_SIZE), (2 * BLOCK_SIZE, BLOCK_SIZE), True))#V5'''


key = Block(GOLD, (0, 2 * BLOCK_SIZE), (2 * BLOCK_SIZE, BLOCK_SIZE), True)
blocks.add(key)

# Boucle principale du jeu
running = True
selected_block = None
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.MOUSEBUTTONDOWN:
            for block in blocks:
                if block.rect.collidepoint(event.pos):
                    block.dragging = True
                    selected_block = block
                    mouse_x, mouse_y = event.pos
                    offset_x = block.rect.x - mouse_x
                    offset_y = block.rect.y - mouse_y
                    break
        elif event.type == pygame.MOUSEBUTTONUP:
            if selected_block:
                selected_block.dragging = False
                selected_block = None
        elif event.type == pygame.MOUSEMOTION:
            if selected_block and selected_block.dragging:
                mouse_x, mouse_y = event.pos
                dx = 0
                dy = 0

                if selected_block.is_horizontal:
                    dx = mouse_x + offset_x - selected_block.rect.x
                    selected_block.move(dx, 0, blocks)
                else:
                    dy = mouse_y + offset_y - selected_block.rect.y
                    selected_block.move(0, dy, blocks)

    # Vérification de la sortie
    if key.rect.right >= WIDTH:
        print("Vous avez gagné !")
        running = False

    # Dessiner tout
    screen.fill(WHITE)
    blocks.draw(screen)

    # Ajouter des bordures aux blocs
    for block in blocks:
        pygame.draw.rect(screen, BLACK, block.rect, 2)

    pygame.display.flip()

pygame.quit()
sys.exit()

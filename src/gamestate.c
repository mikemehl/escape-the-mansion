#include "gamestate.h"
#include "title.h"
#include <stddef.h>

#define GAMESTATES                                                             \
  X(INIT, NULL, NULL, NULL)                                                    \
  X(TITLE, NULL, title_draw, title_update)

#define X(a, b, c, d) [a] = {a, b, c, d},

static GameStateData gamestates[] = {GAMESTATES};

#undef X

static GameStateData *gamestate_current = NULL;

#define CALL_GAMESTATE_CB(cb)                                                  \
  do {                                                                         \
    if (gamestate_current != NULL && gamestate_current->cb != NULL) {          \
      gamestate_current->cb(NULL);                                             \
    }                                                                          \
  } while (0)

int gamestate_set(GameState state) {
  if (state < INIT || state > MAX_GAMESTATE) {
    return -1;
  }
  CALL_GAMESTATE_CB(exit);
  gamestate_current = &gamestates[state];
  CALL_GAMESTATE_CB(init);
  return 0;
}

GameState gamestate_get() { return gamestate_current->state; }

void gamestate_draw() { CALL_GAMESTATE_CB(draw); }

void gamestate_update() { CALL_GAMESTATE_CB(update); }

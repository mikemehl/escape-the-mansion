#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__

typedef enum GameState { INIT, TITLE, START, MAX_GAMESTATE = START } GameState;

typedef void (*GameStateCb)(void *);

typedef struct GameStateData {
  GameState state;
  GameStateCb init;
  GameStateCb draw;
  GameStateCb update;
  GameStateCb exit;
} GameStateData;

int gamestate_set(GameState state);
GameState gamestate_get();
void gamestate_draw();
void gamestate_update();

#endif

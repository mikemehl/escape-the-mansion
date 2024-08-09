BUILD_DIR := justfile_directory() / "build"
BIN_OUT := BUILD_DIR / "escape-the-mansion"

alias r := run
run: build
  {{BIN_OUT}}

alias b := build
build:
  mkdir -p {{BUILD_DIR}}
  cmake -S . -B {{BUILD_DIR}} 
  cmake --build {{BUILD_DIR}} 

alias c := clean
clean:
  -make -C {{BUILD_DIR}} clean

alias gc := git-clean
git-clean:
  git clean -fxd

alias rb := rebuild
rebuild: clean build

alias dbg := debug
debug:
  gdb {{BIN_OUT}}

format:
  fd . ./src -e c -e h -x clang-format -i {}

rrespacker:
  xdg-open https://raylibtech.itch.io/rrespacker

code-count:
  tokei ./src

tiled:
  tiled ./assets/escape-the-mansion.tiled-session

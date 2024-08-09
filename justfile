BUILD_DIR := justfile_directory() / "build"
BIN_OUT := BUILD_DIR / "escape-the-mansion"

run: build
  {{BIN_OUT}}

build:
  mkdir -p {{BUILD_DIR}}
  cmake -S . -B {{BUILD_DIR}}
  cmake --build {{BUILD_DIR}}

clean:
  -make -C {{BUILD_DIR}} clean

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

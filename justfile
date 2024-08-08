BIN_OUT := justfile_directory() / "escape-the-mansion"

run: build
  {{BIN_OUT}}

build:
  bear -- make

clean:
  make clean

debug:
  gdb {{BIN_OUT}}

format:
  fd . ./src -e c -e h -x clang-format -i {}

rrespacker:
  xdg-open https://raylibtech.itch.io/rrespacker

code-count:
  tokei ./src ./Makefile

tiled:
  tiled ./assets/escape-the-mansion.tiled-session

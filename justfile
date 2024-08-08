BIN_OUT := justfile_directory() / "escape-the-mansion"

run: build
  {{BIN_OUT}}

build:
  bear -- make

clean:
  make clean

debug:
  gdb {{BIN_OUT}}

rrespacker:
  xdg-open https://raylibtech.itch.io/rrespacker

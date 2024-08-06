BIN_OUT := justfile_directory() / "escape-the-mansion"

run: build
  {{BIN_OUT}}

build:
  make

clean:
  make clean


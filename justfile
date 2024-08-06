BIN_OUT := justfile_directory() / "escape-the-mansion"

run: build
  {{BIN_OUT}}

build:
  bear -- make

clean:
  make clean


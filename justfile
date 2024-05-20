build:
  make build

clean:
  make clean

bear: clean
  bear -- make

run: build
  mgba-qt ./escape-the-mansion.gba

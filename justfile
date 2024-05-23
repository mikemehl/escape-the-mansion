build: 
  bear -- make build

clean:
  make clean

run: build
  mgba-qt ./escape-the-mansion.gba

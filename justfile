run: build
  #!/usr/bin/fish
  Xwayland 2>&1 1>/dev/null  &
  i3  2>&1 1>/dev/null &
  mgba-qt ./escape-the-mansion.gba

build: 
  -rm ./compile_commands.json
  bear -- make build

clean:
  make clean

start-xwayland:

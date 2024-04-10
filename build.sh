clang -Os -fno-builtin -Wall -Wextra -Wswitch-enum --target=wasm32 --no-standard-libraries -Wl,--no-entry -Wl,--export-all -Wl,--allow-undefined -o ./bin/triangle.wasm -I. ./examples/triangle.c

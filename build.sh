#!/bin/sh


LINUX="linux"
MACOS="macos"
WINDOWS="windows"
WEB="web"


help() {
    echo "Please specify a valid target platform:"
    echo "$0 $LINUX"
    echo "$0 $MACOS"
    echo "$0 $WINDOWS"
    echo "$0 $WEB"
    exit 1
}

help_web() {
    echo "Please source emsdk before trying to build for $WEB"
    echo ". \$HOME/emsdk/emsdk_env.sh"
    exit 1
}


if [ -z $1 ]; then
    help

elif [ $1 = $LINUX ]; then
    # https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux
    cc src/main.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -Wall -o game

elif [ $1 = $MACOS ]; then
    # https://github.com/raysan5/raylib/wiki/Working-on-macOS
    eval cc src/main.c -framework IOKit -framework Cocoa -framework OpenGL $(pkg-config --libs --cflags raylib) -Wall -o game

elif [ $1 = $WINDOWS ]; then
    # https://github.com/raysan5/raylib/wiki/Working-on-Windows
    gcc src/main.c -lraylib -lgdi32 -lwinmm -Wall -o game.exe

elif [ $1 = $WEB ]; then
    if [ -z "$EMSDK" ]; then
        help_web
    fi

    # Try make web directory if doesn't exist
    mkdir -p web

    # https://github.com/raysan5/raylib/wiki/Working-for-Web-(HTML5)v.sh
    # Also define WEB
    emcc -o web/index.html src/main.c -Os -Wall $HOME/raylib/src/web/libraylib.web.a \
        -D WEB \
        -I. -I$HOME/raylib/src -L. -L$HOME/raylib/src/web \
        -s USE_GLFW=3 \
        -s ASYNCIFY \
        --shell-file $HOME/raylib/src/minshell.html \
        --preload-file src/data \
        -s TOTAL_STACK=64MB \
        -s INITIAL_MEMORY=128MB \
        -DPLATFORM_WEB

    # Replace body tag with with a centred one
    sed -i 's|<body>|<body style="margin:0; height:100vh; display:flex; justify-content:center; align-items:center; background-color:black;">|' web/index.html

else
    help
fi

# Exit the script if the last command was unsuccessful
if [ $? -ne 0 ]; then
    echo "Failed to build"
    exit 1
fi

# Run compiled game
if [ $1 = $LINUX ]; then
    ./game
elif [ $1 = $MACOS ]; then
    ./game
elif [ $1 = $WINDOWS ]; then
    ./game.exe
elif [ $1 = $WEB ]; then
    emrun web/index.html
fi

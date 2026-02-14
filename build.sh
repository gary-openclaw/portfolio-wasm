#!/bin/bash
# Build script for portfolio-wasm
# Sets up Emscripten environment and builds

set -e

# Setup pyenv (for Python 3.11)
export PYENV_ROOT="$HOME/.pyenv"
export PATH="$PYENV_ROOT/bin:$PATH"
eval "$(pyenv init -)"

# Setup Emscripten
EMSDK_DIR="$HOME/workspace/emsdk"
if [ ! -d "$EMSDK_DIR" ]; then
    echo "Error: emsdk not found at $EMSDK_DIR"
    exit 1
fi

source "$EMSDK_DIR/emsdk_env.sh"

# Build
make "$@"

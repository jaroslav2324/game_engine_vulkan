#/bin/bash

glslc ./fragment/shader.frag -o ./build/frag.spv
glslc ./vertex/shader.vert -o ./build/vert.spv
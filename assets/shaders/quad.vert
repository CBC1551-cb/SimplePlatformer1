#version 430 core

void main(){
    // Generating Veritces on the GPU
    // mostly because we have a 2D engine

    // OpenGL Coordinates
    // -1/1-----------------1/1
    //   |                   |
    //   |      screen       |
    //   |                   |
    // -1/-1---------------1/-1
    vec2 vertices[6] = {
        //top left
        vec2(-0.5, 0.5),
        //bottom left
        vec2(-0.5, -0.5),
        //top right
        vec2(0.5, 0.5),
        //top right
        vec2(0.5, 0.5),
        //bottom left
        vec2(-0.5, -0.5),
        //bottom right
        vec2(0.5, -0.5)
    };

    gl_Position = vec4(vertices[gl_VertexID], 1.0, 1.0);
}
# Standalone Shadertoy

This is a small utility that attempts to replicate the rendering functionality
available at http://shadertoy.com/ into a standalone utility.

## Limitations

* Sound functionality is unavailable
* Texture support is limited to two hardcoded textures right now
* iDate and iSampleRate is unavailable

## Usage

Start the shadertoy with the glsl program as commandline argument

You can download shaders from shadertoy.com by using the "getshader"
script, which takes either the ID or the URL of the target shader
as commandline argument. It will save the shader code under the
given name of the shader into the current working directory,
as well as the full json object as a temporary file /tmp/current-shader.json.

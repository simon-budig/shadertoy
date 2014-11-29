# Standalone Shadertoy

This is a small utility that attempts to replicate the rendering functionality
available at http://shadertoy.com/ into a standalone utility.

## Limitations

* Sound functionality is unavailable
* iDate and iSampleRate is unavailable

## Usage

### Running shaders

You can start shadertoy with the shader file as argument:
```
./shadertoy <shader>
```

Also, you can specify images, which will be used as textures. For example:
```
./shadertoy --texture 0:texture0.png --texture 1:texture1.png <shader>
```
The textures will be linked inside the shader (iChannel0, iChannel1...).
Additionally you can specify the letters "o" to specify that the texture
is shown once (i.e. not repeated) as well as "n" for nearest-neighbor
interpolation:
```
./shadertoy --texture 0no:nyancat.png nyan.glsl
```


### Getting shaders

You can download shaders from shadertoy.com by using the "getshader.py"
script, which takes either the ID or the URL of the target shader
as commandline argument. It will save the shader code under the
given name of the shader into the current working directory,
as well as the full json object as a temporary file /tmp/current-shader.json.
```
./getshader.py MdX3zr
```

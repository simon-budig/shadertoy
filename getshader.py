#!/usr/bin/env python3

import sys, json, urllib.request, urllib.parse

shaderdecls = """
uniform vec3      iResolution;           // viewport resolution (in pixels)
uniform float     iGlobalTime;           // shader playback time (in seconds)
uniform float     iChannelTime[4];       // channel playback time (in seconds)
uniform vec3      iChannelResolution[4]; // channel resolution (in pixels)
uniform vec4      iMouse;                // mouse pixel coords. xy: current (if MLB down), zw: click
// uniform samplerXX iChannel0..3;             // input channel. XX = 2D/Cube
uniform vec4      iDate;                 // (year, month, day, time in secs)
uniform float     iSampleRate;           // sound sample rate (i.e., 44100)
\n"""

def get_shader (id):
   url = 'https://www.shadertoy.com/shadertoy'
   headers = { 'Referer' : 'https://www.shadertoy.com/' }
   values  = { 's' : json.dumps ({'shaders' : [id]}) }

   data = urllib.parse.urlencode (values).encode ('utf-8')
   req  = urllib.request.Request (url, data, headers)
   response = urllib.request.urlopen (req)
   shader_json = response.read ().decode ('utf-8')

   j = json.loads (shader_json)

   assert (len (j) == 1)

   for s in j:
      name = s["info"]["name"]
      for p in s["renderpass"]:
         code = (p["code"])
         return name, code
   

if __name__ == '__main__':
   if len (sys.argv) < 2:
      print ("Usage: %s <id>", file=sys.stderr)

   for id in sys.argv[1:]:
      attempt = 0
      id = id.split("/")[-1]
      name, code = get_shader (id)
      code = "".join (code.split ("\r"))
      f = None
      while f == None:
         try:
            if attempt == 0:
               fname = "%s.glsl" % name
            else:
               fname = "%s.glsl.%d" % (name, attempt)
            f = open (fname, "x")

         except FileExistsError:
            attempt += 1
            if attempt > 10:
               print ("clean out your files please...", file=sys.stderr)
               sys.exit (0)

      f.write (shaderdecls)
      f.write (code)
      f.close ()
      print ("wrote shader to \"%s\"" % fname, file=sys.stderr)


#!/usr/bin/env python3

# Standalone Shadertoy
# Copyright (C) 2014 Simon Budig <simon@budig.de>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

import sys, json, urllib.request, urllib.parse

shaderinfo = """\
// Shader downloaded from https://www.shadertoy.com/view/%(id)s
// written by shadertoy user %(username)s
//
// Name: %(name)s
// Description: %(description)s
"""

shaderdecls = """
uniform vec3      iResolution;           // viewport resolution (in pixels)
uniform float     iGlobalTime;           // shader playback time (in seconds)
uniform float     iChannelTime[4];       // channel playback time (in seconds)
uniform vec3      iChannelResolution[4]; // channel resolution (in pixels)
uniform vec4      iMouse;                // mouse pixel coords. xy: current (if MLB down), zw: click
uniform sampler2D iChannel0;             // input channel. XX = 2D/Cube
uniform sampler2D iChannel1;             // input channel. XX = 2D/Cube
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
   f = open ("/tmp/current-shader.json", "w")
   f.write (json.dumps (j, indent=2))
   f.close ()

   assert (len (j) == 1)

   for s in j:
      name = s["info"]["name"]
      desc = s["info"]["description"]
      desc = "".join (desc.split("\r"))
      desc = "\n//    ".join (desc.split("\n"))
      s["info"]["description"] = desc
      for p in s["renderpass"]:
         code = (p["code"])
         return name, code, s["info"]
   

if __name__ == '__main__':
   if len (sys.argv) < 2:
      print ("Usage: %s <id>" % sys.argv[0], file=sys.stderr)

   for id in sys.argv[1:]:
      attempt = 0
      id = id.split("/")[-1]
      name, code, info = get_shader (id)
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

      f.write (shaderinfo % info)
      f.write (shaderdecls)
      f.write (code)
      f.close ()
      print ("wrote shader to \"%s\"" % fname, file=sys.stderr)


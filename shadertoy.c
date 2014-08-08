#include <stdio.h>
#include <sys/time.h>

#include <GL/glew.h>
#include <GL/glut.h>

static double mouse_x0 = 0;
static double mouse_y0 = 0;
static double mouse_x = 0;
static double mouse_y = 0;

GLint
compile_shader (const GLenum  shader_type,
                const GLchar *shader_source)
{
  GLuint shader = glCreateShader (shader_type);
  GLint status = GL_FALSE;
  GLint loglen;
  GLchar *error_message;

  glShaderSource (shader, 1, &shader_source, NULL);
  glCompileShader (shader);

  glGetShaderiv (shader, GL_COMPILE_STATUS, &status);
  if (status == GL_TRUE)
    return shader;

  glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &loglen);
  error_message = calloc (loglen, sizeof (GLchar));
  glGetShaderInfoLog (shader, loglen, NULL, error_message);
  fprintf (stderr, "shader failed to compile:\n   %s\n", error_message);
  free (error_message);

  return -1;
}


GLint
link_program (const GLchar *shader_source)
{
  GLint frag, program;
  GLint status = GL_FALSE;
  GLint loglen;
  GLchar *error_message;

  frag = compile_shader (GL_FRAGMENT_SHADER, shader_source);
  if (frag < 0)
    return -1;

  program = glCreateProgram ();

  glAttachShader (program, frag);
  glLinkProgram (program);
  glDeleteShader (frag);

  glGetProgramiv (program, GL_LINK_STATUS, &status);
  if (status == GL_TRUE)
    return program;

  glGetProgramiv (program, GL_INFO_LOG_LENGTH, &loglen);
  error_message = calloc (loglen, sizeof (GLchar));
  glGetProgramInfoLog (program, loglen, NULL, error_message);
  fprintf (stderr, "program failed to link:\n   %s\n", error_message);
  free (error_message);

  return -1;
}

GLint prog = 0;

void
init (char *fragmentshader)
{
  GLenum status;

  status = glewInit ();

  if (status != GLEW_OK)
    {
      fprintf (stderr, "glewInit error: %s\n", glewGetErrorString (status));
      exit (-1);
    }

  fprintf (stderr,
           "GL_VERSION   : %s\nGL_VENDOR    : %s\nGL_RENDERER  : %s\n"
           "GLEW_VERSION : %s\nGLSL VERSION : %s\n\n",
           glGetString (GL_VERSION), glGetString (GL_VENDOR),
           glGetString (GL_RENDERER), glewGetString (GLEW_VERSION),
           glGetString (GL_SHADING_LANGUAGE_VERSION));

  if (!GLEW_VERSION_2_1)
    {
      fprintf (stderr, "OpenGL 2.1 or better required for GLSL support.");
      exit (-1);
    }

  prog = link_program (fragmentshader);

  if (prog < 0)
    exit (-1);
}


void
mouse_press_handler (int button, int state, int x, int y)
{
  if (button != GLUT_LEFT_BUTTON)
    return;

  if (state == GLUT_DOWN)
    {
      mouse_x0 = mouse_x = x;
      mouse_y0 = mouse_y = y;
    }
  else
    {
      mouse_x0 = -1;
      mouse_y0 = -1;
    }
}


void
mouse_move_handler (int x, int y)
{
  mouse_x = x;
  mouse_y = y;
}


void
keyboard_handler (unsigned char key, int x, int y)
{
  switch (key)
    {
      case '\x1b':  /* Escape */
        exit (0);
        break;

      default:
        break;
    }
}


void
display (void)
{
  int width, height, ticks;
  GLint uindex;

  glUseProgram (prog);

  width  = glutGet (GLUT_WINDOW_WIDTH);
  height = glutGet (GLUT_WINDOW_HEIGHT);
  ticks  = glutGet (GLUT_ELAPSED_TIME);

  uindex = glGetUniformLocation (prog, "iGlobalTime");
  if (uindex >= 0)
    glUniform1f (uindex, ((float) ticks) / 1000.0);

  uindex = glGetUniformLocation (prog, "time");
  if (uindex >= 0)
    glUniform1f (uindex, ((float) ticks) / 1000.0);

  uindex = glGetUniformLocation (prog, "iResolution");
  if (uindex >= 0)
    glUniform3f (uindex, width, height, 1.0);

  uindex = glGetUniformLocation (prog, "resolution");
  if (uindex >= 0)
    glUniform2f (uindex, width, height);

  uindex = glGetUniformLocation (prog, "iMouse");
  if (uindex >= 0)
    glUniform4f (uindex,
                 mouse_x,  height - mouse_y,
                 mouse_x0, mouse_y0 < 0 ? -1 : height - mouse_y0);

  uindex = glGetUniformLocation (prog, "led_color");
  if (uindex >= 0)
    glUniform3f (uindex, 0.5, 0.3, 0.8);

  glClear (GL_COLOR_BUFFER_BIT);
  glRectf (-1.0, -1.0, 1.0, 1.0);

  glutSwapBuffers ();
}


void
redisplay (int value)
{
  glutPostRedisplay ();
  glutTimerFunc (value, redisplay, value);
}


char *
load_file (char *filename)
{
  FILE *f;
  int size;
  char *data;

  f = fopen (filename, "rb");
  fseek (f, 0, SEEK_END);
  size = ftell (f);
  fseek (f, 0, SEEK_SET);

  data = malloc (size + 1);
  if (fread (data, size, 1, f) < 1)
    {
      fprintf (stderr, "problem reading file %s\n", filename);
      free (data);
      return NULL;
    }
  fclose(f);

  data[size] = '\0';

  return data;
}


int
main (int   argc,
      char *argv[])
{
  char *frag_code = NULL;
  glutInit (&argc, argv);

  if (argc != 2)
    {
      fprintf (stderr, "Usage: %s <shaderfile>\n", argv[0]);
      exit (-1);
    }

  frag_code = load_file (argv[1]);
  if (!frag_code)
    exit (-1);

  glutInitWindowSize (800, 600);
  glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE);
  glutCreateWindow ("Shadertoy");

  init (frag_code);

  glutDisplayFunc  (display);
  glutMouseFunc    (mouse_press_handler);
  glutMotionFunc   (mouse_move_handler);
  glutKeyboardFunc (keyboard_handler);

  redisplay (1000/16);

  glutMainLoop ();

  return 0;
}


#ifndef MAP_H
#define MAP_H
#include <gl.h>
#include <QGLWidget>
#include <glu.h>
#include "utility.h"
#include <QMouseEvent>
#include <QEvent>
#include <QDebug>

#define MAP_STRING_SIZE 500
#define MAX_TEXTURE_LAYERS 2
#define VERSION 1
#define REVSION  0

enum { CREATE_MODE_NULL = 0, CREATE_MODE_START, CREATE_MODE_SIZE, CREATE_MODE_FINISH };
enum { DRAW_MODE_WIREFRAME = 0, DRAW_MODE_SOLID };
enum {OBJECTTYPE_WALL = 1,OBJECTTYPE_FLOOR, OBJECTTYPE_CEILING, ITEM_START_POSITION, ITEM_DM_POSITION, ITEM_ENTITY, ITEM,SOUND,LIGHT};

typedef struct map_version
{
    GLint version;
    GLint reversion;
}MAP_VERSION;

typedef struct map_header
{
    GLint max_objects;
    GLint max_light;
    GLint max_cameras;
    GLint max_entities;
    GLint max_items;
    GLint max_sound;
    GLboolean use_skybox;
    GLboolean use_fog;


}MAP_HEADER;

typedef struct
{
    char filename[MAP_STRING_SIZE];
    GLint texid;

}MAP_SYKBOX_SIDE;


typedef struct
{
    MAP_SYKBOX_SIDE front;
    MAP_SYKBOX_SIDE back;
    MAP_SYKBOX_SIDE top;
    MAP_SYKBOX_SIDE bottom;
    MAP_SYKBOX_SIDE left;
    MAP_SYKBOX_SIDE right;
}MAP_SKYBOX;

typedef struct
{
    GLint mode;
    GLfloat start;
    GLfloat end;
    GLfloat density;
    GLfloat rgba[4];
}MAP_FOG;

typedef struct
{
    GLdouble xyz[3];
    GLfloat angle[3];
    GLubyte select_rgb[3];
}MAP_STARTING_POSITION;

typedef struct
{
    char map_name[MAP_STRING_SIZE];
    GLint map_type;
    GLint map_exit_rules;

    MAP_STARTING_POSITION single_player;
    MAP_STARTING_POSITION deathmatch[2];

}MAP_DETAILS;


typedef struct
{
    GLdouble xyz[3];
    GLfloat rgba[4];
    GLfloat normals[3];
    GLfloat fog_depth;
    GLubyte select_rgba[3];

}MAP_VERTEX;


typedef struct
{
    GLfloat uv1[2];
    GLfloat uv2[2];
    GLfloat uv3[2];

}MAP_UV_COORDS;


typedef struct
{
    GLint point[3];
    MAP_UV_COORDS uv[MAX_TEXTURE_LAYERS];
}MAP_TRIANGLES;


typedef struct
{
    char filename[MAP_STRING_SIZE];
    GLint id;
    GLint style;
    GLint blend_src;
    GLint blend_dst;
}MAP_TEXTURE;

typedef struct
{
    char name[MAP_STRING_SIZE];
    GLdouble xyz[3];
    GLfloat angle[3];

    GLubyte select_rgb[3];

}MAP_CAMERA;

typedef struct
{
    char name[MAP_STRING_SIZE];
    GLint type;
    GLdouble xyz[3];
    GLfloat angle[3];
    GLfloat rgba[4];

    char texture_filename[MAP_STRING_SIZE];
    GLint texture;

    GLint max_inclusions;
    GLint * inclusions;


    GLubyte select_rgb[3];
}MAP_LIGHT;


typedef struct
{
    char name[MAP_STRING_SIZE];
    GLint type;
    GLint special;

    GLboolean is_colliadble;
    GLboolean is_visible;

    GLint max_triangles;
    GLint max_vertices;
    GLint max_textures;


    MAP_TRIANGLES *triangle;
    MAP_TEXTURE *textures;
    MAP_VERTEX * vertex;

    GLubyte select_rgb[3];

}MAP_OBJECT;

typedef struct
{
    GLint type;
    GLint respawn_wait;
    GLint respawn_time;
    GLdouble xyz[3];

    GLubyte select_rgb[3];

}MAP_ITEM;

typedef struct
{
    GLint type;
    GLdouble xyz[3];
    GLfloat angle[3];

    GLint health;
    GLint strength;
    GLint armour;

    //anyother thing that an entity should have

    GLubyte select_rgb[3];
}MAP_ENTITY;


typedef struct
{
    char filename[MAP_STRING_SIZE];
    GLint id;
    GLdouble xyz[3];
    GLfloat angle[3];

    GLubyte select_rgb[3];
}MAP_SOUND;

typedef struct
{
    bool draw_floor;
    bool draw_celling;
    bool draw_wall;
    bool draw_light;
    bool draw_sound;
    bool draw_item;
    bool draw_entity;
}LAYERS;

extern LAYERS layers;

typedef struct
{
    long draw_mode;

}CONFIG;

extern CONFIG config;


typedef struct
{
    long mouse_x;
    long mouse_y;

    double world_x;
    double world_y;
    double world_z;
}COORDS;



typedef struct
{
    long mode;
    long type;

    COORDS start;
    COORDS finish;


}CREATION_COORDS;





//   ______globals external ____
extern CREATION_COORDS creation_coords;




class Map : public QGLWidget
{
    Q_OBJECT
public:
    MAP_VERSION version;
    MAP_HEADER header;
    MAP_SKYBOX skybox;
    MAP_FOG fog;
    MAP_DETAILS details;

    //might use std vectors for this dynamic array
    MAP_OBJECT *object;
    MAP_ENTITY *entity;
    MAP_CAMERA *camera;
    MAP_LIGHT *light;
    MAP_SOUND *sound;
    MAP_ITEM *item;
    char create_text[500];



    //interface to the rest
    void render();
    bool ColorExists(GLubyte r, GLubyte g, GLubyte b);
    long GenerateColor();
    COORDS compute_mouse_coords(long xpos, long ypos);
    long GenerateVertexColor(long obj);

    bool VertexColorExists(long obj, GLubyte r, GLubyte g, GLubyte b);

    void InsertObject(char *name, GLint type, GLint special=0, GLboolean is_collidable=true, GLboolean is_visible=true);

    bool InsertVertex(long obj, GLdouble x, GLdouble y, GLdouble z,
                      GLfloat r=1.0, GLfloat g=1.0, GLfloat b=1.0,
                      GLfloat a=1.0, GLfloat nx=0.0, GLfloat ny=0.0,
                      GLfloat nz=0.0, GLfloat fogdepth=0.0);

    bool InsertTriangle(long obj, GLint p1, GLint p2, GLint p3,GLfloat u1, GLfloat v1, GLfloat u2, GLfloat v2, GLfloat u3, GLfloat v3);
    bool insertLight(char * name, char * filename, GLdouble x, GLdouble y,GLdouble z, GLfloat xa, GLfloat ya, GLfloat za,
                     GLfloat r, GLfloat g , GLfloat b);
    bool insertSound(char * filename, GLdouble x, GLdouble y, GLdouble z, GLfloat xa, GLfloat ya, GLfloat za);
    bool insertItem(GLdouble x, GLdouble y, GLdouble z, GLint type, GLint respawn_wait, GLint respawn_time);
    bool insertCamera(char * name, GLdouble x, GLdouble y, GLdouble z, GLfloat xa, GLfloat ya, GLfloat za);
    bool insertEntity();
    // ____draw calls_____
    void drawSolid();
    void drawWirefreame();
    void drawItem();
    void drawEntity();
    void drawSound();
    void drawLight();
    void drawStartPosition();

    Map();
    ~Map();


protected:
    void initializeGL();
    void mouseMoveEvent(QMouseEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void resizeGL(int w,int h);
    void paintGL();
 private:
    //

signals:
    void putMessage(char *mess);



};

#endif // MAP_H

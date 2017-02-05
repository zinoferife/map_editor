#include "map.h"

void Map::render(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glPushMatrix();
    glTranslatef(0.0f,0.0f,0.0f);

    if(config.draw_mode == DRAW_MODE_SOLID) drawSolid();
    if(config.draw_mode == DRAW_MODE_WIREFRAME) drawWirefreame();

    drawStartPosition();
    if (layers.draw_entity) drawEntity();
    if (layers.draw_item) drawItem();
    if (layers.draw_sound) drawSound();
    if (layers.draw_light) drawLight();


    if (creation_coords.type == OBJECTTYPE_WALL) {
    glBegin (GL_LINES);
    glVertex2d (creation_coords.start.world_x, creation_coords.start.world_z);
    glVertex2d (creation_coords.finish.world_x, creation_coords.finish.world_z);
    glEnd();
    }

    else if (creation_coords.type == OBJECTTYPE_FLOOR || creation_coords.type == OBJECTTYPE_CEILING)
    {
    glBegin (GL_LINE_LOOP);
    glVertex2d (creation_coords.start.world_x, creation_coords.start.world_z);
    glVertex2d (creation_coords.finish.world_x, creation_coords.start.world_z);
    glVertex2d (creation_coords.finish.world_x, creation_coords.finish.world_z);
    glVertex2d (creation_coords.start.world_x, creation_coords.finish.world_z);
    glEnd();

    }
    glPopMatrix();
}

bool Map::ColorExists(GLubyte r, GLubyte g, GLubyte b)
{
    if((r == 255 && g == 0 && b == 0) || (r  == 0 &&  b == 0 && g == 255) || (r == 0 && b == 255 && g == 0  )
            || (r == 255 && b == 255 && g == 255))
    {
        return (true);
    }

    if(header.max_cameras > 0)
    {
        for(long i = 0; i < header.max_cameras; i ++)
        {
            if(camera[i].select_rgb[0] == r && camera[i].select_rgb[1] == g && camera[2].select_rgb[2] == b)
            {
                return (true);
            }
        }
    }

    if(header.max_entities > 0)
    {
        for(long i = 0; i < header.max_entities; i ++)
        {
            if(entity[i].select_rgb[0] == r && entity[i].select_rgb[1] == g && entity[2].select_rgb[2] == b)
            {
                return (true);
            }
        }
    }


    if(header.max_items > 0)
    {
        for(long i = 0; i < header.max_items; i ++)
        {
            if(item[i].select_rgb[0] == r && item[i].select_rgb[1] == g && item[2].select_rgb[2] == b)
            {
                return (true);
            }
        }
    }

    if(header.max_objects > 0)
    {
        for(long i = 0; i < header.max_cameras; i ++)
        {
            if(object[i].select_rgb[0] == r && object[i].select_rgb[1] == g && object[2].select_rgb[2] == b)
            {
                return (true);
            }
        }
    }


    if(header.max_light > 0)
    {
        for(long i = 0; i < header.max_light; i ++)
        {
            if(light[i].select_rgb[0] == r && light[i].select_rgb[1] == g && light[2].select_rgb[2] == b)
            {
                return (true);
            }
        }
    }


    if(header.max_sound > 0)
    {
        for(long i = 0; i < header.max_sound; i ++)
        {
            if(sound[i].select_rgb[0] == r && sound[i].select_rgb[1] == g && sound[2].select_rgb[2] == b)
            {
                return (true);
            }
        }
    }


    return (false);
}

long Map::GenerateColor()
{

    GLubyte r, b,g;

    r  = rand() % 255;
    g = rand() % 255;
    b  = rand() % 255;

    while(ColorExists(r,g,b))
    {
        r  = rand() % 255;
        g = rand() % 255;
        b  = rand() % 255;
    }

    return (rgb(r,g,b));  // the RGB marco

}

COORDS Map::compute_mouse_coords(long xpos, long ypos)
{
    COORDS coords;
    QRect rect;

    float window_width;
    float window_height;
    float window_start_x;
    float window_start_y;

    coords.mouse_x = xpos;
    coords.mouse_y = ypos;
    rect = geometry();
    window_width = (float)(rect.right() - rect.left());
    window_height =  (float)(rect.bottom() - rect.top());
    window_start_x = (float) (coords.mouse_x - rect.left());
    window_start_y = coords.mouse_y;

    coords.world_x = (window_start_x / window_width) * 2.0 - 1.0;
    coords.world_z = -((window_start_y / window_height) * 2.0 - 1.0);

    return coords;
}

long Map::GenerateVertexColor(long obj)
{
    GLubyte r,g,b;

    r = rand() % 256;
    g = rand() % 256;
    b = rand() % 256;

    while(VertexColorExists(obj,r,g,b))
    {
        r = rand() % 256;
        g = rand() % 256;
        b = rand() % 256;
    }

    return(rgb(r,g,b));
}

bool Map::VertexColorExists(long obj, GLubyte r, GLubyte g, GLubyte b)
{
    if ((r == 255 && g == 0 && b == 0) ||(r == 0 && g == 255 && b == 0) || (r == 0 && g == 0 && b == 255) ||
    (r == 255 && g == 255 && b == 255))
        return (true);
    for (long i = 0; i < object[obj].max_vertices; i++) {
    if (object[obj].select_rgb[0] == r && object[obj].select_rgb[1] == g && object[obj].select_rgb[2] == b)
        return (true);

    }
    return (false);

}


//std vertext all ready handles this but i am writing mine
void Map::InsertObject(char *name, GLint type, GLint special, GLboolean is_collidable, GLboolean is_visible)
{
    MAP_OBJECT new_obj;
    long rgb = GenerateColor();
    if(name == NULL)
    {
        strcpy(new_obj.name,"UNKNOWN");
    }
    strcpy(new_obj.name,name);
    new_obj.type = type;
    new_obj.special = special;
    new_obj.is_colliadble = is_collidable;
    new_obj.is_visible = is_visible;
    new_obj.max_textures = 0;
    new_obj.max_triangles = 0;
    new_obj.max_vertices = 0;

    new_obj.vertex = NULL;
    new_obj.triangle = NULL;
    new_obj.textures = NULL;

    new_obj.select_rgb[0] = getRvalue(rgb);
    new_obj.select_rgb[1] = getGvalue(rgb);
    new_obj.select_rgb[2] = getBvalue(rgb);

    if(header.max_objects == 0) object = new MAP_OBJECT[header.max_objects + 1];
    else
    {
        MAP_OBJECT *temp = new MAP_OBJECT[header.max_objects +1];
        for(long i = 0; i < header.max_objects; i++)
        {
            strcpy(temp[i].name,object[i].name);
            temp[i].type = object[i].type;
            temp[i].special = object[i].special;
            temp[i].is_colliadble = object[i].is_colliadble;
            temp[i].is_visible = object[i].is_visible;
            temp[i].max_textures = object[i].max_textures;
            temp[i].max_triangles = object[i].max_triangles;
            temp[i].max_vertices = object[i].max_vertices;
            temp[i].select_rgb[0] = object[i].select_rgb[0];
            temp[i].select_rgb[1] = object[i].select_rgb[1];
            temp[i].select_rgb[2] = object[i].select_rgb[2];


            if(temp[i].max_vertices > 0)
            {
                temp[i].vertex = new MAP_VERTEX[temp[i].max_vertices+1];
                for (long i2 = 0; i2 < temp[i].max_vertices; i2++)
                    temp[i].vertex[i2] = object[i].vertex[i2];

                delete [] object[i].vertex;
                object[i].vertex = NULL;
            }
            else temp[i].vertex = NULL;

            if (temp[i].max_triangles > 0) {
            temp[i].triangle = new MAP_TRIANGLES[temp[i].max_triangles+1];
            for (long i2 = 0; i2 < temp[i].max_triangles; i2++)
                temp[i].triangle[i2] = object[i].triangle[i2];

            delete [] object[i].triangle;
            object[i].triangle = NULL;
            }
            else temp[i].triangle = NULL;


            if(temp[i].max_textures > 0)
            {
                temp[i].textures = new MAP_TEXTURE[temp[i].max_textures+1];
                for (long i2 = 0; i2 < temp[i].max_textures; i2++)
                    temp[i].textures[i2] = object[i].textures[i2];

                delete [] object[i].textures;
                object[i].textures = NULL;
            }
            else temp[i].textures = NULL;
        }

        delete [] object;
        object = NULL;


        object = new MAP_OBJECT[header.max_objects+2];
        for(long i = 0; i < header.max_objects; i++)
        {
            strcpy(object[i].name,temp[i].name);
            object[i].type = temp[i].type;
            object[i].special = temp[i].special;
            object[i].is_colliadble = temp[i].is_colliadble;
            object[i].is_visible = temp[i].is_visible;
            object[i].max_textures = temp[i].max_textures;
            object[i].max_triangles = temp[i].max_triangles;
            object[i].max_vertices = temp[i].max_vertices;
            object[i].select_rgb[0] = temp[i].select_rgb[0];
            object[i].select_rgb[1] = temp[i].select_rgb[1];
            object[i].select_rgb[2] = temp[i].select_rgb[2];


            if(object[i].max_vertices > 0)
            {
                object[i].vertex = new MAP_VERTEX[object[i].max_vertices+1];
                for (long i2 = 0; i2 < object[i].max_vertices; i2++)
                    object[i].vertex[i2] = temp[i].vertex[i2];

                delete [] temp[i].vertex;
                temp[i].vertex = NULL;
            }
            else object[i].vertex = NULL;

            if (object[i].max_triangles > 0) {
            object[i].triangle = new MAP_TRIANGLES[object[i].max_triangles+1];
            for (long i2 = 0; i2 < object[i].max_triangles; i2++)
                object[i].triangle[i2] = temp[i].triangle[i2];

            delete [] temp[i].triangle;
            temp[i].triangle = NULL;
            }
            else object[i].triangle = NULL;


            if(object[i].max_textures > 0)
            {
                object[i].textures = new MAP_TEXTURE[object[i].max_textures+1];
                for (long i2 = 0; i2 < temp[i].max_textures; i2++)
                    temp[i].textures[i2] = object[i].textures[i2];

                delete [] temp[i].textures;
                temp[i].textures = NULL;
            }
            else object[i].textures = NULL;


        }

        delete [] temp;
        temp= NULL;

    }

    object[header.max_objects] = new_obj;
    header.max_objects++;

}




bool Map::InsertVertex(long obj, GLdouble x, GLdouble y, GLdouble z, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat fogdepth)
{

    MAP_VERTEX new_vertex;
    long rgb = GenerateVertexColor(obj);

    if(obj > header.max_objects || obj < 0) return false;

    new_vertex.xyz[0] = x;
    new_vertex.xyz[1] = y;
    new_vertex.xyz[2]  = z;
    new_vertex.rgba[0] = r;
    new_vertex.rgba[1] = g;
    new_vertex.rgba[2] = b;
    new_vertex.rgba[3] = a;
    new_vertex.normals[0] = nx;
    new_vertex.normals[1] = ny;
    new_vertex.normals[3] = nz;
    new_vertex.fog_depth = fogdepth;
    new_vertex.select_rgba[0] = getRvalue(rgb);
    new_vertex.select_rgba[1] = getGvalue(rgb);
    new_vertex.select_rgba[2] = getBvalue(rgb);


    if(object[obj].max_vertices == 0) object[obj].vertex = new MAP_VERTEX[object[obj].max_vertices+1];
    else
    {
        MAP_VERTEX * temp = new  MAP_VERTEX[object[obj].max_vertices+1];
        for(long i = 0; i < object[obj].max_vertices; i++)
            temp[i] = object[obj].vertex[i];

        delete[] object[obj].vertex;
        object[obj].vertex = new MAP_VERTEX[object[obj].max_vertices+2];

        for(long i = 0; i < object[obj].max_vertices; i++)
            object[obj].vertex[i] = temp[i];
        delete[] temp;
        temp = NULL;
    }

   object[obj].vertex[object[obj].max_vertices] = new_vertex;
   object[obj].max_vertices++;

   return (true);
}

bool Map::InsertTriangle(long obj, GLint p1, GLint p2, GLint p3,GLfloat u1, GLfloat v1, GLfloat u2, GLfloat v2, GLfloat u3, GLfloat v3)
{
    MAP_TRIANGLES new_tri;
    if(obj < 0 || obj > header.max_objects) return false;
    new_tri.point[0] = p1;
    new_tri.point[1] = p2;
    new_tri.point[2] = p3;

    for(long i = 1; i < MAX_TEXTURE_LAYERS; i++)
    {
        new_tri.uv[i].uv1[0] = u1;
        new_tri.uv[i].uv1[1] = v1;

        new_tri.uv[i].uv2[0] = u2;
        new_tri.uv[i].uv2[1] = v2;

        new_tri.uv[i].uv3[0] = u3;
        new_tri.uv[i].uv3[1] = v3;

    }

    if(object[obj].max_triangles == 0)
        object[obj].triangle = new MAP_TRIANGLES[object[obj].max_triangles +1];
    else
    {
        MAP_TRIANGLES * temp = new MAP_TRIANGLES[object[obj].max_triangles+1];
        for(long i = 0; i < object[obj].max_triangles; i++)
        {
            temp[i] = object[obj].triangle[i];
        }
        delete[] object[obj].triangle;
        object[obj].triangle = new MAP_TRIANGLES[object[obj].max_triangles+2];
        for(long i = 0; i < object[obj].max_triangles; i++)
            object[obj].triangle[i] = temp[i];

        delete[] temp;
        temp = NULL;
    }

    object[obj].triangle[object[obj].max_triangles] = new_tri;
    object[obj].max_triangles++;

    return (true);

}

bool Map::insertLight(char *name, char *filename, GLdouble x, GLdouble y, GLdouble z, GLfloat xa, GLfloat ya, GLfloat za, GLfloat r, GLfloat g, GLfloat b)
{
    MAP_LIGHT new_light;
    long rgb = GenerateColor();
    GLint max  = header.max_light;

    strcpy(new_light.name, name);
    strcpy(new_light.texture_filename,filename);
    new_light.angle[0] = xa;
    new_light.angle[1] = ya;
    new_light.angle[2] = za;
    new_light.xyz[0] = x;
    new_light.xyz[1] = y;
    new_light.xyz[2] = z;
    new_light.inclusions = NULL;
    new_light.max_inclusions = 0;
    new_light.rgba[0] = r;
    new_light.rgba[1] = g;
    new_light.rgba[2] = b;
    new_light.texture = 0;
    new_light.type = 0;
    new_light.select_rgb[0] = getRvalue(rgb);
    new_light.select_rgb[1] = getGvalue(rgb);
    new_light.select_rgb[2] = getBvalue(rgb);

    if(max == 0) light = new MAP_LIGHT[max + 1];
    else
    {
        MAP_LIGHT * temp = new MAP_LIGHT[max +1];
        for(long i =0; i <max; i++)
        {
            temp[i].angle[0] = light[i].angle[0];
            temp[i].angle[1] = light[i].angle[1];
            temp[i].angle[2] = light[i].angle[2];
            temp[i].max_inclusions = light[i].max_inclusions;
            if(light[i].max_inclusions > 0)
            {
                temp[i].inclusions = new GLint[temp[i].max_inclusions+1];
                for(long j = 0; j < light->max_inclusions; j++)
                    temp[i].inclusions[j] = light[i].inclusions[j];

                delete [] light[i].inclusions;

            }else
                temp[i].inclusions = NULL;

            temp[i].xyz[0] = light[i].xyz[0];
            temp[i].xyz[1] = light[i].xyz[1];
            temp[i].xyz[2] = light[i].xyz[2];
            strcpy(temp[i].name,light[i].name);
            strcpy(temp[i].texture_filename,light[i].texture_filename);
            temp[i].type = light[i].type;
            temp[i].rgba[0] = light[i].rgba[0];
            temp[i].rgba[1] = light[i].rgba[1];
            temp[i].rgba[2] = light[i].rgba[2];
            temp[i].rgba[3] = light[i].rgba[3];
            temp[i].texture = light[i].texture;
            temp[i].select_rgb[0] = light[i].select_rgb[0];
            temp[i].select_rgb[1] = light[i].select_rgb[1];
            temp[i].select_rgb[2] = light[i].select_rgb[2];

        }
        delete [] light;
        light = new MAP_LIGHT[max +2];

        for(long i =0; i <max; i++)
        {
            light[i].angle[0] = temp[i].angle[0];
            light[i].angle[1] = temp[i].angle[1];
            light[i].angle[2] = temp[i].angle[2];
            light[i].max_inclusions = temp[i].max_inclusions;
            if(light[i].max_inclusions > 0)
            {
                light[i].inclusions = new GLint[light[i].max_inclusions+1];
                for(long j = 0; j < light[i].max_inclusions; j++)
                    light[i].inclusions[j] = temp[i].inclusions[j];

                delete [] temp[i].inclusions;

            }else
                light[i].inclusions = NULL;

            light[i].xyz[0] = temp[i].xyz[0];
            light[i].xyz[1] = temp[i].xyz[1];
            light[i].xyz[2] = temp[i].xyz[2];
            strcpy(light[i].name,temp[i].name);
            strcpy(light[i].texture_filename,temp[i].texture_filename);
            light[i].type = temp[i].type;
            light[i].rgba[0] = temp[i].rgba[0];
            light[i].rgba[1] = temp[i].rgba[1];
            light[i].rgba[2] = temp[i].rgba[2];
            light[i].rgba[3] = temp[i].rgba[3];
            light[i].texture = temp[i].texture;
            light[i].select_rgb[0] = temp[i].select_rgb[0];
            light[i].select_rgb[1] = temp[i].select_rgb[1];
            light[i].select_rgb[2] = temp[i].select_rgb[2];

        }
        delete [] temp;
        temp = NULL;
    }
    light[max] = new_light;
    header.max_light++;
    return true;
}


bool Map::insertSound(char *filename, GLdouble x, GLdouble y, GLdouble z, GLfloat xa, GLfloat ya, GLfloat za)
{
    MAP_SOUND new_sound;
    long rgb;

    rgb = GenerateColor();
    new_sound.select_rgb[0] = getRvalue(rgb);
    new_sound.select_rgb[1] = getGvalue(rgb);
    new_sound.select_rgb[2] = getBvalue(rgb);
    new_sound.angle[0] = xa;
    new_sound.angle[1] = ya;
    new_sound.angle[2] = za;
    new_sound.xyz[0] = x;
    new_sound.xyz[0] = y;
    new_sound.xyz[0] = z;
    strcpy(new_sound.filename, filename);
    new_sound.id = 0;

    if(header.max_sound == 0) sound = new MAP_SOUND[header.max_sound +1];
    else
    {
        MAP_SOUND * temp = new MAP_SOUND[header.max_sound +1];
        for(long i = 0; i < header.max_sound; i++)
        {
            temp[i] = sound[i];
        }
        delete[] sound;
        sound = new MAP_SOUND[header.max_sound+2];
        for(long i = 0; i < header.max_sound; i++)
        {
            sound[i] = temp[i];
        }

        delete [] temp;
        temp = NULL;
    }
    sound[header.max_sound] = new_sound;
    header.max_sound++;
    return true;
}

bool Map::insertItem(GLdouble x, GLdouble y, GLdouble z, GLint type, GLint respawn_wait, GLint respawn_time)
{
    MAP_ITEM new_item;
    long rgb = GenerateColor();
    GLint max = header.max_items;

    new_item.xyz[0] = x;
    new_item.xyz[1] = y;
    new_item.xyz[2] = z;
    new_item.type = type;
    new_item.respawn_time = respawn_time;
    new_item.respawn_wait = respawn_wait;
    new_item.select_rgb[0] = getRvalue(rgb);
    new_item.select_rgb[1] = getGvalue(rgb);
    new_item.select_rgb[2] = getBvalue(rgb);

    if(header.max_items == 0) item = new MAP_ITEM[max + 1];
    else
    {
        MAP_ITEM *temp = new MAP_ITEM[max +1];
        for(long i = 0; i < max; i++) temp[i] = item[i];

        delete []  item;
        item = new MAP_ITEM[max +2];
        for(long i = 0; i < max; i++) item[i] = temp[i];

        delete[] temp;
        temp = NULL;
    }
    item[max] = new_item;
    header.max_items++;
    return true;
}


void Map::drawStartPosition()
{
    glColor3f (0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2d (details.single_player.xyz[0], details.single_player.xyz[2]-0.01);
    glVertex2d (details.single_player.xyz[0]+0.01,details.single_player.xyz[2]);
    glVertex2d (details.single_player.xyz[0],details.single_player.xyz[2]+0.01);
    glVertex2d (details.single_player.xyz[0]-0.01,details.single_player.xyz[2]);
    glEnd();
    glColor3f (1.0f, 1.0f, 1.0f);

}

void Map::drawSolid()
{
    for(long i = 0; i < header.max_objects; i++)
    {
        glColor3ubv(object[i].select_rgb);
        if((layers.draw_floor && object[i].type == OBJECTTYPE_FLOOR)
                || (layers.draw_celling && object[i].type == OBJECTTYPE_CEILING))
        {
            glBegin (GL_TRIANGLES);
            for (long i2 = 0; i2 < object[i].max_triangles; i2++) {
            long vertex_0 = object[i].triangle[i2].point[0];
            long vertex_1 = object[i].triangle[i2].point[1];
            long vertex_2 = object[i].triangle[i2].point[2];

            glVertex2d (object[i].vertex[vertex_0].xyz[0],object[i].vertex[vertex_0].xyz[2]);
            glVertex2d (object[i].vertex[vertex_1].xyz[0],object[i].vertex[vertex_1].xyz[2]);
            glVertex2d (object[i].vertex[vertex_2].xyz[0],object[i].vertex[vertex_2].xyz[2]);
            }
            glEnd();
        }
        else if( (layers.draw_wall) && object[i].type == OBJECTTYPE_WALL)
        {
            for (long i2 = 0; i2 < object[i].max_triangles; i2++)
            {
            long vertex_0 = object[i].triangle[i2].point[0];
            long vertex_1 = object[i].triangle[i2].point[1];
            long vertex_2 = object[i].triangle[i2].point[2];

            glBegin (GL_LINE_LOOP);
            glVertex2d (object[i].vertex[vertex_0].xyz[0],object[i].vertex[vertex_0].xyz[2]);
            glVertex2d (object[i].vertex[vertex_1].xyz[0],object[i].vertex[vertex_1].xyz[2]);
            glVertex2d (object[i].vertex[vertex_2].xyz[0],object[i].vertex[vertex_2].xyz[2]);
            glEnd();


            }
        }
    }

}

void Map::drawWirefreame()
{
    if(header.max_objects > 0)
    {
        for(long i =0; i < header.max_objects; i++)
        {
            if((layers.draw_celling && object[i].type== OBJECTTYPE_CEILING)
                    || (layers.draw_floor && object[i].type == OBJECTTYPE_FLOOR)
                    || (layers.draw_wall && object[i].type == OBJECTTYPE_WALL))
            {
                glBegin(GL_LINE_LOOP);
                for(long j = 0; j < object[i].max_vertices; j++)
                {
                    glVertex2d(object[i].vertex[j].xyz[0],object[i].vertex[j].xyz[2]);
                }
                glEnd();
            }
        }
    }

}

void Map::drawItem()
{
    glColor3f (1.0f, 1.0f, 0.0f);
    glBegin (GL_QUADS);
    for (long i = 0; i < header.max_items; i++)
    {
        glVertex2d (item[i].xyz[0],item[i].xyz[2]-0.01);
        glVertex2d (item[i].xyz[0]+0.01,item[i].xyz[2]);
        glVertex2d (item[i].xyz[0],item[i].xyz[2]+0.01);
        glVertex2d (item[i].xyz[0]-0.01,item[i].xyz[2]);
    }
    glEnd();
    glColor3f (1.0f, 1.0f, 1.0f);

}

void Map::drawEntity()
{

    glColor3f (0.0f, 1.0f, 1.0f);
    glBegin (GL_QUADS);
    for (long i = 0; i < header.max_entities; i++) {
    glVertex2d (entity[i].xyz[0], entity[i].xyz[2]-0.01);
    glVertex2d (entity[i].xyz[0]+0.01,entity[i].xyz[2]);
    glVertex2d (entity[i].xyz[0], entity[i].xyz[2]+0.01);
    glVertex2d (entity[i].xyz[0]-0.01, entity[i].xyz[2]);
    }
    glEnd();
    glColor3f (1.0f, 1.0f, 1.0f);

}

void Map::drawSound()
{
    glColor3f (0.0f, 1.0f, 0.0f);
    glBegin (GL_QUADS);
    for (long i = 0; i <header.max_sound; i++) {
    glVertex2d (sound[i].xyz[0], sound[i].xyz[2]-0.01);
    glVertex2d (sound[i].xyz[0]+0.01,sound[i].xyz[2]);
    glVertex2d (sound[i].xyz[0], sound[i].xyz[2]+0.01);
    glVertex2d (sound[i].xyz[0]-0.01,sound[i].xyz[2]);
    }
    glEnd();
    glColor3f (1.0f, 1.0f, 1.0f);

}

void Map::drawLight()
{
    glColor3f (0.5f, 1.0f, 0.0f);
    glBegin (GL_QUADS);
    for (long i = 0; i <header.max_light; i++) {
    glVertex2d (light[i].xyz[0], light[i].xyz[2]-0.01);
    glVertex2d (light[i].xyz[0]+0.01,light[i].xyz[2]);
    glVertex2d (light[i].xyz[0], light[i].xyz[2]+0.01);
    glVertex2d (light[i].xyz[0]-0.01,light[i].xyz[2]);
    }
    glEnd();
    glColor3f (1.0f, 1.0f, 1.0f);

}

Map::Map()
{
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));
    version.version = VERSION;
    version.reversion = REVSION;
    memset (&header, 0, sizeof(header));
    memset (&details, 0, sizeof(details));
    memset (&skybox, 0, sizeof(skybox));
    memset (&fog, 0, sizeof(fog));
    object = NULL;
    entity = NULL;
    camera = NULL;
    light = NULL;
    sound = NULL;
    item = NULL;
}

Map::~Map()
{
    if(header.max_objects > 0)
    {

        for(long i = 0; i < header.max_objects; i++)
        {
            if(object[i].max_vertices > 0)
            {
                delete [] object[i].vertex;
                object[i].vertex = NULL;
                object[i].max_vertices = 0;
            }

            if(object[i].max_triangles > 0)
            {
                delete [] object[i].triangle;
                object[i].triangle = NULL;
                object[i].max_triangles = 0;
            }
            if(object[i].max_textures > 0)
            {
                delete [] object[i].textures;
                object[i].textures = NULL;
                object[i].max_textures = 0;
            }

        }
        delete[] object;
        object = NULL;
        header.max_objects = 0;

    }


    if (header.max_cameras > 0) {
    delete [] camera;
    camera = NULL;
    header.max_cameras = 0;
    }

    if (header.max_sound > 0) {
    delete [] sound;
    sound = NULL;
    header.max_sound = 0;
    }
    if (header.max_entities > 0) {
    delete [] entity;
    entity = NULL;
    header.max_entities = 0;
    }
    if (header.max_light > 0) {
    delete [] light;
    light = NULL;
    header.max_light = 0;
    }
    if (header.max_items > 0) {
    delete [] item;
    camera = NULL;
    header.max_items = 0;
    }
}

void Map::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    qglClearColor(Qt::black);
}

void Map::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
   // glOrtho(-200,200,200,-200,-2000,2000);
    glMatrixMode(GL_MODELVIEW);
}

void Map::paintGL()
{
    render();
}

void Map::mouseMoveEvent(QMouseEvent *event)
{
   if(creation_coords.type != CREATE_MODE_NULL)
   {
        creation_coords.finish = compute_mouse_coords(event->x(),event->y());
        sprintf(create_text,"MAP COORDS : Mx=%i My=%i, X=%0.4f Z=%0.4f",creation_coords.finish.mouse_x,
                creation_coords.finish.mouse_y,creation_coords.finish.world_x,creation_coords.finish.world_z);
   }else
       sprintf (create_text, "Map Editor, Mx=%i My=%i", event->x(), event->y());
    emit putMessage(create_text);
    QPoint pos = event->pos();
    qDebug() << event->x() << event->y();
}

void Map::mousePressEvent(QMouseEvent *event)
{
    creation_coords.mode = CREATE_MODE_START;
    creation_coords.start = compute_mouse_coords(event->x(),event->y());
    creation_coords.finish = creation_coords.start;
}

void Map::mouseReleaseEvent(QMouseEvent *event)
{
    if(creation_coords.mode != CREATE_MODE_NULL)
    {
        creation_coords.mode = CREATE_MODE_NULL;
        creation_coords.finish = compute_mouse_coords(event->x(),event->x());
        if(creation_coords.type == OBJECTTYPE_WALL)
        {
            InsertObject ("Wall", creation_coords.type);
            InsertVertex (header.max_objects-1, creation_coords.start.world_x, 1, creation_coords.start.world_z);
            InsertVertex (header.max_objects-1, creation_coords.finish.world_x, 1, creation_coords.finish.world_z);
            InsertVertex (header.max_objects-1, creation_coords.finish.world_x, 0, creation_coords.finish.world_z);
            InsertVertex (header.max_objects-1, creation_coords.start.world_x, 0, creation_coords.start.world_z);
            InsertTriangle (header.max_objects-1, 0, 1, 2, 0.0f,0.0f,1.0f,0.0f, 1.0f,1.0f);
            InsertTriangle (header.max_objects-1, 2, 3, 0, 1.0f,1.0f,0.0f,1.0f, 0.0f,0.0f);
            qDebug() << "drawing wall";
        }
        else if(creation_coords.type == OBJECTTYPE_FLOOR)
        {
            InsertObject ("Floor", creation_coords.type);
            InsertVertex (header.max_objects-1, creation_coords.start.world_x, 0, creation_coords.start.world_z);
            InsertVertex (header.max_objects-1, creation_coords.finish.world_x, 0, creation_coords.start.world_z);
            InsertVertex (header.max_objects-1, creation_coords.finish.world_x, 0, creation_coords.finish.world_z);
            InsertVertex (header.max_objects-1, creation_coords.start.world_x, 0, creation_coords.finish.world_z);
            InsertTriangle(header.max_objects-1, 0, 1, 2, 0.0f,0.0f, 1.0f,0.0f, 1.0f,1.0f);
            InsertTriangle(header.max_objects-1, 3, 1, 2, 1.0f,1.0f, 0.0f,1.0f, 0.0f,0.0f);

        }else if(creation_coords.type == OBJECTTYPE_CEILING)
        {
            InsertObject ("Celling", creation_coords.type);
            InsertVertex (header.max_objects-1, creation_coords.start.world_x, 0, creation_coords.start.world_z);
            InsertVertex (header.max_objects-1, creation_coords.finish.world_x, 0, creation_coords.start.world_z);
            InsertVertex (header.max_objects-1, creation_coords.finish.world_x, 0, creation_coords.finish.world_z);
            InsertVertex (header.max_objects-1, creation_coords.start.world_x, 0, creation_coords.finish.world_z);
            InsertTriangle(header.max_objects-1, 0, 1, 2, 0.0f,0.0f, 1.0f,0.0f, 1.0f,1.0f);
            InsertTriangle(header.max_objects-1, 3, 1, 2, 1.0f,1.0f, 0.0f,1.0f, 0.0f,0.0f);
        }
    }
    qDebug() << creation_coords.start.mouse_x << " "<< creation_coords.start.world_z ;
    qDebug() << creation_coords.finish.mouse_x << " "<<creation_coords.finish.world_x ;
    memset (&creation_coords.start, 0, sizeof(creation_coords.start));
    memset (&creation_coords.finish, 0, sizeof(creation_coords.finish));
    updateGL();
}

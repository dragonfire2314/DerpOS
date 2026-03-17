#include "software3dRenderer.h"
#include "vga.h"
#include "math.h"
#include "system.h"

#define HEIGHT 200
#define WIDTH 320

void setMesh();

//DATA STRUCTURES
struct vec3d
{
    short x, y, z;
};

struct triangle
{
    struct vec3d p[3];
};

//Remember to allocate meshes into the heap upon creation
struct mesh
{
    struct triangle tris[1024];
};

struct mat4x4
{
    float m[4][4];
};

struct mesh meshCube;
struct mat4x4 matProj;

void MultiplyMatixVector(struct vec3d *i, struct vec3d *o, struct mat4x4 *m)
{
    o->x = i->x * m->m[0][0] + i->y * m->m[1][0] + i->z * m->m[2][0] + m->m[3][0];
    o->y = i->x * m->m[0][1] + i->y * m->m[1][1] + i->z * m->m[2][1] + m->m[3][1];
    o->z = i->x * m->m[0][2] + i->y * m->m[1][2] + i->z * m->m[2][2] + m->m[3][2];
    float w = i->x * m->m[0][3] + i->y * m->m[1][3] + i->z * m->m[2][3] + m->m[3][3];

    if(w != 0)
    {
        o->x /= w; o->y /= w; o->z /= w;
    }
}

void setMesh()
{
    //PROJECTION MATRIX
    float Near = 0.1;
    float Far = 1000.0;
    float fov = 90.0;
    float AspectRatio = (float)HEIGHT / (float)WIDTH;
    float FovRad = 1.0 / tan(fov * 0.5 / 180.0 * PI); 

    matProj.m[0][0] = AspectRatio * FovRad;
    matProj.m[1][1] = FovRad;
    matProj.m[2][2] = Far / (Far - Near);
    matProj.m[3][2] = (-Far * Near) / (Far - Near);
    matProj.m[2][3] = 1.0;
    matProj.m[3][3] = 0.0;

    matProj.m[0][1] = 0.0;
    matProj.m[0][2] = 0.0;
    matProj.m[0][3] = 0.0;
    matProj.m[1][0] = 0.0;
    matProj.m[2][0] = 0.0;
    matProj.m[3][0] = 0.0;
    matProj.m[1][3] = 0.0;
    matProj.m[2][3] = 0.0;
    matProj.m[3][1] = 0.0;
    matProj.m[3][2] = 0.0;

    //SOUTH
    meshCube.tris[0].p[0].x = 0.0;
    meshCube.tris[0].p[0].y = 0.0;
    meshCube.tris[0].p[0].z = 0.0;

    meshCube.tris[0].p[1].x = 0.0;
    meshCube.tris[0].p[1].y = 1.0;
    meshCube.tris[0].p[1].z = 0.0;

    meshCube.tris[0].p[2].x = 1.0;
    meshCube.tris[0].p[2].y = 1.0;
    meshCube.tris[0].p[2].z = 0.0;

    meshCube.tris[1].p[0].x = 0.0;
    meshCube.tris[1].p[0].y = 0.0;
    meshCube.tris[1].p[0].z = 0.0;

    meshCube.tris[1].p[1].x = 1.0;
    meshCube.tris[1].p[1].y = 1.0;
    meshCube.tris[1].p[1].z = 0.0;

    meshCube.tris[1].p[2].x = 1.0;
    meshCube.tris[1].p[2].y = 0.0;
    meshCube.tris[1].p[2].z = 0.0;
    //EAST
    meshCube.tris[2].p[0].x = 1.0;
    meshCube.tris[2].p[0].y = 0.0;
    meshCube.tris[2].p[0].z = 0.0;

    meshCube.tris[2].p[1].x = 1.0;
    meshCube.tris[2].p[1].y = 1.0;
    meshCube.tris[2].p[1].z = 0.0;

    meshCube.tris[2].p[2].x = 1.0;
    meshCube.tris[2].p[2].y = 1.0;
    meshCube.tris[2].p[2].z = 1.0;

    meshCube.tris[3].p[0].x = 1.0;
    meshCube.tris[3].p[0].y = 0.0;
    meshCube.tris[3].p[0].z = 0.0;

    meshCube.tris[3].p[1].x = 1.0;
    meshCube.tris[3].p[1].y = 1.0;
    meshCube.tris[3].p[1].z = 1.0;

    meshCube.tris[3].p[2].x = 0.0;
    meshCube.tris[3].p[2].y = 1.0;
    meshCube.tris[3].p[2].z = 1.0;
    //NORTH
    meshCube.tris[4].p[0].x = 1.0;
    meshCube.tris[4].p[0].y = 0.0;
    meshCube.tris[4].p[0].z = 1.0;

    meshCube.tris[4].p[1].x = 1.0;
    meshCube.tris[4].p[1].y = 1.0;
    meshCube.tris[4].p[1].z = 1.0;

    meshCube.tris[4].p[2].x = 0.0;
    meshCube.tris[4].p[2].y = 1.0;
    meshCube.tris[4].p[2].z = 1.0;

    meshCube.tris[5].p[0].x = 1.0;
    meshCube.tris[5].p[0].y = 0.0;
    meshCube.tris[5].p[0].z = 1.0;

    meshCube.tris[5].p[1].x = 0.0;
    meshCube.tris[5].p[1].y = 1.0;
    meshCube.tris[5].p[1].z = 1.0;

    meshCube.tris[5].p[2].x = 0.0;
    meshCube.tris[5].p[2].y = 0.0;
    meshCube.tris[5].p[2].z = 1.0;
    //WEST
    meshCube.tris[6].p[0].x = 0.0;
    meshCube.tris[6].p[0].y = 0.0;
    meshCube.tris[6].p[0].z = 1.0;

    meshCube.tris[6].p[1].x = 0.0;
    meshCube.tris[6].p[1].y = 1.0;
    meshCube.tris[6].p[1].z = 1.0;

    meshCube.tris[6].p[2].x = 0.0;
    meshCube.tris[6].p[2].y = 1.0;
    meshCube.tris[6].p[2].z = 0.0;

    meshCube.tris[7].p[0].x = 0.0;
    meshCube.tris[7].p[0].y = 0.0;
    meshCube.tris[7].p[0].z = 1.0;

    meshCube.tris[7].p[1].x = 0.0;
    meshCube.tris[7].p[1].y = 1.0;
    meshCube.tris[7].p[1].z = 0.0;

    meshCube.tris[7].p[2].x = 0.0;
    meshCube.tris[7].p[2].y = 0.0;
    meshCube.tris[7].p[2].z = 0.0;
    //TOP
    meshCube.tris[8].p[0].x = 0.0;
    meshCube.tris[8].p[0].y = 1.0;
    meshCube.tris[8].p[0].z = 0.0;

    meshCube.tris[8].p[1].x = 0.0;
    meshCube.tris[8].p[1].y = 1.0;
    meshCube.tris[8].p[1].z = 1.0;

    meshCube.tris[8].p[2].x = 1.0;
    meshCube.tris[8].p[2].y = 1.0;
    meshCube.tris[8].p[2].z = 1.0;

    meshCube.tris[9].p[0].x = 0.0;
    meshCube.tris[9].p[0].y = 1.0;
    meshCube.tris[9].p[0].z = 0.0;

    meshCube.tris[9].p[1].x = 1.0;
    meshCube.tris[9].p[1].y = 1.0;
    meshCube.tris[9].p[1].z = 1.0;

    meshCube.tris[9].p[2].x = 1.0;
    meshCube.tris[9].p[2].y = 1.0;
    meshCube.tris[9].p[2].z = 0.0;
    //BOTTOM
    meshCube.tris[10].p[0].x = 1.0;
    meshCube.tris[10].p[0].y = 0.0;
    meshCube.tris[10].p[0].z = 1.0;

    meshCube.tris[10].p[1].x = 0.0;
    meshCube.tris[10].p[1].y = 0.0;
    meshCube.tris[10].p[1].z = 1.0;

    meshCube.tris[10].p[2].x = 0.0;
    meshCube.tris[10].p[2].y = 0.0;
    meshCube.tris[10].p[2].z = 0.0;

    meshCube.tris[11].p[0].x = 1.0;
    meshCube.tris[11].p[0].y = 0.0;
    meshCube.tris[11].p[0].z = 1.0;

    meshCube.tris[11].p[1].x = 0.0;
    meshCube.tris[11].p[1].y = 0.0;
    meshCube.tris[11].p[1].z = 0.0;

    meshCube.tris[11].p[2].x = 1.0;
    meshCube.tris[11].p[2].y = 0.0;
    meshCube.tris[11].p[2].z = 0.0;
}

void software3dRenderer_test()
{
    setMesh();
    //vgaStart(2);

	while(1)
	{
		//vga_cls(0);

		//RENDER HERE
        //Sheach though tri
        for(int i = 0; i < 12; i++)
        {
            struct triangle triProjected;
            MultiplyMatixVector(&meshCube.tris[i].p[0], &triProjected.p[0], &matProj);
            MultiplyMatixVector(&meshCube.tris[i].p[1], &triProjected.p[1], &matProj);
            MultiplyMatixVector(&meshCube.tris[i].p[2], &triProjected.p[2], &matProj);

            triProjected.p[0].x += 1.0; triProjected.p[0].y += 1.0;
            triProjected.p[1].x += 1.0; triProjected.p[1].y += 1.0;
            triProjected.p[2].x += 1.0; triProjected.p[2].y += 1.0;

            triProjected.p[0].x *= 0.5 * (short)WIDTH;
            triProjected.p[0].y *= 0.5 * (short)HEIGHT;
            triProjected.p[1].x *= 0.5 * (short)WIDTH;
            triProjected.p[1].y *= 0.5 * (short)HEIGHT;
            triProjected.p[2].x *= 0.5 * (short)WIDTH;
            triProjected.p[2].y *= 0.5 * (short)HEIGHT;

            //vga_draw_triangle(triProjected.p[0].x, triProjected.p[0].y, 
            //    triProjected.p[1].x, triProjected.p[1].y,
            //    triProjected.p[2].x, triProjected.p[2].y, 2);

            putd(triProjected.p[0].x, 0, 0, 0x0F);
            putd(triProjected.p[0].x, 3, 0, 0x0F);
            putd(triProjected.p[1].x, 0, 1, 0x0F);
            putd(triProjected.p[1].x, 3, 1, 0x0F);
            putd(triProjected.p[2].x, 0, 2, 0x0F);
            putd(triProjected.p[2].x, 3, 2, 0x0F);

            putc('R', 4, 6, 0x0F);

            while(1){}
        } 

		//vga_update_buffer();
	}
}

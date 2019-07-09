import bpy
import numpy as np
from time import time

def area2dSurface(s) :
    #area of an arbitrary polygon, as in the notes
    sum  = 0
    for i in range(len(s)) :
        if i == (len(s) - 1) :
            sum += (s[i][0] - s[0][0]) * (s[0][1] + s[i][1])
        else :
            sum += (s[i][0] - s[i+1][0]) * (s[i][1] + s[i+1][1])
    area = sum/2
    return area




def surf(me) :
    print("ex7: Surface Area ")
    print(" ")
    print(" ")
    #considering regular polygons
    #use numpy

    for f in range(len(me.polygons)) :



        numVert = len(me.polygons[f].vertices)
        print("the face ", f, " has ", numVert, " vertices. ")

        # we know that  n = (sx,sy,sz)
        # n has the direction of the normal vector of the plane
        # and |n| is the area of the polygon

        sxVert = []
        syVert = []
        szVert = []
        for v in range(numVert) :
            x = me.vertices[v].co[0]
            y = me.vertices[v].co[1]
            z = me.vertices[v].co[2]
            #I take the projection of the face in each direction
            sxVert.append([y,z]) #projection on x = 0
            syVert.append([x,z]) #projection on y = 0
            szVert.append([x,y]) #projection on z = 0


        #creation of the vector n
        sx = area2dSurface(sxVert)
        sy = area2dSurface(syVert)
        sz = area2dSurface(szVert)


        n = np.array([sx,sy,sz])
        #the module of the vector should be the area of the surface
        surface = r(np.linalg.norm(n))



        print("-----------------------------------")
        print(" computed surface : ", surface )
        print(" area attribute value : ", r(me.polygons[f].area) )
        print("-----------------------------------")
        print(" ")




def r(a) :
    return int(a*1000 + 0.5)/1000.0


def processa_malla(me):
    print('Name of the mesh: %s' % (me.name))
    print(' V= %d' % (len(me.vertices)))
    print(' E= %d' % (len(me.edges)))
    print(' F= %d' % (len(me.polygons)))

    print('Vertex list:')
    for i in range(len(me.vertices)):
        coord=me.vertices[i].co
        print(i, ":", coord[0], coord[1], coord[2])

    print('Face list:')
    for i in range(len(me.polygons)):
        print(i, ":", me.polygons[i].vertices[:])
    print('Equiv________________')
    for poly in me.polygons:
        print("Polygon index: %d, length: %d" % (poly.index, poly.loop_total))
        for loop_index in poly.loop_indices:
#equivalent to
#       for loop_index in range(poly.loop_start, poly.loop_start + poly.loop_total):
            print("    Vertex: %d" % me.loops[loop_index].vertex_index)
#can also be obtained from poly.vertices[] directly:
#       for v in poly.vertices[:]:
#           print("    Vertex: %d" % v)
    print("Loops:")
    for i in me.loops:
        print(i.vertex_index)

    print("Edge list :")
    for i in range(len(me.edges)):
        print(i, ":", me.edges[i].vertices[:])
    print("\nEnd of Data for mesh "+me.name+"\n\n")


def main(sel = 7):
    # Retrieve the active object (the last one selected)
    ob = bpy.data.scenes['Scene'].objects.active

    # Check that it is indeed a mesh
    if not ob or ob.type != 'MESH':
        BPyMessages.Error_NoMeshActive()
        return

    # If we are in edit mode, return to object mode
    bpy.ops.object.mode_set(mode='OBJECT')

    # Retrieve the mesh data
    mesh = ob.data

    # Get current time
    t = time()

    # Function that does all the work
    if sel == 0 :
        processa_malla(mesh)
    elif sel == 7 :
        surf(mesh)
    else :
        surf(mesh)
    # Report performance...
    print("Script took %6.2f secs.\n\n"%(time()-t))

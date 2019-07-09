import bpy
import numpy as np
from time import time



def coco(me):
    print("ex4: Convex and concave edges ")

    convex = 0
    concave = 0
    manifoldEdges = []
    neighbors = computeEdgeNeighbour(me)
    for e in range(len(neighbors)) :
        if len(neighbors[e]) == 2 :
            manifoldEdges.append(neighbors[e])
    print("The mesh, has ", len(manifoldEdges), "2-manifold edges")

    for e in range(len(manifoldEdges)) :
        firstFace = manifoldEdges[e][0]
        #ordered vertex a------>b
        a = me.edges[e].vertices[0]
        b = me.edges[e].vertices[1]

        firstIsLeft = False
        #isLeft(firstFace):
        #check if in the face is left
        numVerFace = len(me.polygons[firstFace].vertices)
        for v in range(numVerFace) :
            if (v == numVerFace - 1) :
                if( a == me.polygons[firstFace].vertices[v] & b == me.polygons[firstFace].vertices[0] ) :
                    firstIsLeft = True
            else :
                if ( a == me.polygons[firstFace].vertices[v] & b == me.polygons[firstFace].vertices[v + 1] ) :
                    firstIsLeft = True
        if firstIsLeft :
            l = manifoldEdges[e][0]
            r = manifoldEdges[e][1]
        else :
            l = manifoldEdges[e][1]
            r = manifoldEdges[e][0]

        #define left and right vector
        lv = me.polygons[l].normal
        rv = me.polygons[r].normal
        #cross product
        clrv = np.cross(lv,rv)
        #obtain the vector of the edge a--->b
        edgev = me.vertices[b].co - me.vertices[a].co
        #check if concave or convex
        if np.dot(clrv, edgev) > 0 :
            convex = convex + 1
        elif np.dot(clrv, edgev) < 0 :
            concave = concave + 1
        elif np.dot(clrv, edgev) == 0 :
            print("male")

    print("-----------------------------------")
    print("convex : ", convex)
    print("concave : ", concave)


def computeEdgeNeighbour(me):
    neighbors = []
    print("compute neighbors of edges")
    for e in range(len(me.edges)):
        edgeNeigh = []
        a = me.edges[e].vertices[0]
        b = me.edges[e].vertices[1]
        for f in range(len(me.polygons)):
            hasA = False
            hasB = False
            for ver in range(len(me.polygons[f].vertices)):
                if ( me.polygons[f].vertices[ver] == a ):
                    hasA = True
                if( me.polygons[f].vertices[ver] == b):
                    hasB = True
            if (hasA & hasB ):
                edgeNeigh.append(f)
        neighbors.append(edgeNeigh)
    return neighbors






def manifold3(me):
    print("ex3: manifold and non-manifold (neighbors of a vertex)")
    neighbors = []
    bound = 0
    mani = 0
    nonMani = 0
    for v in range(len(me.vertices)):
        faces = []
        for f in range(len(me.polygons)):
            for ver in range(len(me.polygons[f].vertices)):
                if me.polygons[f].vertices[ver] == v:
                    faces.append(f)
        neighbors.append(faces)

    for e in range(len(me.edges)):
        count = 0
        a = me.edges[e].vertices[0]
        b = me.edges[e].vertices[1]

        for i in range(len(neighbors[a])):
            for j in range(len(neighbors[b])):
                if neighbors[a][i] == neighbors[b][j]:
                    count = count + 1
        if count == 1 :
            bound = bound + 1
        elif count == 2 :
            mani = mani + 1
        elif count >= 3 :
            nonMani = nonMani + 1
        else :
            print("some edge have some problem :/ ")

    print("-----------------------------------")
    print("The mesh : ", me.name, " has :")
    print(" boundary edges : ", bound)
    print(" manifold edges : ", mani)
    print(" non manifold edges : ", nonMani)



def manifold2(me):
    print("ex3: manifold and non-manifold(neighbors of an edge) ")
    neighbors = []
    bound = 0
    mani = 0
    nonMani = 0

    for e in range(len(me.edges)):
        edgeNeigh = []
        a = me.edges[e].vertices[0]
        b = me.edges[e].vertices[1]
        for f in range(len(me.polygons)):
            hasA = False
            hasB = False
            for ver in range(len(me.polygons[f].vertices)):
                if ( me.polygons[f].vertices[ver] == a ):
                    hasA = True
                if( me.polygons[f].vertices[ver] == b):
                    hasB = True

            if (hasA & hasB ):
                #count = count + 1
                edgeNeigh.append(f)


        neighbors.append(edgeNeigh)

    print("-----------------------------------")
    for e in range(len(me.edges)):
        count = len(neighbors[e])
        if count == 1 :
            bound = bound + 1
        elif count == 2 :
            mani = mani + 1
        elif count >= 3 :
            nonMani = nonMani + 1
        else :
            print("some edge have some problem :/ ")
    print("The mesh : ", me.name, " has :")
    print(" boundary edges : ", bound)
    print(" manifold edges : ", mani)
    print(" non manifold edges : ", nonMani)


def manifold1(me):
    print("ex3: manifold and non-manifold ")
    bound = 0
    mani = 0
    nonMani = 0
    for e in range(len(me.edges)):
        count = 0
        a = me.edges[e].vertices[0]
        b = me.edges[e].vertices[1]
        for f in range(len(me.polygons)):
            hasA = False
            hasB = False
            for ver in range(len(me.polygons[f].vertices)):
                if ( me.polygons[f].vertices[ver] == a ):
                    hasA = True
                if( me.polygons[f].vertices[ver] == b):
                    hasB = True
            if (hasA & hasB ):
                count = count + 1

        #print("the edge : ", e," (with vertices ", a, b, ") has ", count, " incident faces, so it is: ")
        if count == 1 :
            #print("boundary")
            bound = bound + 1
        elif count == 2 :
            #print("manifold")
            mani = mani + 1
        elif count >= 3 :
            #print("non manifold")
            nonMani = nonMani + 1
        else :
            print("some edge have some problem :/ ")

    print("-----------------------------------")
    print("The mesh : ", me.name, " has :")
    print(" boundary edges : ", bound)
    print(" manifold edges : ", mani)
    print(" non manifold edges : ", nonMani)


def valenza (me) :
    print("ex2: vertex valences")

    print('Name of the mesh: %s' % (me.name))
    print(' V= %d' % (len(me.vertices)))
    valenza = []
    for i in range(len(me.vertices)):
        val = 0
        for j in range(len(me.edges)):
            if me.edges[j].vertices[0] == i or me.edges[j].vertices[1] == i :
                val = val + 1
        valenza.append(val)
    for i in range(len(me.vertices)):
        print( "valence of ", i, " is: ", valenza[i])
    min_valenza = min(valenza)
    max_valenza = max(valenza)
    sum_valenza = sum(valenza)
    av_valenza = r(sum_valenza/(len(valenza)))
    print( "minimum valence: ", min_valenza)
    print( "maximum valence: ", max_valenza)
    print( "avarage valence: ", av_valenza)


def centroide (me) :
    print("ex1: centroid")

    print('function centroide su: %s' % (me.name))
    x_tot = 0
    y_tot = 0
    z_tot = 0
    for i in range(len(me.vertices)):
        coord=me.vertices[i].co
        x_tot = x_tot + coord[0]
        y_tot = y_tot + coord[1]
        z_tot = z_tot + coord[2]
    #round the results
    x_tot = r(x_tot)
    y_tot = r(y_tot)
    z_tot = r(z_tot)

    print(' x totale = %d' % x_tot)
    print(' y totale = %d' % y_tot)
    print(' z totale = %d' % z_tot)

    x_mean = r(x_tot/(len(me.vertices)))
    y_mean = r(y_tot/(len(me.vertices)))
    z_mean = r(z_tot/(len(me.vertices)))

    print("centroid coordinates", x_mean,y_mean,z_mean)


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


def r(a) :
    return int(a*1000 + 0.5)/1000.0


def main(sel = 4):
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
    elif sel == 1 :
        centroide(mesh)
    elif sel == 2 :
        valenza(mesh)
    elif sel == 3 | sel == 31 :
        manifold1(mesh)
    elif sel == 32 :
        manifold2(mesh)
    elif sel == 33 :
        manifold3(mesh)
    elif sel == 4 :
        coco(mesh)
    else :
        coco(mesh)
    # Report performance...
    print("Script took %6.2f secs.\n\n"%(time()-t))

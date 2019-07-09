import bpy
import numpy as np
from time import time



def simpleSub (me) :
    new_vertices = []
    new_faces = []

    facePoint = []
    edgePoint = []
    baricenterPoint = []

    edgeN = computeEdgeNeighbour(me)


    #face points
    numFaces = len(me.polygons)
    for f in range(len(me.polygons)):
        facePoint.append (computeFacePoint(me,f))
        new_vertices.append((facePoint[f][0],facePoint[f][1],facePoint[f][2]))



    edgeMidPoint = []
    edgeMidPoint = computeEdgeMidPoint(me)

    #edge points
    numEdges = len(me.edges)
    for e in range(len(me.edges)):
        new_vertices.append((edgeMidPoint[e][0], edgeMidPoint[e][1], edgeMidPoint[e][2]))

    #old vertices
    for v in range(len(me.vertices)) :
        new_vertices.append((me.vertices[v].co[0], me.vertices[v].co[1], me.vertices[v].co[2]))





    adjacentEdges = []
    adjacentEdges = edgesCloseToVertex(me)

    adjacentFaces = []
    adjacentFaces = facesCloseToVertex(me)

    #creating the face
    for f in range(len(me.polygons)):
        #face vertex
        f3 = f

        for v in range(len(me.polygons[f].vertices)):

            face = []

            f1 = 0
            f2 = 0
            f4 = 0

            a = me.polygons[f].vertices[v]


            #v of baricenter vertices
            f1 = numFaces + numEdges + a


            #first edge vertex
            if (v != (len(me.polygons[f].vertices) - 1)):
                b = me.polygons[f].vertices[v+1]
            else :
                b = me.polygons[f].vertices[0]

            for e in adjacentEdges[a]:
                edgeV1 = me.edges[e].vertices[0]
                edgeV2 = me.edges[e].vertices[1]
                if ((edgeV1 == a and edgeV2 == b) or (edgeV1 == b and edgeV2== a)) :
                    f2 = numFaces + e


            #second edge vertex
            if(v != 0):
                b = me.polygons[f].vertices[v-1]
            else:
                b = me.polygons[f].vertices[len(me.polygons[f].vertices)-1]
            for e in adjacentEdges[a]:
                edgeV1 = me.edges[e].vertices[0]
                edgeV2 = me.edges[e].vertices[1]
                if ((edgeV1 == a and edgeV2 == b) or (edgeV1 == b and edgeV2== a)) :
                    f4 = numFaces + e

            face = (f1,f2,f3,f4)
            new_faces.append(face)



    return new_vertices,new_faces







def computeEdgeMidPoint(me):
    sumx = 0
    sumy = 0
    sumz = 0
    edgeMidPoint = []
    for e in me.edges:
        a = me.vertices[e.vertices[0]].co
        b = me.vertices[e.vertices[1]].co
        sumx = (a[0] + b[0])
        sumy = (a[1] + b[1])
        sumz = (a[2] + b[2])
        edgeMidPoint.append((r(sumx/2), r(sumy/2), r(sumz/2)))
    return edgeMidPoint

def edgesCloseToVertex(me):
    adjacentEdges = []
    for v in range (len (me.vertices)):
        closeEdges = []
        for e in range(len(me.edges)):
             if (me.edges[e].vertices[0] == v or me.edges[e].vertices[1] == v ) :
                 closeEdges.append(e)
        adjacentEdges.append(closeEdges)
    return adjacentEdges

def facesCloseToVertex(me):
    adjacentFaces = []
    for v in range (len (me.vertices)):
        closeFaces = []
        for f in range(len(me.polygons)):
            for vi in range(len(me.polygons[f].vertices)):
                if (me.polygons[f].vertices[vi] == v):
                    closeFaces.append(f)
        adjacentFaces.append(closeFaces)
    return adjacentFaces


def computeFacePoint (me, f):
    i = 0
    sumx = 0
    sumy = 0
    sumz = 0
    for v in me.polygons[f].vertices:
        sumx = sumx + me.vertices[v].co[0]
        sumy = sumy + me.vertices[v].co[1]
        sumz = sumz + me.vertices[v].co[2]
        i = i + 1
    facePointCo = (r(sumx/i), r(sumy/i), r(sumz/i))
    return facePointCo

def computeEdgePoint (me, e, facePoint1, facePoint2):
    edgePoint = []
    i = 4
    sumx = 0
    sumy = 0
    sumz = 0
    verA = me.edges[e].vertices[0]
    verB = me.edges[e].vertices[1]
    A = me.vertices[verA].co
    B = me.vertices[verB].co
    C = facePoint1
    D = facePoint2
    sumx = A[0] + B[0] + C[0] + D[0]
    sumy = A[1] + B[1] + C[1] + D[1]
    sumz = A[2] + B[2] + C[2] + D[2]
    edgePoint = (r(sumx/i), r(sumy/i), r(sumz/i))
    return edgePoint




def computeEdgeNeighbour(me):
    neighbors = []
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


def r(a) :
    return int(a*1000 + 0.5)/1000.0


def main(sel = 1):
    coords = []
    faces = []
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
        coords, faces = simpleSub(mesh)
    else :
        coords, faces = simpleSub(mesh)
    # Report performance...

    if(sel != 0):
        me = bpy.data
        bpy.ops.object.delete()
        me = bpy.data.meshes.new("new_me")
        ob = bpy.data.objects.new("new_me",me)
        ob.location = bpy.context.scene.cursor_location
        bpy.context.scene.objects.link(ob)
        me.from_pydata(coords,[],faces)
        me.update(calc_edges = True)
        bpy.data.objects["new_me"].select = True
        bpy.context.scene.objects.active = bpy.data.objects["new_me"]
    print("Script took %6.2f secs.\n\n"%(time()-t))

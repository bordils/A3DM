import bpy
import numpy as np
from time import time






def manifoldShellVertices(me):
    nonManifoldVert = []
    finalSet = []
    returnedVec = []
    neighbors = computeEdgeNeighbour(me)
    for e in range(len(neighbors)) :
        if len(neighbors[e]) != 2 :
            nonManifoldVert.append(me.edges[e].vertices[0])
            nonManifoldVert.append(me.edges[e].vertices[1])
    if len(nonManifoldVert) != 0  :
            print("non manifold vertices >:( found "  )
    edgeList = []
    shells = 0

    #Create a list of all edges
    for ed in range(len(me.edges)):
        edgeList.append([me.edges[ed].vertices[0], me.edges[ed].vertices[1]])
    vertexSets = union_find(edgeList)
    shells = len(vertexSets)


    #Here we discard the sets that have soem manifold vertex
    if len(nonManifoldVert) != 0  :
        for set in range (len(vertexSets)) :
            discardSet = False
            for v in range(len(nonManifoldVert)) :
                if v in vertexSets[set] :
                    discardSet = True
            if discardSet :
                shells = shells - 1
            else :
                finalSet.append(vertexSets[set])
    else:
        finalSet = vertexSets

    print("-----------------------------------")
    print("number of valid shells found = ", shells)

    for i in range(len(finalSet)):
        for j in finalSet[i]:
            returnedVec.append(j)


    return returnedVec


def getValidPolygons(me,vert):
    validPolygons = []
    for f in me.polygons :
        isValid = False
        for v in f.vertices :
            if v in vert:
                isValid = True
        if isValid:
            validPolygons.append(f)
    return validPolygons


def union_find(lis):
    lis = map(set, lis)
    unions = []
    for item in lis:
        temp = []
        for s in unions:
            if not s.isdisjoint(item):
                item = s.union(item)
            else:
                temp.append(s)
        temp.append(item)
        unions = temp
    return unions


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


def main(sel = 8):
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
    if sel == 8:
        volume(mesh)
    else :
        volume(mesh)
    # Report performance...
    print("Script took %6.2f secs.\n\n"%(time()-t))

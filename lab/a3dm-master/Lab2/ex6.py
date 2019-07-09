import bpy
import numpy as np
from time import time


def  genus(me):
    print("ex6: Compute the number of Genus")
    print("*************************************")

    f = len(me.polygons)
    e = len(me.edges)
    v = len(me.vertices)
    r = 0
    s = shells(me)

    print("Euler-Poincaré forumla F + V = E + R + 2 (S - H)")

    h = s - (f + v - e)/2

    print("**************************************")
    print("number of genus : ", h)


def shells(me):
    #requires union_find function and neighbors

    #list of non-manifold edges
    nonManifoldVert = []
    #using the defined functions for the neighbour of an edge we compute the non manifold edge,
    #   and we extract all the vertices belonging to that edges,
    #   because if a shell has a vertex belonging to a non-manifold edge means that it also have the non-manifold edge
    neighbors = computeEdgeNeighbour(me)
    for e in range(len(neighbors)) :
        if len(neighbors[e]) != 2 :
            nonManifoldVert.append(me.edges[e].vertices[0])
            nonManifoldVert.append(me.edges[e].vertices[1])
    if len(nonManifoldVert) != 0  :
        #the set function should remove the duplicates, but it doesn't works :(
        #nonManifoldVert = list(set(nonManifoldVert))
        print("non manifold vertices >:( found "  )


    shells = 0
    edgeList = []

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

    print("-----------------------------------")
    print("number of shells found = ", shells)

    return shells


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


def main(sel = 6):
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
    if sel == 6 :
        genus(mesh)
    else :
        genus(mesh)
    # Report performance...
    print("Script took %6.2f secs.\n\n"%(time()-t))

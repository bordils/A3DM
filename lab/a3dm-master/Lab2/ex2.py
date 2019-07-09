import bpy
from time import time


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


def main(sel = 2):
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
    elif sel == 3 :
        manifold(mesh)
    else :
        valenza(mesh)
    # Report performance...
    print("Script took %6.2f secs.\n\n"%(time()-t))

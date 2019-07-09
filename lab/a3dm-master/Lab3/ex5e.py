import bpy
from time import time

#Function that gives the neighbors polygons of an edge from LAB2
def neighbors(me):
    n=len(me.edges)
    neighbors=[[] for i in range(n)]
    m=len(me.polygons)
    for edge in range(n):
        for poly in range(len(me.polygons)):
            vert=me.edges[edge].vertices
            if ((vert[0] in me.polygons[poly].vertices) and (vert[1] in me.polygons[poly].vertices)):
                neighbors[edge].append(poly)
    return(neighbors)


#Give list of vertices' coordinates
def coord(me):
    coords=[]
    for v in me.vertices:
        coord=v.co
        coords.append((coord[0],coord[1],coord[2]))
    return(coords)


#Add an element to a list if it's not already there. Return the index at which the element has been inserted
def add_check(val,l):
    if not(val in l):
        l.append(val)
    return l.index(val)


#Compute the coordinates of the barycenter of an array of vertices
def barycenter(vertices,coords):
    size=len(vertices)
    meanx=0
    meany=0
    meanz=0
    for v in vertices:
        meanx+=coords[v][0]
        meany+=coords[v][1]
        meanz+=coords[v][2]
    return((meanx/size,meany/size,meanz/size))


#Create list of edges shared by each vertex - the degree of vertex v is the length of VE[v]
def VE(me):
    VE=[[] for i in range(len(me.vertices))]
    for v in range(len(me.vertices)):
        for e in range(len(me.edges)):
            if (me.edges[e].vertices[0]==v or me.edges[e].vertices[1]==v):
                VE[v].append(e)
    return(VE)


#Create list of faces shared by each vertex
def VF(me):
    VF=[[] for i in range(len(me.vertices))]
    for v in range(len(me.vertices)):
        for f in range(len(me.polygons)):
            if v in me.polygons[f].vertices:
                VF[v].append(f)
    return(VF)


#-------------------------------------------------------
#------------ Simple subdivision function --------------
#-------------------------------------------------------

def Simple_subdivisison(C,F):

    me=bpy.data.meshes.new("Subdiv1")
    ob2=bpy.data.objects.new("Subdiv1",me)
    me.from_pydata(C,[],F)

    COORD=coord(me)  #New coordinates
    FACES=[] #New faces

    V_E=VE(me)
    E_F=neighbors(me)

    centroid_ID=[-1 for i in range(len(me.polygons))]
    edge_vertex_ID=[-1 for i in range(len(me.edges))]


    #Compute face vertices
    for face in range(len(me.polygons)):
        Vertices = me.polygons[face].vertices[:]

        centroid=barycenter(Vertices,COORD)  #Compute centroid and add it to new vertices
        centroid_ID[face]=add_check(centroid,COORD)


    #Compute edge vertices
    for edge in range(len(me.edges)):
        #Mean of the two vertices of the edge
        edge_vertex=barycenter([me.edges[edge].vertices[0],me.edges[edge].vertices[1]],COORD)
        edge_vertex_ID[edge]=add_check(edge_vertex,COORD)


    #Loop on all faces
    for face in range(len(me.polygons)):
        Vertices=me.polygons[face].vertices[:]

        for v in range(len(Vertices)):

            edges=V_E[Vertices[v]] #Edges shared by vertex v

            deg=len(edges)  #Degree of vertex v

            select=[0 for i in range(2)]  #Selection of two edge vertices that will form the face
            for e in edges:
                if (face in E_F[e]):
                    v1=me.edges[e].vertices[0]
                    v2=me.edges[e].vertices[1]
                    #We check the order of the edges (=the order of vertices in the edges)
                    if (v==len(Vertices)-1):
                        if (v1==Vertices[0] or v2==Vertices[0]):
                            select[0]=edge_vertex_ID[e]
                        else:
                            select[1]=edge_vertex_ID[e]
                    else:
                        if (v1==Vertices[v+1] or v2==Vertices[v+1]):
                            select[0]=edge_vertex_ID[e]
                        else:
                            select[1]=edge_vertex_ID[e]

            #We create a new face with the vertices in the right order
            new_face=(Vertices[v],select[0],centroid_ID[face],select[1])
            FACES.append(new_face)

    return(COORD,FACES)


#-------------------------------------------------------
#------------- Catmull-Clark Function ------------------
#-------------------------------------------------------

def Catmull_Clark(C,F):

    me=bpy.data.meshes.new("Subdiv1")
    ob2=bpy.data.objects.new("Subdiv1",me)
    me.from_pydata(C,[],F)

    COORD=coord(me)  #New coordinates
    FACES=[] #New faces

    V_E=VE(me)
    V_F=VF(me)
    E_F=neighbors(me)

    centroid_ID=[-1 for i in range(len(me.polygons))]
    edge_vertex_ID=[-1 for i in range(len(me.edges))]

    #Compute face points
    for face in range(len(me.polygons)):
        Vertices = me.polygons[face].vertices[:]

        centroid=barycenter(Vertices,COORD)  #Compute centroid and add it to new vertices
        centroid_ID[face]=add_check(centroid,COORD)

    #Compute edge points
    for edge in range(len(me.edges)):
        mean_list=[me.edges[edge].vertices[0],me.edges[edge].vertices[1]]
        for face in E_F[edge]:
            mean_list.append(centroid_ID[face])
        edge_vertex=barycenter(mean_list,COORD)
        edge_vertex_ID[edge]=add_check(edge_vertex,COORD)

    #Loop on all faces
    for face in range(len(me.polygons)):
        Vertices=me.polygons[face].vertices[:]

        for v in range(len(Vertices)):

            edges=V_E[Vertices[v]] #Edges shared by vertex v
            faces=V_F[Vertices[v]] #Faces shared by vertex v
            mean_faces=faces[:]
            mean_edges=edges[:]

            deg=len(edges)  #Degree of vertex v

            select=[0 for i in range(2)]  #Selection of two edge vertices that will form the face
            for e in edges:
                if (face in E_F[e]):
                    v1=me.edges[e].vertices[0]
                    v2=me.edges[e].vertices[1]
                    #We check the order of the edges (=the order of vertices in the edges)
                    if (v==len(Vertices)-1):
                        if (v1==Vertices[0] or v2==Vertices[0]):
                            select[0]=edge_vertex_ID[e]
                        else:
                            select[1]=edge_vertex_ID[e]
                    else:
                        if (v1==Vertices[v+1] or v2==Vertices[v+1]):
                            select[0]=edge_vertex_ID[e]
                        else:
                            select[1]=edge_vertex_ID[e]


            #Compute F
            for f in range(len(faces)):
                mean_faces[f]=centroid_ID[faces[f]]
            F=barycenter(mean_faces,COORD)

            #Compute R
            for e in range(len(edges)):
                mean_edges[e]=edge_vertex_ID[edges[e]]
            R=barycenter(mean_edges,COORD)


            new_V=((F[0]+2*R[0]+(deg-3)*me.vertices[Vertices[v]].co[0])/deg,(F[1]+2*R[1]+(deg-3)*me.vertices[Vertices[v]].co[1])/deg,(F[2]+2*R[2]+(deg-3)*me.vertices[Vertices[v]].co[2])/deg)
            COORD[Vertices[v]]=new_V

            #We create a new face with the vertices in the right order
            new_face=(Vertices[v],select[0],centroid_ID[face],select[1])
            FACES.append(new_face)

    return(COORD,FACES)



#-------------------------------------------------------
#-------------------- Iteration ------------------------
#-------------------------------------------------------

#This function compute the subdivision's iterations
#This function avoids to compute the subdivisons at each frame

def iteration(me,n):

    coords=coord(me)
    faces=[]
    for face in me.polygons:
        faces.append(face.vertices[:])

    coord1,face1=coords,faces
    coord2,face2=coords,faces

    for i in range(n):
        coord1,face1=Simple_subdivisison(coord1,face1)
        coord2,face2=Catmull_Clark(coord2,face2)

    return(coord1,coord2,face1)




#-------------------------------------------------------
#----------------------- main --------------------------
#-------------------------------------------------------

#Argument n : define the number of iteration of the models

def main(n):

    # Retrieve the active object (the last one selected)
    ob = bpy.data.scenes['Scene'].objects.active

    # If we are in edit mode, return to object mode
    bpy.ops.object.mode_set(mode='OBJECT')

    # Retrieve the mesh data
    mesh = ob.data

    # Get current time
    t = time()

    coord1,coord2,face=iteration(mesh,n)



    #-------------------------------------------------------
    #------------------ Interpolation ----------------------
    #-------------------------------------------------------


    #This function do the interpolation using the results of the subdivisions' iterations
    #It also creates the corresponding object

    def interpolation(t):

        COORD=coord1[:]

        for i in range(len(coord1)):
            COORD[i]=((1-t)*coord1[i][0]+t*coord2[i][0],(1-t)*coord1[i][1]+t*coord2[i][1],(1-t)*coord1[i][2]+t*coord2[i][2])

        bpy.ops.object.delete()

        me=bpy.data.meshes.new("Subdiv1")
        ob2=bpy.data.objects.new("Subdiv1",me)
        ob2.location=bpy.context.scene.cursor_location
        bpy.context.scene.objects.link(ob2)
        me.from_pydata(COORD,[],face)
        me.update(calc_edges=True)
        bpy.data.objects["Subdiv1"].select = True
        bpy.context.scene.objects.active = bpy.data.objects["Subdiv1"]
        ob2.select=True


    #-------------------------------------------------------
    #--------------------- Callback ------------------------
    #-------------------------------------------------------

    # Callback function that is called each time the frame changes

    def Callback(scene):
        #We get the start and stop frames of the animation
        strt=scene.frame_start
        end = scene.frame_end
        #We compute the step
        num=end-strt
        frame = scene.frame_current
        step = float(frame-strt)/num
        #step is between 0 & 1
        interpolation(step)


    bpy.data.scenes['Scene'].frame_end=100
    bpy.data.scenes['Scene'].frame_current=0

    bpy.app.handlers.frame_change_pre.clear()
    bpy.app.handlers.frame_change_pre.append(Callback)


    # Report performance...
    print("Script took %6.2f secs.\n\n"%(time()-t))

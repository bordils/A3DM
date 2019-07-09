import bpy
from time import time

def centroid(thingy):
	print("CENTROID POSITION")
	x1 = 0.0
	y1 = 0.0
	z1 = 0.0
	
	for i in range(len(thingy.vertices)):
		x1 += thingy.vertices[i].co[0]
		y1 += thingy.vertices[i].co[1]
		z1 += thingy.vertices[i].co[2]
	xc = r(x1/len(thingy.vertices))
	yc = r(y1/len(thingy.vertices))
	zc = r(z1/len(thingy.vertices))
	print(xc, yc, zc)

def r(a):
	return int(a*1000+0.5)/1000.0


def main():
	ob = bpy.data.scenes['Scene'].objects.active
	
	if not ob or ob.type != 'MESH':
		BPyMessages.Error_NoMeshActive()
		return
		
	bpy.ops.object.mode_set(mode='OBJECT')
	
	mesh = ob.data
	
	centroid(mesh)
	
	t = time()
	print("script took %6.2f secs.\n\n"%(time()-t))
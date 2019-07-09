import bpy
import numpy
import math
#import random
from mathutils import Matrix
from enum import Enum
from time import time


class shapeType(Enum):
	empty = 0
	cube  = 1
	
class shape(object):
	id   = ""
	type = shapeType.empty
	position = []
	scale = []
	mesh = bpy.ops.object.add()
	
	scopeID = ""
	
	def __init__(self, id, type, position, scale, scopeID):
		self.id = id
		self.type = type
		self.position = position
		self.scale = scale
		self.scopeID = scopeID
		
		if(self.type == shapeType.cube):
			bpy.ops.mesh.addCube()
			self.mesh = bpy.context.object
			self.mesh.name = 'cube'
		else:
			print("no valid shape")
			return
		self.update()
		
	def update(self)
		global scopes
		p = scopes[self.scopeID].p
		self.mesh.scale = (self.scale[0],self.scale[1], self.scale[2]])
		self.mesh.location = (p[0] + self.position[0],p[1] + self.position[1], p[2] + self.position[2])
		
	def translate(self, x, y, z):
		self.position[0] += x
		self.position[1] += y
		self.position[2] += z
		self.update()
	
	def rescale(self, x, y, z):
		self.scale[0] *= x
		self.scale[1] *= y 
		self.scale[2] *= z

class scope(object):
	id = ""
	position = []
	scale = []
	shapes = []
	
	def __init__(self, id, position, scale):
		self.id = id
		self.position = position
		self.scale = scale
		self.shapes = []
	
	def addCube(self, position, scale):
		id = len(self.shapes)
		cube = shape(id, shapeType.cube, position, scale, self.id)
		self.shapes.append(cube)
	
	def resizeScope(self,x,y,z):
	
	def translateScope(self,x,y,z):
	
def addScope(x=0,y=0,z=0,sx=1,sy=1,sz=1):
	global scopeCount
	id = scopeCount
	position = (x,y,z)
	scale = (sx,sy,sz)
	scope = scope(id, position, scale)
	global scopes
	scopes.append(scope)
	scopeCount +=1
	return scope


def main(): 

	


    t = time()
    print("Script took %6.2f secs.\n\n"%(time()-t))
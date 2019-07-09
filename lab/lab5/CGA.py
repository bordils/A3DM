#based on "Procedural Modeling of Buildings" by Muller, Wonka and others

import bpy
import numpy
import math

from time import time

scopeCount = 0
scopes = []

def main():
	global scopes
	clear()
	scope0 = addScope( (0,0,0), (1,1,1) )
	scope0.addCube( (0,0,0), (1,1,1) )
	scope0.addCylinder( (0,0,0), (0.6,0.6,2) )
	scope1 = addScope( (2.2,0,0), (1,1,1) )
	scope1.addCube( (0,0,0), (1,1,1) )
		
def nScopes():
	print(len(scopes))

def clear():
	scopeCount = 0
	#scopes.clear()
	for i in bpy.data.objects:
		if i.type == 'MESH':
			i.select = True
		else:
			i.select = False
	bpy.ops.object.delete()

class Shape(object):
	id = ""
	type = 0 # 0 = none, 1 = cube, 2 = cylinder
	position = []
	size = []
	mesh = bpy.ops.object.add()
	
	scopeID = ""
	
	def __init__(self, id, type, position, size, scopeID):
		self.id = id
		self.type = type
		self.position = position
		self.size = size
		self.scopeID = scopeID
		
		if(self.type == 1):
			bpy.ops.mesh.primitive_cube_add()
			self.mesh = bpy.context.object
			self.mesh.name = 'cube'
		elif(self.type ==2):
			bpy.ops.mesh.primitive_cylinder_add()
			self.mesh = bpy.context.object
			self.mesh.name = 'cylinder'
		self.update()
	
	def update(self):
		global scopes
		scopePos = scopes[self.scopeID].position
		self.mesh.scale = (self.size[0],self.size[1],self.size[2])
		self.mesh.location = (  scopePos[0] + self.position[0],
								scopePos[1] + self.position[1],
								scopePos[2] + self.position[2])


#each scope is built of several shapes
class Scope(object):
	#name of the scope
	id = ""
	#position of the scope
	position = []
	#size of the scope
	size = []
	#shapes (actual meshes it is made of)
	shapes = []
	
	def __init__(self, id, position, size):
		self.id = id
		print("init ID", self.id)
		self.position = position
		self.size = size
		self.shapes = []
	def addCube(self, position, size):
		id = len(self.shapes)
		cube = Shape(id,1,position, size, self.id)
		self.shapes.append(cube)
	def addCylinder(self, position, size):
		id = len(self.shapes)
		cube = Shape(id,2,position, size, self.id)
		self.shapes.append(cube)

def addScope(position=(0,0,0), scale=(1,1,1)):
	global scopeCount
	id = scopeCount
	scope = Scope(scopeCount,position,scale)
	global Scopes
	scopes.append(scope)
	scopeCount += 1
	return scope


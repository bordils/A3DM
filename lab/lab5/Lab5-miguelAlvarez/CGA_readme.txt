Advanced 3D Modeling. Laboratory 5: CGA Architecture
====================================================
====================================================
====================================================
based on the paper:
"Procedural Modeling of Buildings" 
by Muller, Wonka and others
====================================================
====================================================
====================================================


On blender console run the following instructions:
>>>import sys
>>>import importlib
>>>dir = 'C:/'cotaining_CGAfile_directory''
>>>sys.path.append(dir)
>>>import CGA
>>>CGA.main()

====================================================
====================================================

To change the rules, modify the CGA.py in "main"
according to the set of rules stated below.
After modifying it run the following instructions:

>>>importlib.reload(CGA)
>>>CGA.clear()
>>>CGA.main()

====================================================
====================================================
CGA RULES:
Scope-based instructions:
====================================================
addScope(position(x,y,z),size(sx,sy,sz))
#todo
deleteScope
translateScope
scaleScope
rotateScope
subdivision
pushShape


====================================================
Shape-based instructions:

scope.addCube(position(x,y,z),size(sx,sy,sz))
scope.addCylinder(position(x,y,z),size(sx,sy,sz))

#todo
deleteShape
translateShape
scaleShape
totateShape
extrudeShape
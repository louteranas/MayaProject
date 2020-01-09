#! /usr/bin/env python3


import parser as parser
from joint import Joint
import sys

class ScriptWriter:
    def __init__(self, root = None):
        if(root == None):
            print("can't write a script without Root joint, please check the parametres")
            return
        self.root = root 
    
    def runTree(self, joint, script, lastParentList):
        # tempLastParentList = lastParentList
        print(joint.name)
        if(joint.isRoot):
            # print("coucou Root")
            # tempLastParentList += [1]*(len(joint.child))
            joint.setPosition(joint.translation[0])
            script.write("cmds.select(d=True )\n")
            position = (joint.position[0] + joint.offset[0], joint.position[1] + joint.offset[1], joint.position[2] + joint.offset[2])
            script.write("cmds.joint(p=("+str(position[0])+\
                ", "+str(position[1])+\
                ", "+str(position[2]) + ")  )\n")
        else:
            # print(" JE suis un joint")
            # if(not joint.child == None and len(joint.child)>1):
            #     # print(joint.name)
            #     tempLastParentList += [joint.numMaya]*(len(joint.child) -1)
            position = (joint.parent.position[0] + joint.offset[0], joint.parent.position[1] + joint.offset[1], joint.parent.position[2] + joint.offset[2])
            joint.setPosition(position)
            script.write("cmds.joint(p=("+str(position[0])+\
                ", "+str((position[1]))+\
                ", "+str((position[2])) + ")  )\n")
            script.write("cmds.joint('joint"+str(joint.parent.numMaya)+"', e=True, zso=True, oj='xyz' )\n")
            if(joint.child == None):
                # print(" ca sort  ou ca sort pas ?")
                return
        
        # if(len(joint.child) > 1 ):
        #     print(joint.name + " : " + str(len(joint.child)))
        for child in joint.child:
            self.runTree(child, script, lastParentList)
            print(joint.name + " " + str(joint.numMaya))
            script.write("cmds.select('joint"+str(joint.numMaya)+"', r=True )\n")
            # tempLastParentList.pop()

    def writeScript(self):
        script = open('script.py', 'w')
        script.write("#! /usr/bin/env python3\n")
        script.write("\n")
        script.write("import maya.cmds as cmds\n")
        self.runTree(self.root, script, [])
        script.close()
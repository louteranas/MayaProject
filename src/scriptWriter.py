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
    
    def runTree(self, joint, script, positionParent):
        if(joint.isRoot):
            joint.setPosition(joint.translation[0])
            script.write("cmds.select(d=True )\n")
            position = (joint.position[0] + joint.offset[0], joint.position[1] + joint.offset[1], joint.position[2] + joint.offset[2])
            joint.setPosition(position)
            script.write("cmds.joint(p=("+str(position[0])+\
                ", "+str(position[1])+\
                ", "+str(position[2]) + ")  )\n")
            
        else:
            position = (positionParent[0] + joint.offset[0], positionParent[1] + joint.offset[1], positionParent[2] + joint.offset[2])
            joint.setPosition(position)
            script.write("cmds.joint(p=("+str(position[0])+\
                ", "+str((position[1]))+\
                ", "+str((position[2])) + ")  )\n")

            if(joint.child == None):
                return
        
        oldJoint = joint
        for child in joint.child:
            script.write("cmds.select('joint"+str(joint.numMaya)+"', r=True )\n")
            if child.parent != joint:
                print("GROSSE ERREUR")
            self.runTree(child, script, joint.position)
    
    def animate(self, joint, script, numFrame):
        if(joint.child == None):
            return
        script.write("cmds.select('joint"+str(joint.numMaya)+"', r=True )\n")
        indexTime = 0
        if joint.isRoot :
            # # script.write("cmds.currentTime("+ str(numFrame*100 + joint.transformationIndex) +")\n") ,insert=True
            # script.write("cmds.move("+str(joint.translation[numFrame][0]) + "," + \
            # str(joint.translation[numFrame][1]) + "," + str(joint.translation[numFrame][2]) +", r=True )\n")

            script.write("cmds.setKeyframe('joint"+ str(joint.numMaya) +", v="+str(joint.translation[numFrame][0])+" , attribute='translateX', time = "+str(numFrame*100 + joint.transformationIndex)+"')\n" )
            script.write("cmds.setKeyframe('joint"+ str(joint.numMaya) +", v="+str(joint.translation[numFrame][2])+" , attribute='translateY', time = "+str(numFrame*100 + joint.transformationIndex)+"')\n" )
            script.write("cmds.setKeyframe('joint"+ str(joint.numMaya) +", v="+str(joint.translation[numFrame][2])+" , attribute='translateZ', time = "+str(numFrame*100 + joint.transformationIndex)+"')\n" )
          
        
        
        
        script.write("cmds.setKeyframe('joint"+ str(joint.numMaya) +", v="+str(joint.rotation[numFrame][2])+" , attribute='rotateZ', time = "+str(numFrame*100 + joint.transformationIndex)+"')\n" )
        script.write("cmds.setKeyframe('joint"+ str(joint.numMaya) +", v="+str(joint.rotation[numFrame][1])+" , attribute='rotateY', time = "+str(numFrame*100 + joint.transformationIndex)+"')\n" )
        script.write("cmds.setKeyframe('joint"+ str(joint.numMaya) +", v="+str(joint.rotation[numFrame][0])+" , attribute='rotateX', time = "+str(numFrame*100 + joint.transformationIndex)+"')\n" )
     
        # script.write("cmds.rotate("+str(joint.rotation[numFrame][0]) + "," + \
        #     str(joint.rotation[numFrame][1]) + "," + str(joint.rotation[numFrame][2]) +", r=True )\n")
        
        
        # script.write("cmds.setKeyframe('joint"+ str(joint.numMaya) +".rotate')\n" )
        # script.write("cmds.setKeyframe('joint"+ str(joint.numMaya) +"', time="+ str(numFrame*100 + joint.transformationIndex) +")\n" )
        # # script.write("cmds.currentTime("+ str(numFrame*100 + joint.transformationIndex) +")\n")
        # script.write("cmds.setKeyframe('joint"+ str(joint.numMaya) +"', time="+ str(numFrame*100 + joint.transformationIndex) +")\n" )
        for child in joint.child:
            self.animate(child, script, numFrame)




    def writeScript(self):
        script = open('script.py', 'w')
        script.write("#! /usr/bin/env python3\n")
        script.write("\n")
        script.write("import maya.cmds as cmds\n")
        self.runTree(self.root, script, [])
        
        for i in range(len(self.root.translation)):
            self.animate(self.root, script, i)
        script.close()
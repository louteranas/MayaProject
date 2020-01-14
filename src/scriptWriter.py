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

    def animate(self, joint, script):
        nbFrames = len(joint.rotation)
        for i in range(len(joint.rotation)):
            numFrame = i * 10
            if joint.isRoot :
                script.write('cmds.setKeyframe("joint' + str(joint.numMaya) + '.translateX", value = ' + str(joint.translation[i][0]) + ', time=' + str(numFrame) +')\n')
                script.write('cmds.setKeyframe("joint' + str(joint.numMaya) + '.translateY", value = ' + str(joint.translation[i][1]) + ', time=' + str(numFrame) +')\n')
                script.write('cmds.setKeyframe("joint' + str(joint.numMaya) + '.translateZ", value = ' + str(joint.translation[i][2]) + ', time=' + str(numFrame) +')\n')
            script.write('cmds.setKeyframe("joint' + str(joint.numMaya) + '.rotateX", value = ' + str(joint.rotation[i][0]) + ', time=' + str(numFrame) +')\n')
            script.write('cmds.setKeyframe("joint' + str(joint.numMaya) + '.rotateY", value = ' + str(joint.rotation[i][1]) + ', time=' + str(numFrame) +')\n')
            script.write('cmds.setKeyframe("joint' + str(joint.numMaya) + '.rotateZ", value = ' + str(joint.rotation[i][2]) + ', time=' + str(numFrame) +')\n')

        if joint.child == None:
            return
        for child in joint.child:
            self.animate(child, script)




    def writeScript(self):
        script = open('script.py', 'w')
        script.write("#! /usr/bin/env python3\n")
        script.write("\n")
        script.write("import maya.cmds as cmds\n")
        self.runTree(self.root, script, [])

        self.animate(self.root, script)
        script.close()

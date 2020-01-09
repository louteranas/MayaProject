#! /usr/bin/env python3

from joint import Joint 
import sys
from tqdm import tqdm
import time

def parse(argFile = None):
    if(argFile == None):
        print("No file was given to parse")
        return
    indexOfpos = 0
    with open(argFile, 'r') as argument:
        data = argument.read()
        lines = data.split("\n")
        currentJoint = Joint()
        root = Joint()
        compteur = 0
        transformationIndex = 0
        MayaNumber = 1
        for index, line in enumerate(lines):
            compteur += 1
            if(line == "HIERARCHY"):
                # print("Parsing the hierarchical model : ")
                continue
            if("ROOT" in line):
                # print("Root")
                jointName = line.split(' ')[1]
                root = Joint(jointName, True)
                currentJoint = root
                currentJoint.setNumMaya(MayaNumber)
                MayaNumber += 1
                continue
            if("OFFSET" in line):
                # print("Reading offset")
                lineData = line.split(' ')
                currentJoint.setOffset(lineData[1:])
                continue
            if("CHANNELS" in line):
                lineData = line.split(' ')
                if(lineData[1] == "6"):
                    # print("Reading Root channel")
                    currentJoint.setTransformationIndex(transformationIndex)
                    # print(currentJoint.transformationIndex)
                    transformationIndex += 6
                    continue
                if(lineData[1] == "3"):
                    # print("Reading joint channel")
                    currentJoint.setTransformationIndex(transformationIndex)
                    transformationIndex += 3
                    continue
            if("JOINT" in line):
                # print("ltoes" in currentJoint.name)

                # print("Joint")
                jointName = line.split(' ')[1]
                joint = Joint(jointName, False)
                # if(currentJoint == None):
                #     print("Stooooooooooooop")
                joint.setParent(currentJoint)
                currentJoint.addChild(joint)
                currentJoint = joint
                currentJoint.setNumMaya(MayaNumber)
                MayaNumber += 1
                continue
            if("End" in line):
                # print("End Joint")
                jointName = str(currentJoint.name + "-Child")
                joint = Joint(jointName, False)
                currentJoint.addChild(joint)
                joint.setParent(currentJoint)
                currentJoint = joint
                continue
            if("}" in line):
                currentJoint = currentJoint.parent
            if("MOTION" in line):
                indexOfpos = index + 3
                # print("Arbre fini")
                # return root
        for index, line in enumerate(lines[indexOfpos:]):
            transformations = line.split(" ")
            if(not transformations == [''] and index == 0):
                transformations = [item for item in transformations]
                # print(transformations)
                applyTransformations(root, transformations)
    return root
            
def applyTransformations(joint, transformations):
    if(joint.child == None):
        joint.rotation = []
        # print("\n\n\n Nope, cul de sac \n\n\n ")
        return 
    if(joint.isRoot):
        joint.setTranslation([float(transformations[joint.transformationIndex]), float(transformations[joint.transformationIndex+1]), float(transformations[joint.transformationIndex+2])])
        joint.setRotation([float(transformations[joint.transformationIndex+5]), float(transformations[joint.transformationIndex+4]), float(transformations[joint.transformationIndex+3])])
    else:
        joint.setRotation([float(transformations[joint.transformationIndex+2]), float(transformations[joint.transformationIndex+1]), float(transformations[joint.transformationIndex])])
        joint.rotation = [joint.rotation[-1]]

    for child in joint.child:
        applyTransformations(child, transformations)
    


def printH(joint):
    if(joint.child == None):
        print("go back !")
        return
    print(joint.name, end=' ')
    if(joint.isRoot):
        print(joint.translation[:1], end=' ')
        print(joint.rotation[:1], end=' \n')
    print(" Has children : ", end = '')
    for child in joint.child:
        print(child.name + " ", end = '')
    print("\n") 
    liste = joint.child
    # for joints in liste:
    #     print(joints.name + " " +  str(joints.transformationIndex) , end=' ') 
    # for joints in liste:
    #     print(joints.rotation, end=' ')
    print('\n\n')
    for child in joint.child:
        printH(child)





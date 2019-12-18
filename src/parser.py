#! /usr/bin/env python3

from joint import Joint 
import sys
from tqdm import tqdm
import time

def parse(argFile = None):
    if(argFile == None):
        print("No file was given to parse")
        return
    with open(argFile, 'r') as argument:
        data = argument.read()
        lines = data.split("\n")
        currentJoint = Joint()
        root = Joint()
        compteur = 0;
        for line in lines:
            compteur += 1
            if(line == "HIERARCHY"):
                print("Parsing the hierarchical model : ")
                continue
            if("ROOT" in line):
                jointName = line.split(' ')[1]
                root = Joint(jointName, True)
                currentJoint = root
                continue
            if("OFFSET" in line):
                lineData = line.split(' ')
                currentJoint.setOffset(lineData[1:])
                continue
            if("CHANNELS" in line):
                lineData = line.split(' ')
                if(lineData[1] == 6):
                    currentJoint.setTranslation(lineData[2:5])
                    currentJoint.setRotation(lineData[5:])
                    continue
                if(lineData[1] == 3):
                    currentJoint.setTranslation(lineData[2:5])
                    currentJoint.setRotation(lineData[5:])
                    continue
            if("JOINT" in line):
                # print("ltoes" in currentJoint.name)
                jointName = line.split(' ')[1]
                joint = Joint(jointName, False)
                if(currentJoint == None):
                    print("Stooooooooooooop")
                joint.setParent(currentJoint)
                currentJoint.addChild(joint)
                currentJoint = joint
                continue
            if("End" in line):
                jointName = str(currentJoint.name + "-Child")
                joint = Joint(jointName, False)
                currentJoint.addChild(joint)
                joint.setParent(currentJoint)
                currentJoint = joint
                continue
            if("}" in line):
                currentJoint = currentJoint.parent
            if("MOTION" in line):
                print("Arbre fini")
                return root
            

def printH(joint):
    if(joint.child == None):
        print("go back !")
        return
    print(joint.name, end=' ')
    print(" Has children : ")
    liste = joint.child
    for joints in liste:
        print(joints.name, end=' ') 
    print('\n')
    for child in joint.child:
        printH(child)



def main():
    root = parse(sys.argv[1])
    # for i in tqdm(range(10)):
    #     continue
    #    time.sleep(0.1)
    # printH(root)

main()
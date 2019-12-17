#! /usr/env/bin pyhton2.7

import Joint from join

def parse(argFile = None):
    if(argFile == None):
        print("No file was given to parse")
        return
    with open(argFile, 'r') as argument:
        data = argument.read()
        lines = data.split("\n")
        currentJoint = Joint()
        for line in lines:
            if(ligne = "HIERARCHY"):
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
                currentJoint.setOffset(lineData[1:])
                continue




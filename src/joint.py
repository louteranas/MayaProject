#! /usr/bin/env python2.7

class Joint:
    def __init__(self, name = "Unknown", isRoot = False, transformationIndex = "-1", offset = [], rotation = [], translation = [], child = None, parent = None):
        self.name = name
        self.isRoot = isRoot
        self.offset = offset
        self.position = (0, 0, 0)
        self.rotation = rotation
        self.child = child
        self.translation = translation
        self.parent = parent
        self.transformationIndex = transformationIndex
        self.numMaya = 0

    def getRoot(self):
        parent = self.parent
        while(parent != None):
            parent = parent.parent
        print(self.name + " root is : " + parent.name)
        return parent

    def setOffset(self, offset):
        self.offset = (float(offset[0]), float(offset[1]), float(offset[2]))
        
    def setTransformationIndex(self, index):
        self.transformationIndex = index

    def setRotation(self, rotation):
        self.rotation.append(rotation)

    def addChild(self, child):
        if(self.child == None):
            self.child = []
        self.child.append(child)

    def setPosition(self, position):
        self.position = (position[0], position[1], position[2])

    def setParent(self, parent):
        self.parent = parent

    def setNumMaya(self, number):
        self.numMaya = number

    def setTranslation(self, translation):
        if(self.isRoot):
            self.translation.append(translation)
        else:
            print("Only Root can take a translation, Joints have offsets")

#! /usr/bin/env python2.7

class Joint:
    def __init__(self, name = "Unknown", isRoot = False, offset = [0,0,0], rotation = [0,0,0], translation = [0,0,0], child = None, parent = None):
        self.name = name
        self.isRoot = isRoot
        self.offset = offset
        self.rotation = rotation
        self.child = child
        self.translation = translation
        self.parent = parent

    def getRoot(self):
        parent = self.parent
        while(parent != None):
            parent = parent.parent
        print(self.name + " root is : " + parent.name)
        return parent
    
    def setOffset(self, offset):
        self.offset = offset
    
    def setRotation(self, rotation):
        self.rotation = rotation
    
    def addChild(self, child):
        if(self.child == None):
            self.child = []
        self.child.append(child)
    
    def setParent(self, parent):
        self.parent = parent
    
    def setTranslation(self, translation):
        self.translation = translation

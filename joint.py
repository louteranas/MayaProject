#! /usr/env/bin pyhton2.7

class Joint:
    def __init__(self, name = "Unknown", isRoot = False, offset = (0,0,0), rotation = (0,0,0),child = None, parent = None):
        self.name = name
        self.isRoot = isRoot
        self.offset = offset
        self.rotation = rotation
        self.child = child
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
    
    def setChild(self, child):
        self.child = child
    
    def setParent(self, parent):
        self.parent = parent
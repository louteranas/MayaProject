#include "joint.h"


Joint::Joint(string name, bool isRoot, std::array<float, 3> offset,
  std::vector<std::array<float, 3>> rotations,
  std::vector<std::array<float, 3>> translations,
  int transformationIndex, Joint *parent, std::list<Joint*> childs){
    this->name = name;
    this->isRoot = isRoot;
    this->offset = offset;
    this->rotations = rotations;
    this->translations = translations;
    this->transformationIndex = transformationIndex;
    this->parent = parent;
    this->childs = childs;
}

string Joint::getName(){
    return this->name;
}

void Joint::addChild(Joint* child){
    childs.push_back(child);
}

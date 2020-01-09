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

string Joint::toString(){
    string s = this->name + ":\nChildren\n";
    for(std::list<Joint*>::iterator  it = this->childs.begin(); it != this->childs.end(); ++it){
      s += (*it) -> name + "\n";
    }
    if(this->parent != NULL){
      s += "Parent\n";
      s += this->parent->name + "\n";
    }
    return s;
}

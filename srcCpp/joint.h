#include <iostream>
#include <list>
#include <vector>
#include <array>
using namespace std;

class Joint;

class Joint{
  public:
      Joint(string name, bool isRoot, std::array<float, 3> offset,
        std::vector<std::array<float, 3>> rotations,
        std::vector<std::array<float, 3>> translations,
        int transformationIndex, Joint *parent, std::list<Joint*> childs);

      string getName();
      string toString();
      void addChild(Joint* child);

      std::vector<std::array<float, 3>> rotations;
      std::vector<std::array<float, 3>> translations;
      std::list<Joint*> childs;
  private:
      string name;
      bool isRoot;
      std::array<float, 3> offset;
      int transformationIndex;

      Joint *parent;
};

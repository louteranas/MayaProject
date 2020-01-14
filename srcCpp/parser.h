#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include <array>
#include "joint.h"
using namespace std;

std::vector<Joint*> parse(string argFile);
void skipIntro(ifstream& flux);
std::array<double, 3> readOffset(ifstream& flux);
int readChannels(ifstream& flux);
Joint* readRoot(ifstream& flux);
string readJoint(ifstream& flux, Joint* parent, std::vector<Joint*>& lJoints, string firstLine);
int readFramesNumber(ifstream& flux);
float readFrameTime(ifstream& flux);

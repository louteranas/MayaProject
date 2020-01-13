#ifndef _JOINT_H_
#define _JOINT_H_

#include <string>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>

class AnimCurve {
public :
	AnimCurve() {};
	AnimCurve(const char* name) {
			this->name = name;
	};
	~AnimCurve() {
		_values.clear();
	}
public :
	std::string name;					// name of dof
	std::vector<double> _values;		// different keyframes = animation curve
};


enum RotateOrder {roXYZ=0, roYZX, roZXY, roXZY, roYXZ, roZYX};

class Joint {
public :
	std::string _name;					// name of joint
	int id_liste;
	double _offX;						// initial offset in X
	double _offY;						// initial offset in Y
	double _offZ;						// initial offset in Z
	std::vector<AnimCurve> _dofs;		// keyframes : _animCurves[i][f] = i-th dof at frame f;
	double _curTx;						// current value of translation on X
	double _curTy;						// current value of translation on Y
	double _curTz;						// current value of translation on Z
	double _curRx;						// current value of rotation about X (deg)
	double _curRy;						// current value of rotation about Y (deg)
	double _curRz;						// current value of rotation about Z (deg)
	int _rorder;						// order of euler angles to reconstruct rotation
	std::vector<Joint*> _children;	// children of the current joint


public :
	// Constructor :
	Joint() {};
	// Destructor :
	~Joint() {
		_dofs.clear();
		_children.clear();
	}

	// Create from data :
	static Joint* create(std::string name, double offX, double offY, double offZ, Joint* parent, int id_liste) {
		Joint* child = new Joint();
		child->_name = name;
		child->_offX = offX;
		child->_offY = offY;
		child->_offZ = offZ;
		child->_curTx = 0;
		child->_curTy = 0;
		child->_curTz = 0;
		child->_curRx = 0;
		child->_curRy = 0;
		child->_curRz = 0;
		child->id_liste = id_liste;
		if(parent != NULL) {
			parent->_children.push_back(child);
		}
		//Initialisation dofs
		child->_dofs = std::vector<AnimCurve>();
		child->_dofs.push_back(AnimCurve("Xposition"));
		child->_dofs.push_back(AnimCurve("Yposition"));
		child->_dofs.push_back(AnimCurve("Zposition"));
		child->_dofs.push_back(AnimCurve("Xrotation"));
		child->_dofs.push_back(AnimCurve("Yrotation"));
		child->_dofs.push_back(AnimCurve("Zrotation"));

		return child;
	}

	// Load from file (.bvh) :
	static Joint* createFromFile(std::string fileName);

	void animate(int iframe=0);

	// Analysis of degrees of freedom :
	void nbDofs();
};


#endif

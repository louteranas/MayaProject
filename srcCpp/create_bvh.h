#ifndef _CREATE_H_
#define _CREATE_H_

#include <cstring>
#include <map>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <experimental/filesystem>

using namespace std;

class Quaternion {
public:
	// Constructor
	Quaternion(double w, double x, double y, double z) {
		this->w = w;
		this->x = x;
		this->y = y;
		this->z = z;
	}
	Quaternion() {
		this->w = 1;
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}
	//Attributes
	double w;
	double x;
	double y;
	double z;
	//Computes the quaternion's norm
	const double norme() {
		return sqrt(this->w * this->w +	this->x * this->x + this->y * this->y + this->z * this->z);
	}
	//Returns the quaternion's conjugate
	Quaternion* get_conjugate() {
		return new Quaternion(this->w, -this->x, -this->y, -this->z);
	}
	//Return the quaternion's inverse
	Quaternion* get_inv() {
		Quaternion* q_inv = this->get_conjugate();
		*q_inv /= (pow(this->norme(), 2));
		return q_inv;
	}
	//D�finition de l'op�rateur "quaternion /= double"
	Quaternion& operator/=(const double& scal)
	{
		this->w /= scal;
		this->x /= scal;
		this->y /= scal;
		this->z /= scal;
		return *this;
	}
	//Converts quaternion to Euler angles
	const double* projection_xyz() {
		//Computes phi angle
		double phi = atan2(2 * (this->w * this->x + this->y * this->z),
			1 - 2 * (this->x * this->x + this->y * this->y));
		//Computes theta angle
		double theta = asin(2*(this->w * this->y - this->x * this->z));
		//Computes ksi angle
		double ksi = atan2(2 * (this->w * this->z + this->x * this->y),
			1 - 2 * (this->y * this->y + this->z * this->z));
		//Return in an array
		double* proj = new double(3);
		proj[0] = phi * 180 / M_PI;
		proj[1] = theta * 180 / M_PI;
		proj[2] = ksi * 180 / M_PI;
		return proj;
	}
	//Returns multiplication of current Quaternion with another one,
	//i.e returns the composition of the two rotations
	Quaternion* multiply(const Quaternion* q2) {
		double w = this->w * q2->w - this->x * q2->x - this->y * q2->y - this->z * q2->z;
		double x = this->w * q2->x + this->x * q2->w + this->y * q2->z - this->z * q2->y;
		double y = this->w * q2->y - this->x * q2->z + this->y * q2->w + this->z * q2->x;
		double z = this->w * q2->z + this->x * q2->y - this->y * q2->x + this->z * q2->w;
		return new Quaternion(w, x, y, z);
	}
	//Computes the local rotation of the quaternion, in euler coordinates
	const double* get_rotations(Quaternion* parent) {
		Quaternion* inv_parent = parent->get_inv();
		Quaternion* q_rel = this->multiply(inv_parent);
		return q_rel->projection_xyz();
	}
	Quaternion& operator=(const Quaternion& other) // copy assignment
	{
		this->w = other.w;
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
		return *this;
	}
};
void read_mesure(string filename, map<string, vector<Quaternion*>>& mesures, int& nbFrames);
void extractFilename(string& path);
void createBvhFile(map<string, vector<Quaternion*>> mesures, int nbFrames);

#endif

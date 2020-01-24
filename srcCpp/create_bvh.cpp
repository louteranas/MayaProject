
// TO COMPILE WITH : g++ create_bvh.cpp -o output -lstdc++fs



#include "create_bvh.h"
#include "parser.h"

namespace fs = experimental::filesystem;

void extractFilename(string& path) {
    const size_t last_slash_idx = path.find_last_of("\\/");
    if (string::npos != last_slash_idx)
    {
        path.erase(0, last_slash_idx + 1);
    }
    const size_t period_idx = path.rfind('.');
    if (string::npos != period_idx)
    {
        path.erase(period_idx);
    }
    return;
}

void read_mesure(string filename, map<string, vector<Quaternion*>>& mesures, int& nbFrames) {
    if(filename.find(".txt") != string::npos) {
        ifstream flux(filename);  //Ouverture d'un fichier en lecture
        ifstream flux2(filename);  //Ouverture d'un fichier en lecture
        cout << "Avant : " << filename << endl;
        extractFilename(filename);
        cout << "Après : " << filename << endl;
        //On parse le fichier
        string ligne;
        skipIntro(flux, 6); //On passe les premières lignes de commentaires
        unsigned int frame;
        double w, x, y, z;
        vector<Quaternion*> vect_q;
        mesures[filename] = vect_q;
        //Parsing de toutes les lignes de mesures
        while(getline(flux, ligne)) {
            istringstream ss(ligne);
            ss >> frame >> w >> x >> y >> z;
            Quaternion* q = new Quaternion(w, x, y, z);
            mesures[filename].push_back(q);
        }
        nbFrames = mesures[filename].size();
    }
    return;
}

int findParent(vector<Joint*> lJoints, Joint* joint) {
    for(unsigned int i=0; i!=lJoints.size(); i++) {
        for(unsigned int k=0; k!=lJoints[i]->_children.size(); k++) {
            if(lJoints[i]->_children[k]->id_liste == joint->id_liste) {
                return i;
            }
        }
    }
    return -1;
}

void createBvhFile(map<string, vector<Quaternion*>> mesures, int nbFrames) {
    ifstream flux("../bvhFiles/walk.bvh");
    ofstream myfile;
    myfile.open ("homemade.bvh");
    string line;
    //Recopie de la partie squelette à partir d'un bvh existant
    bool motion = false;
    while(!motion) {
        getline(flux, line);
        myfile << line << endl;
        if(line.find("MOTION") != string::npos) {
            motion = true;
        }
    }
    ifstream homemade("./homemade.bvh");
    //On détermine la hiérarchie du squelette que l'on vient de copier, on stocke les joints dans lJoints
    vector<Joint*> lJoints;
    Joint* root = readSquelette(homemade, lJoints);
    /*for(unsigned int k=0; k!=lJoints.size(); k++) {
        cout << lJoints[k]->_name << endl;
    }*/
    //Ecriture des Frames
    myfile << "Frames: " << nbFrames << endl;
    myfile << "Frame Time: 0.033333" << endl;
    unsigned int nbJoints = lJoints.size();
    Quaternion *q, *q_parent;
    for(unsigned int iFrame=0; iFrame!=nbFrames; iFrame++) {
        myfile << "0.0000 0.0000 0.0000 ";
        for(unsigned int iJoint=0; iJoint!=nbJoints; iJoint++) {
            if(mesures[lJoints[iJoint]->_name].size() == nbFrames) {
                q = mesures[lJoints[iJoint]->_name][iFrame];
                int iJointParent = findParent(lJoints, lJoints[iJoint]);
                //Si le parent a une série de mesures
                if(iJoint != -1 && mesures[lJoints[iJointParent]->_name].size() == nbFrames) {
                    q_parent = mesures[lJoints[iJointParent]->_name][iFrame];
                } else {
                    q_parent = new Quaternion(); //Element neutre de la multiplication
                }
                const double* proj = q->get_rotations(q_parent);
                myfile << proj[2] << " " << proj[1] << " " << proj[0] << " ";
            } else {
                if (lJoints[iJoint]->_name.find("/child") == std::string::npos) {
                    myfile << "0.0000 0.0000 0.0000 ";
                }
            }
        }
        myfile << endl;
    }
    myfile.close();
    return;
}

int main(int argc, char** argv) {
    if(argc != 2) {
        cout << "Command line should be : ./output path/to/mesures/" << endl;
        return EXIT_FAILURE;
    }
    string path = argv[1];
    map<string, vector<Quaternion*>> mesures;
    int nbFrames = 0;
    for (const auto & entry : fs::directory_iterator(path)) {
        read_mesure(entry.path().string(), mesures, nbFrames);
    }
    cout << "Nombre frames : " << nbFrames << endl;
    createBvhFile(mesures, nbFrames);
}

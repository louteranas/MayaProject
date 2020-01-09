#include "parser.h"

void skipIntro(ifstream& flux){ //skip jusque OFFSET
    string ligne;
    for(int i = 0; i < 3; i ++)
        getline(flux, ligne);
}

std::array<float, 3> readOffset(ifstream& flux){

    string str;
    getline(flux, str);
    istringstream ss(str);
    // Traverse through all words
    ss >> str;
    if(str.compare("OFFSET") != 0){
        cout<<"Error parse OFFSET"<<endl;
        cout << str << endl;
        exit(0);
    }
    std::array<float, 3> offset;
    ss >> offset[0] >> offset[1] >> offset[2];
    return offset;
}
int readChannels(ifstream& flux){
    string str;
    getline(flux, str);
    istringstream ss(str);
    // Traverse through all words
    ss >> str;
    if(str.compare("CHANNELS") != 0){
        cout<<"Error parse CHANNELS"<<endl;
        cout << str << endl;
        exit(0);
    }
    int nbChannels;
    ss >> nbChannels;
    return nbChannels;
}

Joint* readRoot(ifstream& flux){
    skipIntro(flux);
    std::array<float, 3> offset = readOffset(flux);
    int nbChannels = readChannels(flux);
    return new Joint("Root", true, offset, std::vector<std::array<float, 3>>(), std::vector<std::array<float, 3>>(), 0, NULL, std::list<Joint*>());
}

void readJoint(ifstream& flux, Joint* parent, list<Joint*>& lJoints){
    string str;
    getline(flux, str);

    istringstream ss(str);
    ss >> str;
    if(str.compare("JOINT") == 0){
        string jointName;
        ss >> jointName;
        getline(flux, str);
        std::array<float, 3> offset = readOffset(flux);
        int nbChannels = readChannels(flux);

        string str;
        //getline(flux, str); //read {
        Joint* joint = new Joint(jointName, false, offset, std::vector<std::array<float, 3>>(), std::vector<std::array<float, 3>>(), 0, parent, std::list<Joint*>());
        lJoints.push_back(joint);
        parent->addChild(joint);
        readJoint(flux, joint, lJoints);
        getline(flux, str); //read }
    }else if(str.compare("End") == 0){
        string jointName = parent->getName() + "/child";
        getline(flux, str);
        std::array<float, 3> offset = readOffset(flux);
        Joint* joint = new Joint(jointName, false, offset, std::vector<std::array<float, 3>>(), std::vector<std::array<float, 3>>(), 0, parent, std::list<Joint*>());
        lJoints.push_back(joint);
        parent->addChild(joint);
        getline(flux, str); //read }
    }
    else{

      cout<<"Error parse JOINT"<<endl;
      cout << str << endl;
      exit(0);
    }

}

void parse(string argFile){
    if(argFile.empty()){
        cout << "No file was given to parse"<<endl;
        exit(1);
    }

    //int indexOfpos = 0;
    string ligne;
    ifstream flux(argFile);  //Ouverture d'un fichier en lecture
    if(flux)
    {
        //Tout est prêt pour la lecture.
        list<Joint*> lJoints;
        Joint* root = readRoot(flux);
        lJoints.push_back(root);
        //ajouter while flux n'est pas fini..
        readJoint(flux, root, lJoints);

    }

}

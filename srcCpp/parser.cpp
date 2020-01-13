#include "parser.h"

void skipIntro(ifstream& flux){ //skip jusque OFFSET
    string ligne;
    for(int i = 0; i < 3; i ++)
        getline(flux >> std::ws, ligne);
}

std::array<float, 3> readOffset(ifstream& flux){

    string str;
    getline(flux >> std::ws, str);
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
    getline(flux >> std::ws, str);
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

string readJoint(ifstream& flux, Joint* parent, list<Joint*>& lJoints, string firstLine){
    string str;
    istringstream ss(firstLine);
    ss >> str;
    if(str.compare("JOINT") == 0){
        string jointName;
        ss >> jointName;
        getline(flux >> std::ws, str);
        std::array<float, 3> offset = readOffset(flux);
        int nbChannels = readChannels(flux);

        string str;
        //getline(flux, str); //read {
        Joint* joint = new Joint(jointName, false, offset, std::vector<std::array<float, 3>>(), std::vector<std::array<float, 3>>(), 0, parent, std::list<Joint*>());
        //cout<<jointName<<endl;
        lJoints.push_back(joint);
        parent->addChild(joint);
        string newFirstLine, newLine;
        getline(flux  >> std::ws, newFirstLine);
        while(newFirstLine.front() != '}'){
            newLine = readJoint(flux, joint, lJoints, newFirstLine + "");
            newFirstLine = newLine + "";
            //cout<<firstLine<<endl;
        }
        getline(flux >> std::ws, str); //read }

        //if(str.front() != '}')
        //cout<<str<<endl;

        return str;
    }else if(str.compare("End") == 0){
        string jointName = parent->getName() + "/child";
        getline(flux >> std::ws, str);
        std::array<float, 3> offset = readOffset(flux);
        Joint* joint = new Joint(jointName, false, offset, std::vector<std::array<float, 3>>(), std::vector<std::array<float, 3>>(), 0, parent, std::list<Joint*>());
        lJoints.push_back(joint);
        parent->addChild(joint);
        getline(flux >> std::ws, str); //read }
        getline(flux >> std::ws, str);
        //cout<<str<<endl;
        return str;
    }
    else{

      cout<<"Error parse JOINT"<<endl;
      cout << str << endl;
      getline(flux >> std::ws, str);
      cout << str << endl;
      getline(flux >> std::ws, str);
      cout << str << endl;
      getline(flux >> std::ws, str);
      cout << str << endl;

      exit(0);
    }

}

int readFramesNumber(ifstream& flux){
    string line;
    int nbFrames;
    getline(flux >> line >> nbFrames, line);
    cout << nbFrames << endl;
    return nbFrames;
}

float readFrameTime(ifstream& flux){
    string line;
    float frameTime;
    getline(flux >> line >> line >> frameTime, line);
    cout << frameTime << endl;
    return frameTime;
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
        list<Joint*> lJoints;
        Joint* root = readRoot(flux);
        lJoints.push_back(root);
        //Tout est prêt pour la lecture.
        //ajouter while flux n'est pas fini..

        string firstLine;
        getline(flux  >> std::ws, firstLine);
        while(firstLine.front() != '}'){
            firstLine = readJoint(flux, root, lJoints, firstLine);
            //getline(flux >> std::ws, firstLine);
            //cout<<firstLine<<endl;
        }
        getline(flux >> std::ws, firstLine);
        cout << firstLine << "\n";

      /*  for(std::list<Joint*>::iterator  it = lJoints.begin(); it != lJoints.end(); ++it){
          cout<< (*it)->toString()<<"\n";
        }*/

        // Frames parsing
        int nbFrames = readFramesNumber(flux);
        float frameTime = readFrameTime(flux);

        float rx, ry, rz, tx, ty, tz;
        string str;
        for (int i = 0; i < nbFrames; i++){
            getline(flux, str);
            istringstream ss(str);
            ss >> rx >> ry >> rz >> tx >> ty >> tz;
            root->rotations.push_back(std::array<float, 3>({rx, ry, rz}));
            root->translations.push_back(std::array<float, 3>({tx, ty, tz}));
            for(std::list<Joint*>::iterator  it = ++(lJoints.begin()); it != lJoints.end(); ++it){
                ss >> rx >> ry >> rz;
                (*it)->rotations.push_back(std::array<float, 3>({rx, ry, rz}));
                (*it)->translations.push_back(std::array<float, 3>({0, 0, 0}));
            }
        }
    }

}
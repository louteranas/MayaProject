#include "parser.h"

void skipIntro(ifstream& flux, unsigned int nbLignes){ //skip nbLignes lignes
    string ligne;
    for(int i = 0; i < nbLignes; i ++)
        getline(flux >> std::ws, ligne);
    return;
}

std::array<double, 3> readOffset(ifstream& flux){

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
    std::array<double, 3> offset;
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
    skipIntro(flux, 3);
    std::array<double, 3> offset = readOffset(flux);
    int nbChannels = readChannels(flux);
    //return new Joint("Root", true, offset, std::vector<std::array<float, 3>>(), std::vector<std::array<float, 3>>(), 0, NULL, std::list<Joint*>());
    return Joint::create("Root", offset[0], offset[1], offset[2], NULL, 1);
}

string readJoint(ifstream& flux, Joint* parent, std::vector<Joint*>& lJoints, string firstLine){
    string str;
    istringstream ss(firstLine);
    ss >> str;
    if(str.compare("JOINT") == 0){
        string jointName;
        ss >> jointName;
        getline(flux >> std::ws, str);
        std::array<double, 3> offset = readOffset(flux);
        int nbChannels = readChannels(flux);

        string str;
        //getline(flux, str); //read {
        //Joint* joint = new Joint(jointName, false, offset, std::vector<std::array<float, 3>>(), std::vector<std::array<float, 3>>(), 0, parent, std::list<Joint*>());
        Joint* joint = Joint::create(jointName, offset[0], offset[1], offset[2], parent, lJoints.size() + 1);
        //cout<<jointName<<endl;
        lJoints.push_back(joint);
        //parent->addChild(joint);
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
        string jointName = parent->_name + "/child";
        getline(flux >> std::ws, str);
        std::array<double, 3> offset = readOffset(flux);
        //Joint* joint = new Joint(jointName, false, offset, std::vector<std::array<float, 3>>(), std::vector<std::array<float, 3>>(), 0, parent, std::list<Joint*>());
        Joint* joint = Joint::create(jointName, offset[0], offset[1], offset[2], parent, lJoints.size() + 1);
        lJoints.push_back(joint);
        //parent->addChild(joint);
        getline(flux >> std::ws, str); //read }
        getline(flux >> std::ws, str);
        //cout<<str<<endl;
        return str;
    }
    else{

      cout<<"Error parse Joint"<<endl;
      cout << str << endl;
      getline(flux >> std::ws, str);
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
    //cout << nbFrames << endl;
    return nbFrames;
}

float readFrameTime(ifstream& flux){
    string line;
    float frameTime;
    getline(flux >> line >> line >> frameTime, line);
    //cout << frameTime << endl;
    return frameTime;
}

Joint* readSquelette(ifstream& flux, vector<Joint*>& lJoints) {
    cout << "Début de parsing du squelette" << endl;
    Joint* root = readRoot(flux);
    lJoints.push_back(root);
    //Tout est prêt pour la lecture.
    //ajouter while flux n'est pas fini..

    string firstLine;
    getline(flux  >> std::ws, firstLine);
    while(firstLine.front() != '}'){
        firstLine = readJoint(flux, root, lJoints, firstLine);

    }
    getline(flux >> std::ws, firstLine);
    cout << "Ligne après lecture du squelette : " <<firstLine << "\n";
    return root;
}


vector<Joint*> parse(string argFile){
    vector<Joint*> lJoints;
    if(argFile.empty()){
        cout << "No file was given to parse"<<endl;
        exit(1);
    }

    //int indexOfpos = 0;
    string ligne;
    ifstream flux(argFile);  //Ouverture d'un fichier en lecture
    if(flux)
    {
        Joint* root = readSquelette(flux, lJoints);

      /*  for(std::list<Joint*>::iterator  it = lJoints.begin(); it != lJoints.end(); ++it){
          cout<< (*it)->toString()<<"\n";
        }*/
        cout << "Début de parsing des frames" << endl;
        // Frames parsing
        int nbFrames = readFramesNumber(flux);
        float frameTime = readFrameTime(flux);
        //cout << frameTime << endl;

        float rx, ry, rz, tx, ty, tz;
        string str;
        for (int i = 0; i < nbFrames; i++){
            getline(flux, str);
            istringstream ss(str);
            ss >> tx >> ty >> tz >> rz >> ry >> rx;
            root->_dofs[0]._values.push_back(rx);
            root->_dofs[1]._values.push_back(ry);
            root->_dofs[2]._values.push_back(rz);
            root->_dofs[3]._values.push_back(tx);
            root->_dofs[4]._values.push_back(ty);
            root->_dofs[5]._values.push_back(tz);
            for(std::vector<Joint*>::iterator it = ++(lJoints.begin()); it != lJoints.end(); ++it) {
				if ((*it)->_name.find("/child") == std::string::npos) {
					ss >> rz >> ry >> rx;
					(*it)->_dofs[0]._values.push_back(rx);
					(*it)->_dofs[1]._values.push_back(ry);
					(*it)->_dofs[2]._values.push_back(rz);
				}
            }
        }
        cout << "Fin de parsing du fichier" << endl;
    } else {
        cout << "Pas de flux" << endl;
    }
    return lJoints;
}

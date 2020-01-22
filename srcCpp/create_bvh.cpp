#include "create_bvh.h"

using namespace std;
namespace fs = std::experimental::filesystem;

map<string, vector<Quaternion>> read_mesures(string path_repository) {
	cout << "Path : " << path_repository << endl;
	map<string, vector<Quaternion>> mesures;
	for (const auto & entry : fs::directory_iterator(path_repository)) {
		cout << entry.path() << std::endl;
	}
	return mesures;
}

int main() {
	read_mesures("C:\\Users\\ensimag\\Desktop\\caulier\\louter\\rotation_en_x\\");
}
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
using namespace std;
#include <PeacAliEnQuery.h>

void help() {
	printf("Help :\n");
	printf("\t -u <userName> : userName used in alien-token-init\n");
	printf("\t -rmdupli      : remove duplicate paths\n");
	printf("\t -rand         : randomize paths' order\n");
	printf("\t -t            : test file (user's path is ignored) \n");
    printf("\t -d <num>      : debug level\n");
	printf("\t                 0-silent(default), 1-std, >=2-debug\n");
	printf("\n");
}

int main ( int argc, char **argv ) {

	int ret = 0;

	string fileName="";
	string userName="proof";
	int debug = 0;
    vector<string> paths;
    bool removeDoubles = false;
    bool randomizePaths = false;

    bool debugArgs=false;
	string argStr;
	for (int iArg=1; iArg < argc; iArg++) {
		argStr = argv[iArg];
		if (debugArgs) printf("Arg[%d] = %s",iArg,argStr.data());
		if (!argStr.compare("-h")) {
			help();
			return 255;
		} else if (!argStr.compare("-rmdupli")) {
			removeDoubles = true;
		} else if (!argStr.compare("-rand")) {
			randomizePaths = true;
		} else if (!argStr.compare("-t")) {
			fileName="/alice/cern.ch/user/p/proof/xrddmtest.zip";
		} else if (!argStr.compare("-d")) {
			if (iArg+1<argc) {
				iArg++;
				argStr = argv[iArg];
				if (argStr.empty() || argStr.find_first_of('-') == 0) {
					argStr = "0";
					iArg--;
				}
			} else {
				argStr = debug;
			}
				debug = atoi(argStr.data());
				if (debugArgs) printf(" %s",argStr.data());
		} else if (!argStr.compare("-u")) {
			if (iArg+1<argc) {
				iArg++;
				argStr = argv[iArg];
				if (argStr.find_first_of('-') == 0) {
					argStr = "";
					iArg--;
				}
			} else {
				argStr = "";
			}
			if (argStr.empty()) argStr = userName;
			userName = argStr.data();
			if (debugArgs) printf(" %s",argStr.data());
		}
		else {
			if (fileName.empty()) fileName = argStr.data();
		}
		if (debugArgs) printf("\n");
	}

	if (fileName.empty()) {
		printf("\nError : No file specified !!!\n\n");
		help();
		return 1;
	}

	PeacAliEnQuery alienQuery;
	alienQuery.SetUser(userName.data());
	if (debug>2) alienQuery.Print();
	if (alienQuery.DoAliEnTokenInit()>0) {
		printf("Error : Error doing AliEn token !!!\n");
		return 1;
	}

	if (alienQuery.ConnectToAliEn()>0) {
		printf("Error : Problem conecting to AliEn !!!\n");
		return 10;

	} else {
		if (debug>1) printf("AliEn connection OK\n");
		if (debug>0) printf("Doing query of '%s' ...\n",fileName.data());

		ret = alienQuery.FillListOfFileNamesFromAliEnUsingWhereis(fileName.data(),paths);
		if (ret) printf("[Error] alien-query : Error doing query of %s !!! [rc=%d]\n",fileName.data(),abs(ret));
		else {

			if (removeDoubles) paths.erase(unique(paths.begin(),paths.end()), paths.end());
			if (randomizePaths) random_shuffle(paths.begin(),paths.end());

			for (int i=0; i<paths.size(); i++)
			printf("%s\n",paths.at(i).data());
		}
	}


	if (debug>1) printf("RC=%d\n",abs(ret));
	return abs(ret);
}

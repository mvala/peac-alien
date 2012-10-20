#include <stdio.h>
#include <PeacAliEnQuery.h>
int main ( int argc, char **argv ) {

    PeacAliEnQuery alienQuery;
    alienQuery.SetUser("mvala");
    alienQuery.Print();
    if (alienQuery.DoAliEnTokenInit()>0) {
    	printf("Error : Error doing AliEn token !!!\n");
    	return 1;
    }
//    if (!alienQuery.IsProxyValid()) {
//    	printf("XrdGsiProxy is not valid!!!\n");
//        if (!alienQuery.ProxyInit()) {
//        	printf("XrdGsiProxy init didn't finish OK !!!\n");
//        	return 1;
//        }
//    }
//
//    if (!alienQuery.IsTokenValid()) {
//    	printf("Warning : AliEn token is not valid!!! Trying to get it ...\n");
//
//        if (!alienQuery.TokenInit()) {
//        	printf("Error : alien-token-init error!!!\n");
//        }
//        if (!alienQuery.IsTokenValid()) {
//        	printf("Error: AliEn token is not valid!!!\n");
//        	return 2;
//        }
//    }

    if (alienQuery.ConnectToAliEn()>0) {
    	printf("Error : Problem conecting to AliEn !!!\n");
    	return 10;

    } else {
    	printf("AliEn connection OK\n");
    	alienQuery.FillListOfFileNamesFromAliEnUsingWhereis("/alice/data/2011/LHC11a/000146824/ESDs/pass4_without_SDD/11000146824009.490/root_archive.zip");
    }


    printf("All OK\n");
    return 0;
}

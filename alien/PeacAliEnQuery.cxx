//#include <signal.h>

#include "PeacAliEnQuery.h"

PeacAliEnQuery::PeacAliEnQuery() :
fRenewCertificateTime ( "1:00" ),
fCertificateDir ( "" ),
fAliEnUserName ( "proof" ),
fXrdgsiproxyCmd ( ALIEN_XRDGSIPROXY ),
fAliEnTokenInitCmd ( ALIEN_ALIEN_TOKEN_INIT ),
fAliEnTokenDestroyCmd ( ALIEN_ALIEN_TOKEN_DESTROY ),
fAliEnTokenInfoCmd( ALIEN_ALIEN_TOKEN_INFO ),
//        fAliEnCpCmd ( ALIEN_ALIEN_CP ),
//        fAliEnTimeout ( 120 ),
        fAliEn ( 0 ),
        fIsAliEnConnected ( false )
//
{
}

PeacAliEnQuery::~PeacAliEnQuery() {
}

void PeacAliEnQuery::Print() const {

	printf("AliEnQuery Info:\n");
	printf("\t%20s = %-20s\n","RenewCertificateTime",fRenewCertificateTime.data());
	printf("\t%20s = %-20s\n","CertificateDir",fCertificateDir.data());
	printf("\t%20s = %-20s\n","AliEnUserName",fAliEnUserName.data());
	printf("\t%20s = %-20s\n","XrdgsiproxyCmd",fXrdgsiproxyCmd.data());
	printf("\t%20s = %-20s\n","AliEnTokenDestroyCmd",fAliEnTokenDestroyCmd.data());
	printf("\t%20s = %-20s\n","AliEnTokenInfoCmd",fAliEnTokenInfoCmd.data());
	//	printf("\t%20s = %-20s\n","AliEnCpCmd",fAliEnCpCmd.data());
}
int PeacAliEnQuery::FillListOfFileNamesFromAliEnUsingWhereis ( string fname ,vector<string> &paths) {

    if ( !fAliEn ) {
//        XrdDMLogError ( "fAliEn is null!!!! Skipping..." );
        return -11;
    }
    if ( !fIsAliEnConnected ) {
//        XrdDMLogError ( "No AliEn connection available!!!!! Skipping..." );
        return -12;
    }
    string alienfilename = fname.c_str();
//    string alienfilename = GetXrdFilename ( fname.c_str() );
    string cmd ( "whereis " );
    cmd.append ( alienfilename );
//     fCurrentFileNameNice = filename.c_str();
//    XrdDMLogDebugFull ( "Doing '%s' ...",cmd.c_str() );
    bool retValWhereis = true;
//     signal ( SIGALRM,CallAliEnTimeout );
//     alarm ( fAliEnTimeout );
//    try {

        retValWhereis = fAliEn->Command ( cmd.c_str() );
//         alarm (0);
//    } catch ( int e ) {
//        XrdDMLogGError ( "fAliEn->Command(%s) time out %d !!! Exiting ...",cmd.c_str(),e );
//        XrdDMLogGError ( "Error in AliEn !!! " );
//        return 0;
//    }

//    XrdDMLogDebugFull ( "Doing '%s' done with %d...",cmd.c_str(),retValWhereis );

    int numberOfReplicas = fAliEn->GetStreamColumns ( 2 );

    if (numberOfReplicas<1) {
        // file was not found in catalogue
        string str = "xrddm [";
        str.append(fname.c_str());
        str.append("] No replicas in AliEn catalogue. File not found");
//        XrdDMLogGError ( str.data());
        return -13;
    }

    string currFileStr,grpath;
//    XrdDMLogDebugFull ( "Number of replicas %d in AliEn catalogue ...",numberOfReplicas );
    for ( int i = 0;i < numberOfReplicas;i++ ) {
        currFileStr = fAliEn->GetStreamFieldValue ( 2, i, 2 );
//        XrdDMLogDebugFull ( "Found Path in AliEn catalogue GR %s ...",currFileStr.c_str() );
        if ( !currFileStr.compare ( 0,7,"guid://" ) ) {
//            XrdDMLogWarning ( "%s starts with 'guid://' !!! Skipping ...",currFileStr.c_str() );
            continue;
        }
//        if ( ( !i ) && ( !fGlobalRedirectorName.empty() ) ) {
//            grpath = GetXrdGRPath ( currFileStr ).c_str();
//            XrdDMLogDebugFull ( "Adding GR %s ...",grpath.c_str() );
//            fListOfFileNamesFromGR.push_back ( grpath );
//             break;
//        }
//        printf ( "AliEn whereis : file ->  %s ...\n",currFileStr.c_str() );
        paths.push_back(currFileStr);
//        fListOfFileNamesFrom.push_back ( currFileStr );
    }
    return 0;
}

int PeacAliEnQuery::ConnectToAliEn() {

    if ( fIsAliEnConnected ) {
//        XrdDMLogWarning ( "Already connected to AliEn. Skipping  ..." );
        return 0;
    }

//    XrdDMLogDebugSilent ( "Connecting to AliEn ..." );

    if ( !fAliEn ) {
//        XrdDMLogDebugSilent ( "Creating GapiUI... " );
        fAliEn = GapiUI::MakeGapiUI ( true );
    } else {
//        XrdDMLogDebugSilent ( "Reusing GapiUI... " );
    }

    if ( !fAliEn ) {
//        XrdDMLogGError ( "Error creating GapiUI !!! " );
        fIsAliEnConnected = false;
        return 1;
    }
//    XrdDMLogDebugSilent ( "GapiUI is OK ... " );
    string alienHost,alienPort,alienUser;


//     XrdDMLogDebugSilent ( "Test ... " );
//    alienHost = GetEnvValue ( "alien_API_HOST" );

//     XrdDMLogDebugSilent ( "Test ... " );
//    alienPort = GetEnvValue ( "alien_API_PORT" );
//     XrdDMLogDebugSilent ( "Test ... " );
//    alienUser = GetEnvValue ( "alien_API_USER" );

//     XrdDMLogDebugSilent ( "Test ... " );
//     XrdDMLogGInfo ( "alien_API_HOST=%s",alienHost.c_str() );
//     XrdDMLogGInfo ( "alien_API_PORT=%s",alienPort.c_str() );
//     XrdDMLogGInfo ( "alien_API_USER=%s",alienPort.c_str() );

// alienHost(fAliEnUserName);

//     if ((alienHost.empty())||(alienPort.empty())||(alienPort.empty())) {
//         XrdDMLogGError ( "\talien_API_HOST=%s",alienHost.c_str() );
//         XrdDMLogGError ( "\talien_API_PORT=%s",alienPort.c_str() );
//         XrdDMLogGError ( "\talien_API_USER=%s",alienPort.c_str() );
//         XrdDMLogGError ( "Error in AliEn Variables !!! " );
//         return 2;
//     }

//    XrdDMLogDebugSilent ( "AliEn variables OK ... " );
//    XrdDMLogDebugSilent ( "Doing  fAliEn->Connect(\"\",0,%s,0) ...", fAliEnUserName.c_str() );
//     fAliEn->Connect ( alienHost.c_str(), atoi ( alienPort.c_str() ), alienUser.c_str(), "0" );

//    signal ( SIGALRM,&CallAliEnTimeout );
//    alarm ( fAliEnTimeout );
//    try {
//        XrdDMLogInfo ( "Connecting to AliEn ..." );
        fAliEn->Connect ( "",0,fAliEnUserName.c_str(),0 );
//         XrdDMLogGInfo ( "Sleeping %d ...",fAliEnTimeout+100);
//         sleep(fAliEnTimeout+100);
//         XrdDMLogGInfo ( "fAliEn->Connect ...");
//        alarm ( 0 );
        fIsAliEnConnected = true;
//    } catch ( int e ) {
//        XrdDMLogGError ( "fAliEn->Connect time out %d !!! Exiting ...",e );
//        XrdDMLogGError ( "Error connecting to AliEn !!! " );
//        fIsAliEnConnected = false;
//        return 1;
//    }


    if ( !fAliEn->Connected() ) {
//        XrdDMLogGError ( "Error connecting to AliEn (fAliEn->Connected()) !!! " );
        fIsAliEnConnected = false;
        return 1;
    } else {
        fIsAliEnConnected = true;
    }
//    XrdDMLogDebugSilent ( "Connecting to AliEn is successful..." );
    return 0;
}
//
//
//string PeacAliEnQuery::GetEnvValue ( string env ) {
//    char * tmpenvvar;
//    tmpenvvar = getenv ( env.c_str() );
//    if ( tmpenvvar!=NULL )
//        return string ( tmpenvvar );
//    else
//        return "";
//}
//
//
int PeacAliEnQuery::DoAliEnTokenInit () {


	    if (!IsProxyValid()) {
	    	printf("XrdGsiProxy is not valid!!!\n");
	        if (!ProxyInit()) {
	        	printf("XrdGsiProxy init didn't finish OK !!!\n");
	        	return 1;
	        }
	    }

	    if (!IsTokenValid()) {
	    	printf("Warning : AliEn token is not valid!!! Trying to get it ...\n");

	        if (!TokenInit()) {
	        	printf("Error : alien-token-init error!!!\n");
	        }
	        if (!IsTokenValid()) {
	        	printf("Error: AliEn token is not valid!!!\n");
	        	return 2;
	        }
	    }
    return 0;
}

bool PeacAliEnQuery::IsTokenValid() {

    if ( fAliEnTokenInfoCmd.empty() ) {
//        XrdDMLogGFatal ( "Command alien-token-info was not found at compilation time !!!" );
//        XrdDMLogFatal ( "Command alien-token-info was not found at compilation time !!!" );
        return false;
    }

    string cmd;
    string redirectTo;

//    if ( !fCurrentGlobalLogFileName.empty() ) {
//        redirectTo.append ( ">> " );
//        redirectTo.append ( fCurrentGlobalLogFileName.c_str() );
//        redirectTo.append ( " 2>&1 " );
//    }
    cmd = fAliEnTokenInfoCmd.c_str();
    cmd.append(" ");
    cmd.append(fAliEnUserName.c_str());
    cmd.append(" > /dev/null 2>&1");

//    XrdDMLogDebugFull ( "Running %s",cmd.c_str() );

    if ( system ( cmd.c_str() ) ) {
//        XrdDMLogGWarning ( "AliEn token expired or not found !!!" );
//        XrdDMLogWarning ( "AliEn token expired or not found !!!" );
        return false;
    }

    return true;
}

bool PeacAliEnQuery::TokenInit () {

    if ( fAliEnTokenInitCmd.empty() ) {
//        XrdDMLogGFatal ( "Command alien-token-init was not found at compilation time !!!" );
//        XrdDMLogFatal ( "Command alien-token-init was not found at compilation time !!!" );
        return false;
    }
    string cmd;
    string redirectTo;

//    if ( !fCurrentGlobalLogFileName.empty() ) {
//        redirectTo.append ( ">> " );
//        redirectTo.append ( fCurrentGlobalLogFileName.c_str() );
//        redirectTo.append ( " 2>&1 " );
//    }

    cmd = fAliEnTokenInitCmd.c_str();
    cmd.append(" ");
    cmd.append(fAliEnUserName.c_str());
    cmd.append(" ");
//    cmd.append((  ? redirectTo.c_str() : "> /dev/null 2>&1" ));

//    XrdDMLogDebugFull ( "Sleeping 2 sec ..." );
    sleep ( 2 );
//    XrdDMLogDebugFull ( "Running %s",cmd.c_str() );

    if ( system ( cmd.c_str() ) ) {
//        XrdDMLogGError ( "alien-token-init returned non zero value !!!" );
//        XrdDMLogError ( "alien-token-init returned non zero value !!!" );
        return false;
    }
    return true;
}

bool PeacAliEnQuery::TokenDestroy () {
    if ( fAliEnTokenDestroyCmd.empty() ) {
//        XrdDMLogGFatal ( "Command alien-token-destroy was not found at compilation time !!!" );
//        XrdDMLogFatal ( "Command alien-token-destroy was not found at compilation time !!!" );
        return false;
    }

    string cmd;
    string redirectTo;

//    if ( !fCurrentGlobalLogFileName.empty() ) {
//        redirectTo.append ( ">> " );
//        redirectTo.append ( fCurrentGlobalLogFileName.c_str() );
//        redirectTo.append ( " 2>&1 " );
//    }

    cmd = fAliEnTokenDestroyCmd.c_str();
    cmd.append(" ");
//    cmd.append((  ? redirectTo.c_str() : "> /dev/null 2>&1" ));

//    XrdDMLogDebugFull ( "Running %s",cmd.c_str() );
    if ( system ( cmd.c_str() ) ) {
//        XrdDMLogGWarning ( "alien-token-destroy returned non zero value !!!" );
//        XrdDMLogWarning ( "alien-token-destroy returned non zero value !!!" );
        return false;
    }
    return true;
}


bool PeacAliEnQuery::IsProxyValid() {
	//  if ( fXrdgsiproxyCmd.empty() ) {
	//    XrdDMLogGFatal ( "Command xrdgsiproxy was not found at compilation time !!!" );
	//    XrdDMLogFatal ( "Command xrdgsiproxy was not found at compilation time !!!" );
	//    return false;
	//  }
	//
	string cmd;
	string redirectTo;
	//  if ( !fCurrentGlobalLogFileName.empty() ) {
	//    redirectTo.append ( ">> " );
	//    redirectTo.append ( fCurrentGlobalLogFileName.c_str() );
	//    redirectTo.append ( " 2>&1 " );
	//  }

	cmd = fXrdgsiproxyCmd.c_str();
	cmd.append(" -e -valid ");
	cmd.append(fRenewCertificateTime.c_str());

//	cmd.append((  ? redirectTo.c_str() : " > /dev/null 2>&1" ));

	//    XrdDMLogDebugFull ( "Running %s",cmd.c_str() );
	if ( system ( cmd.c_str() ) ) {
		//      XrdDMLogGError ( "xrdgsiproxy returned non zero value !!!" );
		//      XrdDMLogError ( "xrdgsiproxy returned non zero value !!!" );
		return false;
	}

	return true;
}

bool PeacAliEnQuery::ProxyInit () {

	if ( fXrdgsiproxyCmd.empty() ) {
		//        XrdDMLogGFatal ( "Command xrdgsiproxy was not found at compilation time !!!" );
		//        XrdDMLogFatal ( "Command xrdgsiproxy was not found at compilation time !!!" );
		return false;
	}

	string cmd;
	string redirectTo;
	//    if ( !fCurrentGlobalLogFileName.empty() ) {
	//        redirectTo.append ( ">> " );
	//        redirectTo.append ( fCurrentGlobalLogFileName.c_str() );
	//        redirectTo.append ( " 2>&1 " );
	//    }

	cmd = fXrdgsiproxyCmd.c_str();
	cmd.append(" init" );

	if ( !fCertificateDir.empty() ) {
		cmd.append(" -cert " );
		cmd.append(fCertificateDir.c_str() );
		cmd.append("/usercert.pem" );
		cmd.append(" -key " );
		cmd.append(fCertificateDir.c_str() );
		cmd.append("/userkey.pem" );
	}

//	cmd.append((  ? redirectTo.c_str() : " > /dev/null 2>&1" ));

	//	XrdDMLogDebugFull ( "Running %s",cmd.c_str() );
	//	XrdDMLogGInfo ( "Running %s",cmd.c_str() );
	if ( system ( cmd.c_str() ) ) {
		//		XrdDMLogGError ( "xrdgsiproxy returned non zero value !!!" );
		//		XrdDMLogError ( "xrdgsiproxy returned non zero value !!!" );
		return false;
	}

	return true;
}

//int PeacAliEnQuery::DoSetAliEnVariables() {
//
//
//    XrdDMLogDebugFull ( "DoSetAliEnVariables()" );
//    string filename ( "/tmp/gclient_env_" );
//    char uid[10];
//    sprintf ( uid, "%u", getuid() );
//    filename.append ( uid );
//    XrdDMLogDebugFull ( "DoSetAliEnVariables %s",filename.c_str() );
//
//    fstream file ( filename.c_str(), ios::in );
//    if ( !file.is_open() ) {
//        XrdDMLogGWarning ( "File %s could not be opened !!! Probably alien-token was destroyed ...", filename.c_str() );
//        XrdDMLogWarning ( "File %s could not be opened !!! Probably alien-token was destroyed ...", filename.c_str() );
//        return -1;
//    }
//    size_t found;
//    string str="", var="", val="";
//    while ( file >> str ) {
//
//        found = str.find ( "export" );
//        if ( found != string::npos ) {
//            file >> str;
//            found = str.find_first_of ( "=" );
//            var = str.substr ( 0, found );
//            val = str.substr ( found + 1, str.length() );
//            found = val.find ( "\"" );
//            if ( found != string::npos ) {
//                val.replace ( 0, 1, "" );
//                val.replace ( val.length() - 1, 1, "" );
//            }
//            found = var.find ( "LD_LIBRARY_PATH" );
//            if ( ! ( found != string::npos ) ) {
//                setenv ( var.c_str(), val.c_str(), 1 );
//            }
//            XrdDMLogDebugFull ( "DoSetAliEnVariables %s %s",var.c_str(), val.c_str() );
//        }
//
//    }
//    file.close();
////     system("export");
//    return 0;
//}
//
//void PeacAliEnQuery::CallAliEnTimeout ( int i ) {
//
//    alarm ( 0 );
//    throw 100;
//
//
//    if ( i ) return;
//}
//

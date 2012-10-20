//////////////////////////////////////////////////////////////////////////
//                                                                      //
// xrddm                                                                //
//                                                                      //
// Author: Martin Vala (SASKE Slovakia and JINR, Russia, 2010)          //
//                                                                      //
// A xrdcp-like command line tool for copying from alien                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef PeacAliEnQuery_h
#define PeacAliEnQuery_h

#include <string>
using namespace std;
#include <gapiUI.h>
#include <PeacAliEnConfig.h>

class PeacAliEnQuery {

public:
	PeacAliEnQuery();
    ~PeacAliEnQuery();

    void Print() const;
//
//    virtual int       DoAuthenticate ( bool showProcess );
//
//    virtual int       DoCopyPre ( string from, string to );
//    virtual int       DoCopyWithType ( EInputTypes_t type, string from, string to );
//    virtual int       DoCopyPost ( string from, string to );
//
//    void          SetCertificateDir ( string certDir ) { fCertificateDir = certDir; }
//
//
//protected:
//    virtual int       FillListOfFileNamesFrom ( string fname );
//    virtual int       SetConfigOptions ( string var, string val );
//    virtual EInputTypes_t   GetInputTypeId ( string inputTypeString = "std" );
//
private:
    string          fRenewCertificateTime;
    string          fCertificateDir;
    string          fAliEnUserName;
//
    string          fXrdgsiproxyCmd;
    string          fAliEnTokenInitCmd;
    string          fAliEnTokenDestroyCmd;
    string          fAliEnTokenInfoCmd;
//    string          fAliEnCpCmd;

//    unsigned int    fAliEnTimeout;
//
//    static void     CallAliEnTimeout(int i);
//
//    GapiUI          *fAliEn;
//    bool            fIsAliEnConnected;
//
//    // Authentication: check for token and proxy validity, and renew them
//    int           DoAliEnTokenInit ( bool showProcess );
//    bool          IsTokenValid(bool showProcess);
//    bool          TokenInit ( bool showProcess );
//    bool          TokenDestroy ( bool showProcess );
//    bool          IsProxyValid(bool showProcess);
//    bool          ProxyInit ( bool showProcess );
//
//    int           ConnectToAliEn();
//    int           DoSetAliEnVariables();
//
//    int           FillListOfFileNamesFromAliEnUsingWhereis ( string fname );
//
//    int           DoAliEnCp ( string from,string to );
};

#endif // PeacAliEnQuery_H
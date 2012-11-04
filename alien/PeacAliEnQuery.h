#ifndef PeacAliEnQuery_H
#define PeacAliEnQuery_H

#include <string>

#include <PeacAliEnConfig.h>

using namespace std;

class GapiUI;
class PeacAliEnQuery {

public:
	PeacAliEnQuery();
    ~PeacAliEnQuery();

    void Print() const;

    // xrdgsiproxy stuff
    bool          IsProxyValid();
    bool          ProxyInit ();

    // alien token stuff
    bool          IsTokenValid();
    bool          TokenInit ();
    bool          TokenDestroy ();
    int           DoAliEnTokenInit ();

    // lock function for alien-token-init (so it is done only once)
    int           TryGetLockAliEnToken();
    void          ReleaseLockAliEnToken( int fd);

    // alien stuff
    int           ConnectToAliEn();

    // Generating list of replicas from AliEn catalogue
    int           FillListOfFileNamesFromAliEnUsingWhereis ( string fname , vector<string> &paths);

    // setters
    void SetUser(const char* user) { mAliEnUserName = user; }

private:
    string          mRenewCertificateTime;
    string          mCertificateDir;
    string          mAliEnUserName;
    string          mXrdgsiproxyCmd;
    string          mAliEnTokenInitCmd;
    string          mAliEnTokenDestroyCmd;
    string          mAliEnTokenInfoCmd;
    string 			mAliEnTokenLockFileName;
    GapiUI         *mAliEn;
    bool            mIsAliEnConnected;
};

#endif // PeacAliEnQuery_H

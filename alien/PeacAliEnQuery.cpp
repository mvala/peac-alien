#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>

#include <gapiUI.h>

#include "PeacAliEnQuery.h"

PeacAliEnQuery::PeacAliEnQuery() :
mRenewCertificateTime("1:00"),
mCertificateDir(""),
mAliEnUserName("proof"),
mXrdgsiproxyCmd(ALIEN_XRDGSIPROXY),
mAliEnTokenInitCmd(ALIEN_ALIEN_TOKEN_INIT),
mAliEnTokenDestroyCmd(ALIEN_ALIEN_TOKEN_DESTROY),
mAliEnTokenInfoCmd(ALIEN_ALIEN_TOKEN_INFO),
mAliEn(0),
mIsAliEnConnected(false),
mAliEnTokenLockFileName("/tmp/alien-query-token.lock")
//
{
}

PeacAliEnQuery::~PeacAliEnQuery() {
}

void PeacAliEnQuery::Print() const {

	printf("AliEnQuery Info:\n");
	printf("\t%20s = %-20s\n", "RenewCertificateTime",mRenewCertificateTime.data());
	printf("\t%20s = %-20s\n", "CertificateDir", mCertificateDir.data());
	printf("\t%20s = %-20s\n", "AliEnUserName", mAliEnUserName.data());
	printf("\t%20s = %-20s\n", "XrdgsiproxyCmd", mXrdgsiproxyCmd.data());
	printf("\t%20s = %-20s\n", "AliEnTokenDestroyCmd", mAliEnTokenDestroyCmd.data());
	printf("\t%20s = %-20s\n", "AliEnTokenInfoCmd", mAliEnTokenInfoCmd.data());
	//	printf("\t%20s = %-20s\n","AliEnCpCmd",mAliEnCpCmd.data());
}
int PeacAliEnQuery::FillListOfFileNamesFromAliEnUsingWhereis(string fname,
		vector<string> &paths) {

	if (!mAliEn) {
		return -11;
	}
	if (!mIsAliEnConnected) {
		return -12;
	}
	string alienfilename = fname.c_str();
	string cmd("whereis ");
	cmd.append(alienfilename);
	bool retValWhereis = true;
	retValWhereis = mAliEn->Command(cmd.c_str());
	int numberOfReplicas = mAliEn->GetStreamColumns(2);
	if (numberOfReplicas < 1) {
		return -13;
	}

	string currFileStr, grpath;
	for (int i = 0; i < numberOfReplicas; i++) {
		currFileStr = mAliEn->GetStreamFieldValue(2, i, 2);
		if (!currFileStr.compare(0, 7, "guid://")) {
			continue;
		}
		paths.push_back(currFileStr);
	}
	return 0;
}

int PeacAliEnQuery::ConnectToAliEn() {

	if (mIsAliEnConnected) {
		return 0;
	}

	// getting AliEn Api
	if (!mAliEn) mAliEn = GapiUI::MakeGapiUI(true);

	if (!mAliEn) {
		mIsAliEnConnected = false;
		return 1;
	}
	string alienHost, alienPort, alienUser;
	mAliEn->Connect("", 0, mAliEnUserName.c_str(), 0);
	if (!mAliEn->Connected()) {
		mIsAliEnConnected = false;
		return 1;
	}
	mIsAliEnConnected = true;
	return 0;
}
int PeacAliEnQuery::DoAliEnTokenInit() {

	if (!IsProxyValid()) {
		remove(mAliEnTokenLockFileName.data());
		if (!ProxyInit()) {
			printf("XrdGsiProxy init didn't finish OK !!!\n");
			return 1;
		}
	}
	if (!IsTokenValid()) {
		int id_lock = TryGetLockAliEnToken();
		int timeOut = 120;
		int timeOutCounter = 0;
		while (id_lock < 0) {
			if (IsTokenValid()) return 0;
			id_lock = TryGetLockAliEnToken();
			sleep(1);
			timeOutCounter++;
		    if (timeOutCounter>=timeOut) return 3;
		}

		if (!TokenInit()) {
			printf("Error : alien-token-init error!!!\n");
		}
		if (!IsTokenValid()) {
			printf("Error: AliEn token is not valid!!!\n");
			ReleaseLockAliEnToken(id_lock);
			return 2;
		}
		ReleaseLockAliEnToken(id_lock);
	}

	return 0;
}

bool PeacAliEnQuery::IsTokenValid() {

	if (mAliEnTokenInfoCmd.empty()) {
		return false;
	}

	string cmd;
	string redirectTo;
	cmd = mAliEnTokenInfoCmd.c_str();
	cmd.append(" ");
	cmd.append(mAliEnUserName.c_str());
	cmd.append(" > /dev/null 2>&1");
	if (system(cmd.c_str())) {
		return false;
	}
	return true;
}

bool PeacAliEnQuery::TokenInit() {

	if (mAliEnTokenInitCmd.empty()) {
		return false;
	}
	string cmd;
	string redirectTo;
	cmd = mAliEnTokenInitCmd.c_str();
	cmd.append(" ");
	cmd.append(mAliEnUserName.c_str());
	cmd.append(" > /dev/null 2>&1");

	sleep(2);

	if (system(cmd.c_str())) {
		return false;
	}
	return true;
}

bool PeacAliEnQuery::TokenDestroy() {
	if (mAliEnTokenDestroyCmd.empty()) {
		return false;
	}

	string cmd;
	string redirectTo;
	cmd = mAliEnTokenDestroyCmd.c_str();
	cmd.append(" > /dev/null 2>&1");

	if (system(cmd.c_str())) {
		return false;
	}
	return true;
}

bool PeacAliEnQuery::IsProxyValid() {
	string cmd;
	string redirectTo;
	cmd = mXrdgsiproxyCmd.c_str();
	cmd.append(" -e -valid ");
	cmd.append(mRenewCertificateTime.c_str());
	cmd.append(" > /dev/null 2>&1");

	if (system(cmd.c_str())) {
		return false;
	}

	return true;
}

bool PeacAliEnQuery::ProxyInit() {

	if (mXrdgsiproxyCmd.empty()) {
		return false;
	}

	string cmd;
	string redirectTo;
	cmd = mXrdgsiproxyCmd.c_str();
	cmd.append(" init");

	if (!mCertificateDir.empty()) {
		cmd.append(" -cert ");
		cmd.append(mCertificateDir.c_str());
		cmd.append("/usercert.pem");
		cmd.append(" -key ");
		cmd.append(mCertificateDir.c_str());
		cmd.append("/userkey.pem");
	}
	cmd.append(" > /dev/null 2>&1");

	if (system(cmd.c_str())) {
		return false;
	}

	return true;
}

int PeacAliEnQuery::TryGetLockAliEnToken() {
	mode_t m = umask(0);
	int fd = open(mAliEnTokenLockFileName.data(), O_RDWR | O_CREAT, 0666);
	umask(m);
	if (fd >= 0 && flock(fd, LOCK_EX | LOCK_NB) < 0) {
		close(fd);
		fd = -1;
	}
	return fd;
}

void PeacAliEnQuery::ReleaseLockAliEnToken(int fd) {
	if (fd < 0)
		return;
	remove(mAliEnTokenLockFileName.data());
	close(fd);
}

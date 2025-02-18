/*
 * This software was developed at the National Institute of Standards and
 * Technology (NIST) by employees of the Federal Government in the course
 * of their official duties. Pursuant to title 17 Section 105 of the
 * United States Code, this software is not subject to copyright protection
 * and is in the public domain. NIST assumes no responsibility whatsoever for
 * its use by other parties, and makes no guarantees, expressed or implied,
 * about its quality, reliability, or any other characteristic.
 */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>

#include <unistd.h>

#include <be_error_signal_manager.h>

using namespace std;
using namespace BiometricEvaluation;

int main(int argc, char *argv[])
{
	
	/*
	 * Test setting an invalid signal set.
	 */
	cout << "Creating SignalManager object with invalid signal SIGSTOP : ";
	sigset_t sigset;
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGSTOP);
	bool success = false;
	try {
		Error::SignalManager bsigmgr(sigset);
	} catch (Error::ParameterError) {
		cout << "success.\n";
		success = true;
	} catch (Error::StrategyError &e) {
		cout << "failed." << endl;
		cout << "Caught " << e.what() << ".\n";
		return (EXIT_FAILURE);
	}
	if (!success) {
		cout << "failed.\n";
		return (EXIT_FAILURE);
	}

	Error::SignalManager *sigmgr = nullptr;
	cout << "Creating a default SignalManager: ";
	try {
		sigmgr = new Error::SignalManager();
		cout << "success." << endl;
	} catch (Error::StrategyError &e) {
		cout << "failed." << endl;
		cout << "Caught " << e.what() << ".\n";
		return (EXIT_FAILURE);
	}
	std::unique_ptr<Error::SignalManager> asigmgr(sigmgr);

	/*
	 * Test setting an invalid signal set.
	 */
	cout << "Setting invalid signal SIGKILL in signal set: ";
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGKILL);
	success = false;
	try {
		asigmgr->setSignalSet(sigset);
	} catch (Error::ParameterError) {
		cout << "success.\n";
		success = true;
	}
	if (!success) {
		cout << "failed.\n";
	}

	/*
	 * Create one of the signals handled by default.
	 */
	cout << "Testing default signal handling: ";
	char *cptr = nullptr;
	BEGIN_SIGNAL_BLOCK(asigmgr, sigblock1);
	if (asigmgr->sigHandled()) {
		cout << "sigHandled is set prematurely; failure.";
		return (EXIT_FAILURE);
	}
	*cptr = 'a';
	cout << "You should not see this message.\n";
	return (EXIT_FAILURE);
	END_SIGNAL_BLOCK(asigmgr, sigblock1);
	if (asigmgr->sigHandled())
		cout << "success.\n";
	else
		cout << "Signal handled, but sigHandled flag not set.\n";

	/*
	 * Test filtering of specified signals.
	 */
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGUSR1);
	asigmgr->setSignalSet(sigset);
	for (int i = 1; i< 3; i++) {
		cout << "Testing catch of SIGUSR1: Test number " << i << ": ";
		BEGIN_SIGNAL_BLOCK(asigmgr, sigblock2);
		kill(getpid(), SIGUSR1);
		cout << "You should not see this message.\n";
		return (EXIT_FAILURE);
		END_SIGNAL_BLOCK(asigmgr, sigblock2);
		if (asigmgr->sigHandled()) {
			cout << "success.\n";
		} else {
			cout << "Signal handled, but sigHandled() is false.\n";
			return (EXIT_FAILURE);
		}
	}

	/*
	 * Last test.
	 */
	cout << "Testing send of SIGUSR1: Should see signal and end of process: ";
	fflush(stdout);
	kill(getpid(), SIGUSR1);

	cout << "Failed to allow signal through.\n";
	return (EXIT_FAILURE);
}


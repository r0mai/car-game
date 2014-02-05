
#include "RealTimeGameManager.hpp"
#include "AIGameManager.hpp"

int main(int argc, char ** /*argv*/ ) {
	if ( argc > 1 ) {
		car::AIGameManager manager;
		manager.run();
	} else {
		car::RealTimeGameManager manager;
		manager.run();
	}
}



#include "RealTimeGameManager.hpp"
#include "AIGameManager.hpp"

#include <cstdlib>
#include <ctime>

int main(int argc, char ** /*argv*/ ) {
	std::srand(std::time(0));
	if ( argc > 1 ) {
		car::AIGameManager manager;
		manager.run();
	} else {
		car::RealTimeGameManager manager;
		manager.run();
	}
}


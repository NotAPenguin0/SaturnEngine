#include "Engine.hpp"

int main() {
	Saturn::Engine::CreateInfo engine_create_info;
    Saturn::Application app = Saturn::Engine::initialize(engine_create_info);
}
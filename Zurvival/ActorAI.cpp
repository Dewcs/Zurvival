#include "ActorAI.h"


ActorAI::ActorAI()
{

}


ActorAI::~ActorAI()
{

}

void ActorAI::setBrain(Brain *brain) {
	if (ia != NULL) delete ia;
	ia = brain;
}

void ActorAI::save(const char * fname) {
	ia->store(fname);
}

void ActorAI::think() {
	ia->setInput(input); // insert input 
	ia->evaluate(); // eval brain
	ia->getResult(output); // collect output
}

void ActorAI::addDamageDealt(double damage) {
	damageDealt += damage;
}

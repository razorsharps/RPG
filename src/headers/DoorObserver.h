#pragma once

#include <iostream>
#include "Observer.h"

class DoorObserver: public Observer {
  public:
    DoorObserver(Subject *mod, Sound* s) : Observer(mod, s) {
	}

    void update() {
		s->playSound();
    }
};

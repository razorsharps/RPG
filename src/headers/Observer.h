#pragma once

#include <vector>

class Subject {
  public:
    std::vector < class Observer * > views;
    const char* filename;

    void attach(Observer *obs) {
        views.push_back(obs);
    }
    void setVal() {
        notify();
    }

    const char* getFileName() {
        return filename;
    }

    void notify();
};

class Observer {
  public:
	Subject *model;
	const char* filename;
	Sound* s;

    Observer(Subject *mod, Sound * sound) {
        model = mod;
        model->attach(this);
		s = sound;
    }

    virtual void update() = 0;
  protected:
    Subject *getSubject() {
        return model;
    }

	const char* getFileName() {
		return filename;
	}
};

void Subject::notify() {
   views[0]->update();
}
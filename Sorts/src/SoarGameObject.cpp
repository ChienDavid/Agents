#include"SoarGameObject.h"
#include"SoarGameGroup.h"
#include<iostream>
#include<assert.h>
#include<string>

#include "MoveFSM.h"
#include "MineFSM.h"

void SoarGameObject::identifyBehaviors() {
  string name = gob->bp_name();
  if (friendly && name == "worker") {
    FSM* moveBehavior = new MoveFSM();
    FSM* mineBehavior = new MineFSM();
    registerBehavior(moveBehavior);
    registerBehavior(mineBehavior);
    cout << "FRIENDLY WORKER" << endl;
  }
}

SoarGameObject::SoarGameObject(GameObj *g, bool _friendly, bool _world)
: gob(g), friendly(_friendly), world(_world)
{
  status = OBJ_IDLE;
  identifyBehaviors();
}

SoarGameObject::~SoarGameObject()
{
  while(!memory.empty()) {
    memory.pop();
  }

  for(map<SoarActionType, FSM*>::iterator i = behaviors.begin(); 
      i != behaviors.end(); i++) 
  {
    delete i->second;
  }
}

void SoarGameObject::registerBehavior(FSM *b)
{
  assert (behaviors.find(b->name) == behaviors.end());
  b->setGameObject(gob);
  behaviors[b->name] = b;
}


void SoarGameObject::removeBehavior(SoarActionType name)
{
  map<SoarActionType, FSM*>::iterator i = behaviors.find(name);
  if (i != behaviors.end()) {
    delete i->second;
    behaviors.erase(i);
  }
}


//template<class T>
void SoarGameObject::issueCommand(SoarActionType cmd, Vector<sint4> prms)
{
  //Whether we really want this is up for analysis
  while(!memory.empty())
    memory.pop();

  map<SoarActionType, FSM*>::iterator i = behaviors.find(cmd);
  assert(i != behaviors.end());

  i->second->init(prms);
  memory.push(i->second);
  update();
  //currentCommand = cmd;
}


void SoarGameObject::update()
{
  int fsmStatus;
  group->setStale();
  if(!memory.empty())
  {
    fsmStatus = memory.top()->update();
    if(fsmStatus != FSM_RUNNING)
    {
      memory.pop();
    //  if(memory.empty())
        //currentCommand = SA_IDLE;
    }

    if (fsmStatus == FSM_SUCCESS) {
      status = OBJ_SUCCESS;
    }
    else if (fsmStatus == FSM_FAILURE) {
      status = OBJ_FAILURE;
    }
    else {
      status = OBJ_RUNNING;
    }
  }
}


void SoarGameObject::setGroup(SoarGameGroup *g)
{
 group = g;
}

SoarGameGroup *SoarGameObject::getGroup(void)
{
 return group;
}

/*SoarActionType SoarGameObject::getCurrentCommand()
{
 return currentCommand;
}*/

int SoarGameObject::getStatus()
{
  return status;
}


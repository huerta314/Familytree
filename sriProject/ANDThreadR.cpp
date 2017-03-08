#include "ANDThreadR.h"
#include <iostream>

using namespace std;

//Constructor which just passes the thread routine to the thread base class
ANDThreadR::ANDThreadR(void *(*_threadRoutine) (void *), void * args):Thread(_threadRoutine), andArgsR(args){
    //pthread_mutex_init(&datamutex,NULL);
}

ANDThreadR::~ANDThreadR(){}


void * ANDThreadR::threadMainBody(void * args){
    //Constructs a second query object with the name of the second half of the rule and the matching parameter between
    //both subrules and replaces it with the result of the left half of the expression. Then queries it into the rule/fact base
    Query constructedSecondParam;
    constructedSecondParam.name = andArgsR.originalQuery.ruleParamName[1];
    constructedSecondParam.parameters = andArgsR.originalQuery.parameters;
    constructedSecondParam.parameters[0] = andArgsR.inputQ.parameters[1];

    constructedSecondParam.flag = 1;
    deque<Query> tempOutput;
    /* Not needed anymore
    //
    for(int j = 0; j < andArgsR.originalQuery.ruleParams[1].size(); j++){
        auto it = andArgsR.varmap->find(andArgsR.originalQuery.ruleParams[1][j]);
        if(it != andArgsR.varmap->end()){
            constructedSecondParam.parameters[j] = andArgsR.inputQ.parameters[ andArgsR.varmap->operator[]( andArgsR.originalQuery.ruleParams[1][j] ) .origParam ];
        }
    }
    constructedSecondParam.parameters[1] = andArgsR.originalQuery.parameters[1];
    */
    if(andArgsR.rbPtr->setSecondIdent( constructedSecondParam, constructedSecondParam.name)){
        andArgsR.rbPtr->QueryRule(constructedSecondParam, tempOutput, *(andArgsR.kbPtr) );
    }
    else
        andArgsR.kbPtr->QueryFact(constructedSecondParam, tempOutput);
    //Anything retrieved from tempOutput is the result so we loop through it and
    //insert into the result deque which will be the result of the entire query.
    //Do not want all the threads to insert at the same time so lock this section
    //is locked by a shared mutex.
    pthread_mutex_lock(andArgsR.datamutex);
    int count = tempOutput.size();
    for(int i = 0; i < count; i++){
        Query q;
        q.name = andArgsR.originalQuery.name;
        q.parameters.push_back(andArgsR.inputQ.parameters[0]);
        q.parameters.push_back(tempOutput[i].parameters[1]);

        
        andArgsR.result->push_back(q);
        
    }
    pthread_mutex_unlock(andArgsR.datamutex);
    pthread_mutex_unlock(&mutex);
}
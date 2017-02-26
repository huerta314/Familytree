/* RuleBase.h */
//Rulebase which holds the rulebase datastructure for all the ruels
#ifndef RULEBASE_H_
#define RULEBASE_H_

#include <map>
#include <deque>
#include <string>

#include "Query.h"
#include "KnowledgeBase.h"

using std::map;
using std::string;
using std::vector;

class RuleBase{
	friend class Dumper;

private:
	map<string, deque<Query> > ruleContainer;	//A map with the Rule name as the key and a deque of query objects to hold all the paramters that the rule has

public:
	int 	AddRule(Query query);									//Adds a rule into the database
	int 	RemoveRule(Query query);								//Removes a rule by name
	int 	QueryRule(Query query,deque<Query>& output,KnowledgeBase kb);//Deduce aliases to facts and logical operations
	
	bool	doesRuleExist(Query query);								//Check if the rule exists
	bool	doesRuleExist(Query& query, string name);				//Check if the rule exists and passes the existing rule to the query
	Query	createFactQuery(string name, deque<string> parameters);	//Creates a fact inference to search the knowledge base with
	RuleBase();
	~RuleBase();
};
#endif

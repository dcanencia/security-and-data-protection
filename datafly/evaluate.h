#ifndef _EVALUATE_H
#define _EVALUATE_H

using namespace std;

#include <vector>
#include <tuple>
#include "../utilities/frequencies.h"
#include "../utilities/hierarchy.h"
#include "../utilities/tree.h"
#include "generalize.h"

tuple<vector<vector<string>>, vector<vector<vector<string>>>> datafly(
                                vector<vector<string>> dataset,
								map<int, vector<vector<string>>> hierarchies,
								vector<int> qids, const int K);

#endif

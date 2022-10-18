#include "node.h"

Node::Node (int id, vector<int> data) {
	this->id = id;
	this->data = data;
}

int Node::getId() const {
	return this->id;
}

int Node::getData(int i) const {
	return this->data[i];
}

bool Node::isEqual (const vector<int>& node) const {
	for (int i=0; i < (int)node.size(); i++) {
		if (this->data[i] != node[i])
			return false;
	}
	return true;
}

bool Node::isChild (Node node) {
	/*int sum1 = node.getSum();
	int sum2 = getSum();

	if (sum1 == 1 + sum2)
		return true;
	return false;	*/

	int sum=0, flag=0;
	for (int i=0; i < int(this->data.size()); i++) {
		if (this->data[i] == node.getData(i) - 1) {
			sum = sum + 1;
		}
		if (abs(node.getData(i) - this->data[i]) == 1) {
			flag += 1;
		}

		if (abs(node.getData(i) - this->data[i]) >= 2) {
			return false;
		}
	}

	if (sum == 1 && flag == 1)
		return true;
	return false;
}

bool Node::operator < (const Node& node) const {
        return (getSum() < node.getSum());
}

int Node::getSum() const {
	int sum = 0;
	for (const auto& entry : this->data)
		sum += entry;
	return sum;
}

bool Node::marked() const {
	return this->nodeMark;
}

void Node::mark() {
	this->nodeMark = true;
}

void Node::setKAnon() {
	this->kAnon = true;
}

bool Node::isKAnon() const {
	return this->kAnon;
}

string Node::generalizeEntry(string entry, const vector<vector<string>> hierarchy,
		       vector<string> generalizations) {
	int index = -1;

	for (int i=0; i < int(hierarchy.size()); i++) {
		for (int j=0; j < int(generalizations.size()); j++) {
			if (hierarchy[i][j].compare(entry) == 0) {
				index = j; break;
			}
		}
	}

	if (index == -1)
		return "No entry found";
	return generalizations[index];
}


vector<int> Node::evaluateFrequency(vector<vector<string>> generalizations,
	    		      int rows, int cols) {

	// Transpose generalizations matrix
	string genT[cols][rows];
	for (int i=0; i < rows; i++) {
		for (int j=0; j < cols; j++)
			genT[j][i] = generalizations[i][j];
	}


	// Concat values in every row
	vector<stringstream> values(cols);
	for (int i=0; i < cols; i++) {
		for (int j=0; j < rows; j++) {
			values[i] << genT[i][j];
		}
	}

	// Convert stringstream array to an array of string
	vector<string> vals;
	for (auto a = values.begin(); a != values.end(); a++) {
		vals.emplace_back((*a).str());
	}
	// Remove non unique values	
	sort(vals.begin(), vals.end());
	vals.erase(unique(vals.begin(), vals.end()), vals.end());
	// Count unique values
	vector<int> count(vals.size());
	// Check frecuencies in values array
	for (size_t i=0; i < vals.size(); i++) {
		for (int j=0; j < cols; j++) {
			if (vals[i] == values[j].str())
				count[i]++;
		}
	}

	return count;
}


bool Node::getKAnonymity(map<int, vector<vector<string>>> hierarchies,
			 vector<vector<string>> transposedTable,
			 vector<int> qids, int K) {

	vector<vector<string>> genArray(qids.size());

	// Iterate through every table row corresponding to node's
	// qids attributes
	int index = 0;
	for (const int& qid : qids) {

		// Node's data contains the level of generalization,
		// hierarchy level we should use
		//	this->data[index]
		vector<string> generalizations = hierarchies[qid][this->data[index]];

		// Table T rows representing qid's values
		for (const string& entry : transposedTable[qid]) {

			// Generate new row generalize

			// Iterate through possible generalizations in this
			// level and find the one that suits the original data.
			string choosenGen; 
			choosenGen = generalizeEntry(entry, hierarchies[qid], generalizations);
			genArray[index].emplace_back(choosenGen);
		}
		index++;
	}


	// Evaluate Frecuency set of T with respect to attributes of node (qids)
	vector<int> freq = evaluateFrequency(genArray, index, transposedTable[0].size());

	int flag = 1;
	for (const int& value : freq)
		if (value < K) flag = 0;
	if (!flag)
		return false;

	return true;
}

void Node::print() const {
	cout << "Id: " + to_string(id) + ", ";
	cout << "Data: ";
	for (const int& val : this->data)
		cout << to_string(val) + " ";
	cout << endl;
}

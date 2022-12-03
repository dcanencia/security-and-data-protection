#include "mondrian.h"

int main(int argc, char** argv) {

	if (argc != 2) {
		cout << "\nInvalid arguments.\n"
			"Use ./datafly.out [data directory]\n\n"
			"* Make sure your data directory meets the following structure:\n"
			"  (check dataset folder for an example)\n\n"
			" |-- [data directory]\n"
			"      |\n"
			"      |-- csv input file\n"
			"      |-- hierarchies\n"
			"           |\n"
			"           |-- hierarchy tables as csv files\n"
			"           |-- ....\n"
		<< endl;
		return -1;
	}

	// Read input
	// K
	string line;
	cout << "Insert K: ";
	getline(cin, line);
	const int K = stoi(line);

	// Qids
	/*set<string> qid_set;
	cout << "Number of qids: ";
	getline(cin, line);
	const int nqids = stoi(line);
	for (int i=0; i < nqids; i++) {
		cout << "Enter qid " << i << ": ";
		string qid;
		getline(cin, qid);
		qid_set.insert(qid);
	}
	if ((int)qid_set.size() != nqids) {
		cout << "Input Error: Qids should be unique.";
		cout << "Check if you repeated some of them" << endl; 
		return 1;
	}*/
	//vector<string> qidNames(qid_set.begin(), qid_set.end());
	vector<string> qidNames = {"Age", "Country", "Occupation"};

	// Weights
	/*vector<double> weights = {};
	cout << "Do you want to use weights (will only be used on analysis) [Y(y)/N(n)]: ";
	char answer;
	cin >> answer;
	bool keep;
	while(!keep) {
		switch(answer) {
			case 'Y':
			case 'y':
				for (int i=0; i < nqids; i++) {
					cout << "Weight for qid " << i << ": ";
					double weight;
					cin >> weight;
					weights.emplace_back(weight);
				}
				if (accumulate(weights.begin(), weights.end(),
					(float)0) != (float)1) {
					cout << "Input Error: Weights must sum 1" << endl;
					continue;
				}
				keep = true;
				break;
			case 'N':
			case 'n':
				keep = true;
				break;
			default:
				cout << "Do you want to use weights ";
				cout << "(will only be used on analysis) ";
				cout << "[Y(y)/N(n)]: ";
				cin >> answer;
		}
	}*/


	// Read data file and hierarchy folders
	vector<string> headers;
	vector<int> catQids, numQids, allQids;
	vector<int> isQidCat;
	vector<vector<string>> qids_dataset, dataset;
	map<int, vector<vector<string>>> hierarchies_map;

	try {
		hierarchies_map = read_directory(fs::path(argv[1]),
					dataset, headers, K, qidNames,
					catQids);
		sort(catQids.begin(), catQids.end());


		// Compare headers and qids
		allQids = getQidsHeaders(headers, qidNames);
		if (catQids.size() == 0 || allQids.size() < qidNames.size()) {
			cout << "An error occured.\nCheck the qid "
				"names entered exists. They should be "
				"referenced\nin their respectives "
				"hierarchy files." << endl << endl;
			return -1;
		}

		cout << "allQids: " << endl;
		for (const auto& a : allQids)
			cout << to_string(a) + ", ";
		cout << endl;
		cout << "catQids: " << endl;
		for (const auto& a : catQids)
			cout << to_string(a) + ", ";
		cout << endl;


		// Build a vector containing qid types
		for (const auto& qid : allQids) {
			if (find(catQids.begin(), catQids.end(), qid)
				!= catQids.end()) {
				isQidCat.emplace_back(1);
				continue;
			}
			isQidCat.emplace_back(0);
		}
	} catch (const char* e) {
		cout << e << endl; 
		return -1;
	}



	// *********************************
	// Main algorithm
	
	// Obtain a subset of dataset containing only qids
	for (size_t i=0; i < dataset.size(); i++) {
		vector<string> aux;
		for (const int& idx : allQids) {
			aux.emplace_back(dataset[i][idx]);
		}
		qids_dataset.emplace_back(aux);
	}

	// 1. Create a hierarchy tree for every qid
	map<int, Tree> trees;
	for (const int& i : catQids) {
		trees[i] = Tree(hierarchies_map[i]);
	}

	// 2. Anonymize whole initial partition
	vector<string> gens;
	for (size_t i=0; i < allQids.size(); i++) {
		if (isQidCat[i]) {
			gens.emplace_back(trees[i].root);
			continue;
		}
		gens.emplace_back("");
	}

	Partition partition(dataset, gens, allQids,
			    isQidCat, trees, K);
	vector<vector<string>> result = evaluate(partition);
	// End of main algorithm
	// *********************************


	// GCP Analysis
	// 1. Create equivalence classes or clusters
	//vector<vector<vector<string>>> clusters = createClusters(qids_dataset, qids);

	// 2. Especify weights, if any (Already entered by user)
	//vector<double> weights = {0.3, 0.4, 0.3};

	// 3. Precalculate NCP for every qid value included in every cluster
	// Count total number of distinct qid values in dataset
	/*vector<long double> cncps;
	int nweights = weights.size();
	for (const auto& cluster : clusters) {
		vector<vector<string>> tcluster = transpose(cluster);

		// Qids Att Values
		long double ncp = 0.0;

		for (size_t i=0; i < qids.size(); i++) {
			// Calculate NCP fot qid values 
			long double card = trees[i].getNCP(tcluster[i]);
			if (card == 1)
				continue;

			card /= trees[i].getNumLeaves();
			double weight = nweights > 0 ? weights[i] : 1;
			ncp += (long double)weight * card;
		}
		ncp *= nweights ? qids.size() : 1;
		cncps.emplace_back(ncp);
	}
	*/

	// 3. Calculate GCP
	//printAnalysis(clusters, dataset.size(), qids, cncps);

	return 0;
}

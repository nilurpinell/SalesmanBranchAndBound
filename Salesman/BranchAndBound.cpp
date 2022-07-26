#include "stdafx.h"
#include "CGraph.h"

#include <set>

int Numeronodes = 0;
class Solucio {
public:
	
	int visits;
	CVertex** camiParcial;
	bool* nodesVisitats;
	int nNodesVisitats;

	double cost;
	double heur;

	Solucio(int visits) {
		this->visits = visits;
		this->camiParcial = new CVertex*[visits];

		this->nNodesVisitats = 0;
		this->nodesVisitats = new bool[visits];

		memset(this->camiParcial, NULL, visits * sizeof(CVertex*));

		for (int i = 0; i < visits; i++) {
			this->nodesVisitats[i] = false;
		}

		//memset(this->nodesVisitats, 0, visits * sizeof(bool));

		this->cost = 0;
		this->heur = 0;
	}

	Solucio(Solucio &s) {
		this->visits = s.visits;
		this->camiParcial = new CVertex*[visits];

		this->nNodesVisitats = s.nNodesVisitats;
		this->nodesVisitats = new bool[visits];

		memcpy(this->camiParcial, s.camiParcial, visits * sizeof(CVertex*));
		memcpy(this->nodesVisitats, s.nodesVisitats, visits * sizeof(bool));

		this->cost = s.cost;
		this->heur = s.heur;

	}

	void addVertex(CVertex* v) {

		camiParcial[nNodesVisitats] = v;

		nNodesVisitats++;
		Numeronodes++;
		nodesVisitats[v->id] = true;

	}

	void addVertex(CVertex* v, double **matriu) {

		camiParcial[nNodesVisitats] = v;

		cost += matriu[camiParcial[nNodesVisitats-1]->id][camiParcial[nNodesVisitats]->id];

		nNodesVisitats++;
		Numeronodes++;
		nodesVisitats[v->id] = true;

	}

	~Solucio() {
		delete[] this->camiParcial;
		delete[] this->nodesVisitats;
	}

};

class comparator
{
public:
	bool operator() (const Solucio* n1, const Solucio* n2) const
	{
		return n1->heur < n2->heur;
	}
};

/*void heur(double** matrix, Solucio* sol) {

	sol->heur = 0;
}*/

void addAtTrack3(CTrack &track, CVertex* next, CVertex* current) {
	if (next == current)
		return;

	addAtTrack3(track, next->m_pDijkstraPrevious, current);

	track.AddLast(next);
}

class Problem {
public:
	CGraph *graph;
	int nVertex;
	CVertex** cvisits;
	bool circular;

	Solucio* initial;

	double **matriu;
	//int *minDistances = new int[nVertex];
	int **minDistances2D = new int*[nVertex];

	Problem(CGraph *graph, CVisits &visits): nVertex(visits.m_Vertices.size()) {
		this->graph = graph;

		cvisits = new CVertex*[nVertex];

		int i = 0;
		for (CVertex* v : visits.m_Vertices) {
			v->id = i;
			cvisits[i] = v;
			i++;
		}

		circular = visits.m_Vertices.front() == visits.m_Vertices.back();

		matriu = nullptr;
		//minDistances = nullptr;
		minDistances2D = nullptr;

		calcCostMatrix();
		calcMinDistances2D();

		buildInitialTrack();

	}

	void buildInitialTrack() {

		initial = new Solucio(nVertex);

		initial->addVertex(cvisits[0]);

		bool* nodesVisitats = new bool[nVertex];
		for (int i = 0; i < nVertex; i++) {
			nodesVisitats[i] = false;
		}

		nodesVisitats[0] = true;

		int lastVisit = 0;
		for (int i = 1; i < nVertex - 1; i++) {

			for (int j = 0; j < nVertex; j++) {

				int nextVisit = minDistances2D[lastVisit][j];

				if (nextVisit != nVertex - 1 && !nodesVisitats[nextVisit]) {
					initial->addVertex(cvisits[nextVisit], matriu);
					nodesVisitats[nextVisit] = true;
					lastVisit = nextVisit;
					break;
				}

			}

		}

		initial->addVertex(cvisits[nVertex-1], matriu);

		delete[] nodesVisitats;

	}

	void solve(CTrack &track) {

		std::priority_queue<Solucio*, vector<Solucio*>, comparator> queue;
		
		Solucio* s2 = new Solucio(nVertex);
		s2->addVertex(cvisits[0]);

		queue.push(s2);

		Solucio* b = initial;

		while (!queue.empty()) {

			Solucio* c = queue.top();
			queue.pop();

			//std::cout << "H: " << c->heur << std::endl;

			if (c->heur < b->cost) {

				for (int i = 1; i < nVertex - 1; i++) {
					if (!c->nodesVisitats[i]) {

						if (c->cost + matriu[c->camiParcial[c->nNodesVisitats-1]->id][i] < b->cost) {

							Solucio* s = new Solucio(*c);
							s->addVertex(cvisits[i], matriu);

							if (s->nNodesVisitats == nVertex - 1) {
								s->addVertex(cvisits[nVertex - 1], matriu);

								if (//b == nullptr ||
									s->cost < b->cost) {
									if (b == nullptr)
										delete b;
									b = s;
									//std::cout << "Better Solution: " << b->cost << std::endl;
								}
								else {
									delete s;
								}
							}
							else {
								heuristica(s);
								if (s->heur < b->cost) {								
									queue.push(s);
								}
								else {
									delete s;
								}

							}
							
						}

					}
				}

			}

			delete c;
		}

		if (b != nullptr) {

			CVertex* from = b->camiParcial[0];

			track.AddFirst(from);

			std::list<CVertex*>::iterator it;
			for (int i = 1; i < nVertex; i++) {
				CVertex* next = b->camiParcial[i];

				graph->DijkstraQueue(from);

				addAtTrack3(track, next, from);

				from = next;
			}

		}
		std::cout << "Nodes: " << Numeronodes << std::endl;
		Numeronodes = 0;

	}

	void calcCostMatrix() {
		matriu = new double*[nVertex];

		for (int i = 0; i < nVertex; i++) {
			CVertex* v = cvisits[i];

			matriu[v->id] = new double[nVertex];

			graph->DijkstraQueue(v);

			bool first = true;
			int minCost = std::numeric_limits<int>::max();
			for (int j = 0; j < nVertex; j++) {
				CVertex* v2 = cvisits[j];

				matriu[v->id][v2->id] = v2->m_DijkstraDistance;
			}

			//cvisits[v->id] = v;
		}
	}

	/*void calcMinDistances() {
		minDistances = new int[nVertex];

		for (int i = 0; i < nVertex; i++) {
			CVertex* v = cvisits[i];

			int minCost = std::numeric_limits<int>::max();
			for (int j = 0; j < nVertex; j++) {
				if (i == j)continue;

				CVertex* v2 = cvisits[j];

				int cost = matriu[v->id][v2->id];

				if (cost < minCost) {
					minCost = cost;
					minDistances[i] = j;
				}
			}

			cvisits[v->id] = v;
		}

		std::cout << "MINDISTANCES: ";
		for (int i = 0; i < nVertex; i++)
			cout << minDistances[i] << " ";
		std::cout << std::endl;

	}*/

	void calcMinDistances2D() {
		minDistances2D = new int*[nVertex];

		for (int i = 0; i < nVertex ; i++) {
			CVertex* v = cvisits[i];

			list<int> sorted;

			int minCost = std::numeric_limits<int>::max();
			for (int j = 0; j < nVertex; j++) {
				if (i == j)continue;

				CVertex* v2 = cvisits[j];
				
				int cost1 = matriu[v->id][v2->id];

				boolean inserted = false;
				for (std::list<int>::iterator it = sorted.begin(); !inserted && it != sorted.end(); it++) {
					int cost2 = matriu[v->id][(*it)];

					if (cost2 > cost1) {
						sorted.insert(it, j);
						inserted = true;
					}

				}

				if (!inserted) {
					sorted.push_back(j);
				}
				
			}

			minDistances2D[i] = new int[nVertex];

			int k = 0;
			for (std::list<int>::iterator it = sorted.begin(); it != sorted.end(); it++) {
				minDistances2D[i][k] = *it;
				k++;
			}

		}

	}

	virtual void heuristica(Solucio*) =0;

	~Problem() {
		delete cvisits;
		for (int i = 0; i < nVertex; i++) {
			delete[] matriu[i];
			delete[] minDistances2D[i];
		}
		delete[] matriu;
		delete[] minDistances2D;
	}

};

class ProblemH1 : public Problem {
public:
	ProblemH1(CGraph *graph, CVisits &visits) : Problem(graph, visits) {}

	void heuristica(Solucio* sol) {
		sol->heur = sol->cost;
	}

};

class ProblemH2 : public Problem {
public:
	ProblemH2(CGraph *graph, CVisits &visits) : Problem(graph, visits) {}

	void heuristica(Solucio* sol) {
		double costmin = 0;
		for (int i = 1; i < nVertex; i++) {
			if (!sol->nodesVisitats[i]) {
				costmin = costmin + matriu[i][minDistances2D[i][0]];
			}
		}
		// distancia entre un node i el node mes proper= matrix[i][minCosts[i]];
		sol->heur = sol->cost + costmin;
	}

};

class ProblemH3 : public Problem {
public:

	bool* nodesVisitats;

	ProblemH3(CGraph *graph, CVisits &visits) : Problem(graph, visits) {//, nodesVisitats(new bool[nVertex]) {
	
	}

	~ProblemH3() {
		//delete[] nodesVisitats;
	}

	void heuristica(Solucio* sol) {

		/*double costmin = 0;
		if (sol->nNodesVisitats != nVertex) {

			int quedan = 0;
			bool* nodesVisitats = new bool[nVertex];
			for (int i = 0; i < nVertex; i++) {
				nodesVisitats[i] = sol->nodesVisitats[i];
				if (!nodesVisitats[i])
					quedan++;
			}

			for (int i = 1; i < nVertex && quedan > 1; i++) {
				if (nodesVisitats[i]) {
					//costmin += matriu[i][minDistances2D[i][0]];
				}
				else {
					int j = 0;
					while (j < nVertex && nodesVisitats[minDistances2D[i][j]]) {
						j++;
					}
					costmin += matriu[i][minDistances2D[i][j]];

					nodesVisitats[i] = true;
					nodesVisitats[minDistances2D[i][j]] = true;

					quedan -= 2;

				}
			}
			// distancia entre un node i el node mes proper= matrix[i][minCosts[i]];
		}

		sol->heur = sol->cost + costmin;*/

		/*for (int i = 0; i < nVertex; i++) {
			nodesVisitats[i] = sol->nodesVisitats[i];
		}

		double costmin = 0;
		for (int i = 1; i < nVertex; i++) {
			if (!nodesVisitats[i]) {

				int jVertex;
				int j = 0;
				while (j < nVertex-1 && nodesVisitats[jVertex = minDistances2D[i][j]]) {
					j++;
				}

				costmin += matriu[i][jVertex];
				nodesVisitats[i] = true;
				nodesVisitats[jVertex] = true;

			}
		}*/

		//delete[] nodesVisitats;

		// distancia entre un node i el node mes proper= matrix[i][minCosts[i]];
		
		double costmin = 0;
		for (int i = 1; i < nVertex - 1; i++) {
			if (!sol->nodesVisitats[i]) {
				costmin = costmin + matriu[i][minDistances2D[i][0]];
			}
		}

		// distancia entre un node i el node mes proper= matrix[i][minCosts[i]];
		sol->heur = sol->cost + costmin;

	}

};

// SalesmanTrackBranchAndBound1 ===================================================

CTrack CGraph::SalesmanTrackBranchAndBound1(CVisits &visits)
{
	ProblemH1 h1(this, visits);
	h1.calcCostMatrix();

	CTrack track(this);

	h1.solve(track);
	
	return track;
}

// SalesmanTrackBranchAndBound2 ===================================================

CTrack CGraph::SalesmanTrackBranchAndBound2(CVisits &visits)
{
	ProblemH2 h1(this, visits);
	h1.calcCostMatrix();

	CTrack track(this);

	h1.solve(track);

	return track;
}

// SalesmanTrackBranchAndBound3 ===================================================

CTrack CGraph::SalesmanTrackBranchAndBound3(CVisits &visits)
{
	ProblemH3 h1(this, visits);
	h1.calcCostMatrix();
	h1.calcMinDistances2D();

	CTrack track(this);

	h1.solve(track);

	return track;
}

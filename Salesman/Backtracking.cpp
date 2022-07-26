#include "stdafx.h"
#include "CGraph.h"


// SalesmanTrackBacktracking ===================================================

class BacktrackingNode {
public:
	CVertex* v;
	bool hardVisited = false;
	int softVisited = -1;
	bool first = false;
	bool intermediate = false;
	bool last = false;

	BacktrackingNode(){};
	BacktrackingNode(CVertex* v): v(v) {};

	void set(CVertex* v, int softVisited) {
		this->v = v;
		this->softVisited = softVisited;
	}

	CString toString() {
		CString cs;
		cs.Format("ID: %d - HV: %d, SV: %d | F: %d, I: %d, L: %d", v->id, hardVisited, softVisited, first, intermediate, last);
		return cs;
	}
};

bool recursiveBacktracking(CTrack &track, BacktrackingNode* nodes, BacktrackingNode* vertex, int remainingVisits, double currentDistance, double& optimalDistance, int depth) {
	
	//if(vertex->intermediate)
		//cout << "DEPTH: " << depth << " " << vertex->toString() << endl;

	bool returns = false;

	// Si solo queda una visita y hemos encontrado el nodo final
	if (remainingVisits == 1 && vertex->last) {

		// Actualizar track si es óptimo por ahora
		//if (currentDistance < optimalDistance) {
		// Siempre es optimo en este lugar

		//cout << currentDistance << endl;

			optimalDistance = currentDistance;
			track.Clear();
			returns = true;
		//}

	}
	else {

		// Variable para saber si hemos encontrado un nodo que podemos visitar
		bool isHasVisited = false;

		// Buscamos un nodo que podamos visitar
		if (vertex->intermediate && !vertex->hardVisited) {
			vertex->hardVisited = true;
			remainingVisits--;

			isHasVisited = true;
		}

		// Por cada vecino del nodo
		for (CVertex* v : vertex->v->m_Neighbords) {
			BacktrackingNode* bn = &nodes[v->id];

			// Si no está visitado en esta iteración
			if (bn->softVisited > remainingVisits) {
				
				double distance = (vertex->v->m_Point - bn->v->m_Point).Module() + currentDistance;
				
				// Si no se pasa de la distancia
				if (distance < optimalDistance) {

					// Lo visitamos
					int lastValue = bn->softVisited;
					bn->softVisited = remainingVisits;
					
					returns |= recursiveBacktracking(track, nodes, bn, remainingVisits, distance, optimalDistance, depth+1);

					bn->softVisited = lastValue;
				}
			}

			//if (returns)
			//	break;

		}

		// Desvisitar la visita si se ha visitado en esta llamada recursiva
		if (isHasVisited) {
			vertex->hardVisited = false;
			//remainingVisits++;
		}

	}

	// Add at first if it returns true
	if (returns) {
		track.AddFirst(vertex->v);

		/*cout << "[";
		for (CVertex* v : track.m_Vertices) {
			cout << v->id << ",";
		}

		cout << "]" << endl;*/
	}

	return returns;

}

CTrack CGraph::SalesmanTrackBacktracking(CVisits &visits)
{
	
	// Create BacktrackingNode for each vertex
	BacktrackingNode* nodes = new BacktrackingNode[m_Vertices.size()];

	int i = 0;
	for (CVertex &v : m_Vertices) {
		v.id = i;
		nodes[i].set(&v, visits.m_Vertices.size());
		i++;
	}

	// Set boolean variables first, last and intermediate
	BacktrackingNode* firstNode = &nodes[visits.m_Vertices.front()->id];
	BacktrackingNode* lastNode = &nodes[visits.m_Vertices.back()->id];

	i = 0;
	for (CVertex *v : visits.m_Vertices) {

		if (i == 0)
			nodes[v->id].first = true;
		else if (i == visits.m_Vertices.size() - 1)
			nodes[v->id].last = true;
		else
			nodes[v->id].intermediate = true;

		i++;
	}

	/*for (i = 0; i < m_Vertices.size(); i++)
		cout << nodes[i].toString() << endl;*/

	CTrack track(this);

	double max = numeric_limits<double>::max();
	recursiveBacktracking(track, nodes, firstNode, visits.m_Vertices.size()-1, 0, max, 0);

	delete nodes;

	return track;
}

// SalesmanTrackBacktrackingGreedy =============================================

// Classe comparadora de distancies per a la cua
class comparator
{
public:
	double** matrix;
	CVertex* from;

	comparator(CVertex* from, double** matrix) : from(from), matrix(matrix) {};

	bool operator() (const CVertex* n1, const CVertex* n2) const
	{
		double dist1 = matrix[from->id][n1->id];
		double dist2 = matrix[from->id][n2->id];

		return dist1 > dist2;
	}
};

// Definicio d'una cua de prioritat per als nodes
typedef std::priority_queue<CVertex*, std::vector<CVertex*>, comparator> backtraking_queue;

bool recursiveBacktrackingGreedy(CTrack &track, double** matriu, int* minimums, int** orderindex, CVertex** visits, int nVisits,  CVertex* vertex, CVertex* last, int remainingVisits, double currentDistance, double& optimalDistance) {

	//if(vertex->intermediate)
	//cout << "DEPTH: " << depth << " " << vertex->toString() << endl;

	bool returns = false;

	// Si solo queda una visita y hemos encontrado el nodo final
	if (remainingVisits == 0) {

		double defDistance = currentDistance + matriu[vertex->id][last->id];

		// Actualizar track si es óptimo por ahora
		if (defDistance < optimalDistance) {

			//cout << defDistance << endl;

			optimalDistance = defDistance;
			track.Clear();
			returns = true;
		}

	}
	else {

		double distanceToFinal = matriu[vertex->id][minimums[vertex->id]] + currentDistance;

		if (distanceToFinal < optimalDistance) {

			//std::list<CVertex*>::iterator it;

			/*comparator cmp(vertex, matriu);
			backtraking_queue queue(cmp);

			for (it = visits.begin(); it != visits.end(); ++it) {
				if (!(*it)->m_Saved) {
					queue.push((*it));
				}
			}
			*/

			for (int i = 1; i < nVisits-1; i++) {
				CVertex* next = visits[i];// visits[orderindex[vertex->id][i]];

				if (next->m_Saved)
					continue;

				// Por cada vecino del nodo
				/*while (!queue.empty()) {

					CVertex* next = queue.top();
					queue.pop();*/

				double distance = matriu[vertex->id][next->id] + currentDistance;

				// Si no se pasa de la distancia
				if (distance < optimalDistance) {

					// Lo visitamos
					next->m_Saved = true;

					returns |= recursiveBacktrackingGreedy(track, matriu, minimums, orderindex, visits, nVisits, next, last, remainingVisits - 1, distance, optimalDistance);

					next->m_Saved = false;

				}


				//if (returns)
				//	break;

			}

		}

	}
	
	// Add at first if it returns true
	if (returns) {
		track.AddFirst(vertex);

		/*cout << "[";
		for (CVertex* v : track.m_Vertices) {
		cout << v->id << ",";
		}

		cout << "]" << endl;*/
	}

	return returns;

}

void addAtTrack2(CTrack &track, CVertex* next, CVertex* current) {
	if (next == current)
		return;

	//cout << i << endl;


	//if (next->m_pDijkstraPrevious == NULL)
	//	cout << next << " " << current << endl;
	//else
	addAtTrack2(track, next->m_pDijkstraPrevious, current);

	track.AddLast(next);
}

CTrack CGraph::SalesmanTrackBacktrackingGreedy(CVisits &visits)
{	
	int distance = visits.m_Vertices.size();

	CVertex** cvisits = new CVertex*[distance];

	bool circular = visits.m_Vertices.front() == visits.m_Vertices.back();

	//contemplar que el primer i el ultim node 
	int i = 0;
	for (CVertex *v : visits.m_Vertices) {
		if (circular && i == distance - 1)
			continue;
		v->id = i;
		v->m_Saved = false;
		cvisits[i] = v;
		i++;
	}

	visits.m_Vertices.front()->m_Saved = true;
	visits.m_Vertices.back()->m_Saved = true;

	double **matriu = new double*[distance];
	//int **orderindex = new int*[distance];
	int* minimums = new int[distance];

	for (i = 0; i < distance - 1; i++) {
		CVertex* v = cvisits[i];

		//cout << i << " " << v->id << endl;

		matriu[v->id] = new double[distance];
		//orderindex[v->id] = new int[distance];

		DijkstraQueue(v);

		bool first = true;
		int min = 0;
		for (CVertex *v2 : visits.m_Vertices) {
			matriu[v->id][v2->id] = v2->m_DijkstraDistance;
			if (first) {
				min = v2->m_DijkstraDistance;
				first = false;
			}
			else if(min > v2->m_DijkstraDistance){
				min = v2->m_DijkstraDistance;
			}
		}

		minimums[v->id] = min;

		/*comparator cmp(v, matriu);
		backtraking_queue queue(cmp);

		for (CVertex *v2 : visits.m_Vertices) {
			queue.push(v2);
		}

		for (int i = 0; i < distance; i++) {
			orderindex[v->id][i] = queue.top()->id;
			queue.pop();
		}*/

		cvisits[v->id] = v;
	}

	double max = numeric_limits<double>::max();
	CTrack track(this);

	recursiveBacktrackingGreedy(track, matriu, minimums, NULL, cvisits, distance, visits.m_Vertices.front(), visits.m_Vertices.back(), visits.m_Vertices.size() - 2, 0, max);

	track.AddLast(visits.m_Vertices.back());
	track.m_Vertices.pop_front();

	CTrack track2(this);

	CVertex* from = visits.m_Vertices.front();
	track2.AddFirst(from);

	std::list<CVertex*>::iterator it;
	for (it = track.m_Vertices.begin(); it != track.m_Vertices.end(); ++it) {
		CVertex* next = *it;

		DijkstraQueue(from);

		addAtTrack2(track2, next, from);

		from = next;
	}

	for (int i = 0; i < distance - 1; i++) {
		delete[] matriu[i];
		//delete[] orderindex[v->id];
	}
	delete[] matriu;
	//delete[] orderindex;

	delete[] cvisits;
	delete[] minimums;

	return track2;
}

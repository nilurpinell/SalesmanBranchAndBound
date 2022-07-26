#include "stdafx.h"
#include "CGraph.h"

// Calcs the distance between two points
double calcDistance(CGPoint &p1, CGPoint &p2) {
	return hypot(p1.m_X - p2.m_X, p1.m_Y - p2.m_Y);
	//return sqrt(pow(p1.m_X - p2.m_X, 2) + pow(p1.m_X - p2.m_X, 2));
}

// =============================================================================
// Dijkstra ====================================================================
// =============================================================================

void CGraph::Dijkstra(CVertex *pStart)
{

	// Initiate all vertex to an unknown distance
	for (CVertex &v : m_Vertices) {
		v.m_DijkstraVisit = false;
		v.m_DijkstraDistance = (&v == pStart) ? 0 : numeric_limits<double>::max();
	}

	CVertex *selected;

	do {

		selected = NULL;

		// Busqueda del seguent node
		for (CVertex &v : m_Vertices) {
			if (!v.m_DijkstraVisit && (selected == NULL || v.m_DijkstraDistance < selected->m_DijkstraDistance))
				selected = &v;
		}

		// Si s'ha trobat un node sense visitar
		if (selected != NULL) {
			selected->m_DijkstraVisit = true;

			// Actualitzar distancies dels veïns
			for (CVertex *n : selected->m_Neighbords) {
				double dist = selected->m_DijkstraDistance + calcDistance(selected->m_Point, n->m_Point);

				if (dist < n->m_DijkstraDistance) {
					n->m_DijkstraDistance = dist;
					n->m_pDijkstraPrevious = selected;
				}
			}

		}

	} while (selected != NULL);
}

// =============================================================================
// DijkstraQueue ===============================================================
// =============================================================================

// Classe Wrapper per a la desar la distancia en el moment de la insercció a la cua d'un vertex
class DijkstraQueueNode {
public:
	CVertex *v;
	double dist;

	DijkstraQueueNode(CVertex *v2) : v(v2), dist(v->m_DijkstraDistance) { /*soc un constructor!*/ };
};

// Classe comparadora de distancies per a la cua
class comparator
{
public:
	bool operator() (const DijkstraQueueNode* n1, const DijkstraQueueNode* n2) const
	{
		return n1->dist > n2->dist;
	}
};

// Definicio d'una cua de prioritat per als nodes
typedef std::priority_queue<DijkstraQueueNode*, std::vector<DijkstraQueueNode*>, comparator> dijkstra_queue;

void CGraph::DijkstraQueue(CVertex *pStart)
{

	dijkstra_queue queue;
	
	// Inicialitzacio dels nodes i de la cua
	for (CVertex &v : m_Vertices) {
		v.m_DijkstraVisit = false;
		v.m_DijkstraDistance = (&v == pStart) ? 0 : numeric_limits<double>::max();
		queue.push(new DijkstraQueueNode(&v));
	}

	DijkstraQueueNode *selectedNode = NULL;
	
	while (!queue.empty()) {

		// Obtenir el cap
		selectedNode = queue.top();
		queue.pop();

		// Processar-lo només si no està visitat
		if (!selectedNode->v->m_DijkstraVisit) {

			CVertex *selected = selectedNode->v;

			selected->m_DijkstraVisit = true;

			// Actualitzar distancies dels veïns
			for (CVertex *n : selected->m_Neighbords) {
				double dist = selected->m_DijkstraDistance + calcDistance(selected->m_Point, n->m_Point);

				if (dist < n->m_DijkstraDistance) {
					n->m_DijkstraDistance = dist;
					n->m_pDijkstraPrevious = selected;
					queue.push(new DijkstraQueueNode(n));

				}
			}

		}

		// Eliminar la instancia del node
		delete selectedNode;
	}


}


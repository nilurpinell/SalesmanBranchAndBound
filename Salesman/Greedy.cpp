#include "stdafx.h"
#include "CGraph.h"

// SalesmanTrackGreedy =========================================================

void addAtTrack(CTrack &track, CVertex* next, CVertex* current) {
	if (next == current)
		return;

	//cout << i << endl;


	//if (next->m_pDijkstraPrevious == NULL)
	//	cout << next << " " << current << endl;
	//else
		addAtTrack(track, next->m_pDijkstraPrevious, current);

	track.AddLast(next);
}

CTrack CGraph::SalesmanTrackGreedy(CVisits &visits) 
{
	CTrack track(this);

	CVertex* ini = visits.m_Vertices.front();
	CVertex* fin = visits.m_Vertices.back();

	CVertex* current = ini;
	visits.m_Vertices.pop_back();
	visits.m_Vertices.pop_front();
	
	track.AddFirst(current);

	while (!visits.m_Vertices.empty()) {

		visits.m_pGraph->DijkstraQueue(current);

		double v = numeric_limits<double>::max();
		CVertex* next = NULL;
		for (auto it = visits.m_Vertices.cbegin(); it != visits.m_Vertices.cend(); ++it) {
			if ((*it)->m_DijkstraDistance < v) {
				v = (*it)->m_DijkstraDistance;
				next = *it;
			}
		}

		addAtTrack(track, next, current);
		//cout << "FIN" << endl;
		visits.m_Vertices.remove(next);

		current = next;
	}

	visits.m_pGraph->DijkstraQueue(current);
	addAtTrack(track, fin, current);
	//cout << "FIN" << endl;

	
	return track;
}

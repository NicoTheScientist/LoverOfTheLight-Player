#include "Cupid.h"
#include "Reaper.h"
#include "Breeder.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>

Cupid::Cupid()
{
	type = cupid;
}

Cupid::~Cupid()
{
    
}

CupidGenome Cupid::getGenome() {
    return genome;
}

void Cupid::setGenome(CupidGenome g) {
    genome = g;
}

template <class T> void Cupid::selectForType(vector<T*> selectFrom, vector<T*> selectTo, bool(*compare)(Agent*, Agent*), double probability)
{
	selectTo.clear();
	int tournamentSize = genome.maxTournamentSize < selectFrom.size() ? genome.maxTournamentSize : selectFrom.size();
	vector<T*> selectedForTournament;
	for (int i = 0; i < (selectFrom.size() * 2); ++i) {
		if(((double)rand() / (RAND_MAX + 1.0)) < probability) {
			for (int j = 0; j < tournamentSize; ++j) {
				selectedForTournament.at(j) = selectFrom.at((int)((double)rand() / (RAND_MAX + 1.0) * (double)tournamentSize));
			}
			sort(selectedForTournament.begin(), selectedForTournament.end(), compare);
			selectTo.push_back(selectedForTournament.at(tournamentSize - 1));
		}
	}
	if (selectTo.size() % 2 == 1) {
		selectTo.push_back(selectTo.at((int)((double)rand() / (RAND_MAX + 1.0) * (double)(selectTo.size()))));
	}
}

void Cupid::performSelection()
{
	selectForType<CandidateSolution>(candidateSolutions, selectedCandidateSolutions, &Agent::compare, genome.probCS);
    selectForType<Breeder>(breeders, selectedBreeders, &Agent::compare, genome.probB);
    selectForType<Cupid>(cupids, selectedCupids, &Agent::compare, genome.probC);
    selectForType<Reaper>(reapers, selectedReapers, &Agent::compare, genome.probR);
}

Agent** Cupid::getParents()
{
	vector<AgentType> typesToChooseFrom;

	if (!selectedCandidateSolutions.empty()) {typesToChooseFrom.push_back(candidateSolution);}
	if (!selectedCupids.empty()) {typesToChooseFrom.push_back(cupid);}
	if (!selectedBreeders.empty()) {typesToChooseFrom.push_back(breeder);}
	if (!selectedReapers.empty()) {typesToChooseFrom.push_back(reaper);}

	if (typesToChooseFrom.empty())
	{
		return NULL;
	}

	AgentType chosenType = typesToChooseFrom.at((int)((double)rand() / (RAND_MAX + 1.0) * (double)typesToChooseFrom.size()));

	Agent** parents = new Agent*[2];

	switch(chosenType)
	{
	case candidateSolution:
		{
			parents[0] = selectedCandidateSolutions.at(selectedCandidateSolutions.size() - 1);
			selectedCandidateSolutions.pop_back();
			parents[1] = selectedCandidateSolutions.at(selectedCandidateSolutions.size() - 1);
			selectedCandidateSolutions.pop_back();
			break;
		}
	case cupid:
		{
			parents[0] = selectedCupids.at(selectedCupids.size() - 1);
			selectedCupids.pop_back();
			parents[1] = selectedCupids.at(selectedCupids.size() - 1);
			selectedCupids.pop_back();
			break;
		}
	case reaper:
		{
			parents[0] = selectedReapers.at(selectedReapers.size() - 1);
			selectedReapers.pop_back();
			parents[1] = selectedReapers.at(selectedReapers.size() - 1);
			selectedReapers.pop_back();
			break;
		}
	case breeder:
		{
			parents[0] = selectedBreeders.at(selectedBreeders.size() - 1);
			selectedBreeders.pop_back();
			parents[1] = selectedBreeders.at(selectedBreeders.size() - 1);
			selectedBreeders.pop_back();
			break;
		}
	default:
		{
			break;
		}
	}

	return parents;
}

Agent* Cupid::getEmptyAgent()
{
	if (empties.empty()) {
		return NULL;
	}
    else {
        int position = (int)((double)rand() / (RAND_MAX + 1.0) * (double)empties.size());
        Agent* emptyAgent = empties.at(position);
        empties.erase(empties.begin() + position);
        return emptyAgent;
    }
}

Breeder* Cupid::getRandomBreeder()
{
	if (breeders.empty())
	{
		return NULL;
	}
	return breeders.at((int)((double)rand() / (RAND_MAX + 1.0) * (double)breeders.size()));
}
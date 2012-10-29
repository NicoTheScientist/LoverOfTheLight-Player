#include "Reaper.h"
#include "Cupid.h"
#include "Breeder.h"
#include <algorithm>
#include <vector>

Reaper::Reaper()
{
	type = reaper;
}

Reaper::~Reaper()
{
    
}

ReaperGenome Reaper::getGenome() {
    return genome;
}

void Reaper::setGenome(ReaperGenome g) {
    genome = g;
}

template <class T> void Reaper::selectForType(vector<T*> selectFrom, vector<T*> selectTo, bool(*compare)(Agent*, Agent*), double probability)
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

void Reaper::performAction()
{
	selectForType<CandidateSolution>(candidateSolutions, selectedCandidateSolutions, &Agent::compare, genome.probCS);
    selectForType<Breeder>(breeders, selectedBreeders, &Agent::compare, genome.probB);
    selectForType<Cupid>(cupids, selectedCupids, &Agent::compare, genome.probC);
    selectForType<Reaper>(reapers, selectedReapers, &Agent::compare, genome.probR);
}

Agent* Reaper::getAgentToKill()
{
	Agent* toKill = NULL;

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

	switch(chosenType)
	{
	case candidateSolution:
		{
			toKill = (Agent*)selectedCandidateSolutions.at(selectedCandidateSolutions.size() - 1);
			selectedCandidateSolutions.pop_back();
			break;
		}
	case cupid:
		{
			toKill = (Agent*)selectedCupids.at(selectedCupids.size() - 1);
			selectedCupids.pop_back();
			break;
		}
	case reaper:
		{
			toKill = (Agent*)selectedReapers.at(selectedReapers.size() - 1);
			selectedReapers.pop_back();
			break;
		}
	case breeder:
		{
			toKill = (Agent*)selectedBreeders.at(selectedBreeders.size() - 1);
			selectedBreeders.pop_back();
			break;
		}
	default:
		{
			break;
		}
	}

	return toKill;
}
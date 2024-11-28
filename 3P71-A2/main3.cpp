#include <iostream>
#include "common.h"
#include "loader.h"
#include <algorithm>

using namespace std;

// Outline: Use GA to solve class scheduling problem
// 
// 
// Time slots are standardized 0 -> n, where each day is divided into n/5 slots. So a typical day from
//	8 am to 6 pm could have 10 slots, with the whole week having 50 slots, no classes on weekends.
// 
// Each room has a capacity, and each course has a number of students. A course can be taught in only one
//	room and each room can only have one course at a time.
// 
// Courses have a duration and so can take up multiple slots.
// 
// Each course has a professor, and each professor can only teach one course at a time. No considerations
//	are made for time to travel between rooms, professors have mastered teleportation.


#pragma region STRUCTURES

// Population size
const int POP_SIZE = 100;

// Max number of generations
const int MAX_GEN = 1000;


// Elite rate (what % of the population is carried over to the next generation)
const float ELITE_RATE = 0.0f;

// Actual number of elites
int ELITE_AMT = (int) (POP_SIZE * ELITE_RATE);


// Tournament size, % of remaining population after elites are removed
// To calculate actual tournament size do (POP_SIZE * TOURNAMENT_SIZE) - (POP_SIZE * ELITE_RATE)
const float TOURNAMENT_SIZE = 0.1f;

// Actual number of tournament participants
int TOURNAMENT_AMT = (int) (POP_SIZE * TOURNAMENT_SIZE) - ELITE_AMT;

// Tournament win rate (what % of the tournament wins are selected for breeding)
const float TOURNAMENT_WIN_RATE = 0.5f;

// Actual number of tournament winners
int TOURNAMENT_WIN_AMT = (int)(TOURNAMENT_AMT * TOURNAMENT_WIN_RATE);


// Uniform corssover so for now no need to define a crossover rate


// Mutation rate
const float MUT_RATE = 0.5f;

// Cross over rate
const float CROSS_RATE = 0.7f;

// Number of genes in a chromosome, equal to the number of courses
int chromosome_size;


// Structures

struct gene {
	int room_idx;
	int time_idx;
};

struct chromosome {
	// The index of any given gene is the same as the course index, save 4 byts of memory
	vector<gene> genes;
	float fitness;
};

// Population
vector<chromosome> population;

// Winners of selection
vector<chromosome> selected;

// Acceleration structures
vector<bool> timeslot_taken;
vector<int> room_taken;
vector<int> prof_taken;

#pragma endregion


#pragma region OPERATORS

// Compare function for sorting chromosomes by fitness, favoring higher fitness
bool compare_fitness(chromosome a, chromosome b) {
	return a.fitness < b.fitness;
}

// Void functions operate inline on the above structures

// population initializer
void init_pop() {

	// We want to create one gene for each course in the chromosome
	// The index of the gene is equal to the index of the course so we just need to iterate over courses
	chromosome_size = (int)courses.size();

	// Initialize the population with random genes
	for (int i = 0; i < POP_SIZE; i++) {
		chromosome c;
		for (int j = 0; j < chromosome_size; j++) {
			gene g;
			/*g.room_idx = rand() % rooms.size();
			g.time_idx = rand() % num_slots;*/
			
			g.room_idx = rand() % 3;
			g.time_idx = rand() % 3;

			c.genes.push_back(g);
		}
		c.fitness = -1;
		population.push_back(c);
	}
}

// Selection operator
// Since we are doing tournament selection we select random individuals so we only need to compute fitness
// on the selected individuals
void select() {

	// Clear the selected vector
	selected.clear();

	// Select random individuals for the tournament
	for (int i = 0; i < TOURNAMENT_AMT; i++) {
		int idx = rand() % POP_SIZE;
		selected.push_back(population[idx]);
	}

	// Sort the selected individuals by fitness
	sort(selected.begin(), selected.end(), compare_fitness);

	// Remove the least fit individuals
	selected.erase(selected.begin() + TOURNAMENT_WIN_AMT, selected.end());

}

// Crossover operator (uniform crossover), returns two children
pair<chromosome, chromosome> crossover(chromosome a, chromosome b) {

	chromosome c1;
	chromosome c2;

	for (int i = 0; i < chromosome_size; i++) {
		if (rand() % 2 == 0) {
			c1.genes.push_back(a.genes[i]);
			c2.genes.push_back(b.genes[i]);
		}
		else {
			c1.genes.push_back(b.genes[i]);
			c2.genes.push_back(a.genes[i]);
		}
	}

	return make_pair(c1, c2);
}

// Mutation operator
void mutate(chromosome& c) {

	for (int i = 0; i < chromosome_size; i++) {
		if (rand() / RAND_MAX <= MUT_RATE) {
			/*c.genes[i].room_idx = rand() % rooms.size();
			c.genes[i].time_idx = rand() % num_slots;*/
			
			c.genes[i].room_idx = rand() % 3;
			c.genes[i].time_idx = rand() % 3;
		}
	}

}

// Fitness function
float fitness(chromosome c) {
	// For now just maximize the 1s

	int conflicts = 0;
	
	// loop over every gene in the chromosome and check for conflicts
	for (int i = 0; i < chromosome_size; i++) {
		gene g = c.genes[i];
		int room = g.room_idx;
		int time = g.time_idx;

		// just maximize 1s for now
		if (room != 1) {
			conflicts++;
		}

		if (time != 1) {
			conflicts++;
		}

		
	}

	float max_conflicts = 2 * chromosome_size; // 2 checks per gene
	return (max_conflicts - conflicts) / max_conflicts;
}


#pragma endregion

int main() {

	cout << "Enter the directory of the data files (def: data/t1):" << "\n" << "> ";
	string input;
	
	// Use getline to accept no imput for default
	getline(cin, input);

	if (input.empty()) {
		input = "data/t1";
	}

	load(input);

	// The actual genetic algorithm
	init_pop();

	for (int generation = 0; generation < MAX_GEN; generation++) {

		// Selection
		select();

		// Clear the population
		population.clear();

		// Crossover until the population is full
		for (int i = 0; i < POP_SIZE; i += 2) {
			pair<chromosome, chromosome> children = crossover(selected[i % (TOURNAMENT_WIN_AMT - 1)], selected[(i % (TOURNAMENT_WIN_AMT - 1)) + 1]);
			population.push_back(children.first);
			population.push_back(children.second);
		}

		// Mutation
		for (int i = 0; i < POP_SIZE; i++) {
			mutate(population[i]);
		}

		// Fitness
		for (int i = 0; i < POP_SIZE; i++) {
			float temp_fit = fitness(population[i]);
			population[i].fitness = temp_fit;
		}

		// Sort the population by fitness
		sort(population.begin(), population.end(), compare_fitness);

		// Print the best individual
		cout << "Generation: " << generation << " Best fitness: " << population[0].fitness << "\n";
		cout << "Best individual: \n";
		for (int i = 0; i < chromosome_size; i++) {
			cout << population[0].genes[i].room_idx << "," << population[0].genes[i].time_idx << " ";
		}
		cout << "\n\n";

	}
	

}


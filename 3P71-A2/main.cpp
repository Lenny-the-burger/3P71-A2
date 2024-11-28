#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

#include "common.h"
#include "loader.h"

using namespace std;

bool AUTO_MODE = false;

// Population size, must be even
int POP_SIZE = 51;

// Max number of generations
int MAX_GEN = 1000;


// Elite rate
float ELITE_RATE = 0.02f;


// Tournament size, % of remaining population
float TOURNAMENT_SIZE = 0.6f;

// How many winners are selected from the tournament
float TOURNAMENT_WIN_RATE = 0.5f;


// Crossover rate
float CROSS_RATE = 0.90f;


// Mutation rate
float MUT_RATE = 0.10f;


// Chromosome size
int chromosome_size; // this is set

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


// Gene
struct gene {
	int course_idx;
	int room_idx;
	int timeslot;
};

struct chromosome {
	vector<gene> genes;
	float fitness;
	int multiverse_idx;
};

// This goes into every slot in the timetable
struct are_busy {
	vector<int> room_idx;
	vector<int> prof_idx;
};

// Write true to this if any gene uses the slot
// This is used by the ga to compute things on
// uyes i could just operate on this directly but its more complicated
vector<vector<are_busy>> pop_timetables;

// Build timetables for given chromosomes
vector<vector<are_busy>> build_timetable(vector<chromosome> &population) {

	vector<vector<are_busy>> timetables;

	for (int i = 0; i < population.size(); i++) {
		chromosome c = population[i];

		vector<are_busy> timetable;

		// Initialize timetable
		for (int j = 0; j < num_slots; j++) {
			are_busy ab;
			timetable.push_back(ab);
		}

		// Fill in timetable
		for (int j = 0; j < chromosome_size; j++) {
			gene g = c.genes[j];
			int duration = courses[g.course_idx].duration + 1;

			// Account for courses that take multiple slots
			for (int k = 0; k < duration; k++) {
				if (g.timeslot + k >= num_slots) {
					break;
				}
				are_busy ab = timetable[g.timeslot + k];

				ab.room_idx.push_back(g.room_idx);
				ab.prof_idx.push_back(courses[g.course_idx].prof);

				timetable[g.timeslot + k] = ab;
			}
		}

		timetables.push_back(timetable);
	}

	return timetables;
}


// Calculate fitness of chromosome
// Returns float fitness
float get_fitness(chromosome c) {
	// When checking for conflicts, we need to check for:
	// 1. One room can only be used by one course at a time
	// 2. One professor can only teach one course at a time
	// 3. Room must have enough capacity for the course

	// For it to be a conflict, the timeslot must contain at least two examples, so if only 
	// one room is mentioned in the isbusy, then we are the only ones using it

	int conflicts = 0;
	int max_conflicts = 1;

	// Check if room capacity is enough
	for (int i = 0; i < chromosome_size; i++) {
		gene g = c.genes[i];
		if (rooms[g.room_idx].capacity < courses[g.course_idx].students) {
			conflicts++;
		}
	}

	// Get the timetable we belong to from the multiverse index from any gene
	vector<are_busy> timetable = pop_timetables[c.multiverse_idx];
	// yes ik that this is stupid but im not rewriting all the operators to operate on the timetable
	// directly it would be a non standard nightmare, this is what you get to gradfe for giving us a 
	// gene where the fitness depends on the entire chromosome


	// Check for room and professor conflicts
	for (int i = 0; i < chromosome_size; i++) {
		gene g = c.genes[i];
		int durration = courses[g.course_idx].duration + 1;

		// the timeslot + duration of the course then we just add one conflict for each slot
		if (g.timeslot + durration > num_slots) {
			conflicts++;
			continue;
		}
		max_conflicts += durration - 1;

		int room = g.room_idx;
		int prof = courses[g.course_idx].prof;

		// Some courses take multiple slots so we need to check all slots
		for (int j = 0; j < durration; j++) {

			are_busy ab = timetable[g.timeslot + j];
			int temp = ab.room_idx.size();
			
			// Count the number of times conficts happen.
			int appearances = 0;
			for (int k = 0; k < ab.room_idx.size(); k++) {
				if (ab.room_idx[k] == room) {
					appearances++;
				}
			}
			if (appearances > 1) {
				conflicts++;
			}
			max_conflicts++;

			// do the same for the prof
			appearances = 0;
			for (int k = 0; k < ab.prof_idx.size(); k++) {
				if (ab.prof_idx[k] == prof) {
					appearances++;
				}
			}
			if (appearances > 1) {
				conflicts++;
			}
			max_conflicts++;
		}

	}

	float fitness = 1.0f / (float)(1 + conflicts);
	//float fitness = 1.0f - (float)conflicts / (float)max_conflicts;
	return fitness;
}

// Initialize population
// Output is the population
vector<chromosome> init_population() {
	vector<chromosome> population;

	for (int i = 0; i < POP_SIZE; i++) {
		chromosome c;
		c.multiverse_idx = i;
		for (int j = 0; j < chromosome_size; j++) {
			gene g;
			// course index is the same as gene index to prevent overlap
			g.room_idx = rand() % rooms.size();
			g.timeslot = rand() % num_slots;
			g.course_idx = j;
			c.genes.push_back(g);
		}

		population.push_back(c);
	}

	return population;

}

// Sort chromosomes by fitness
// Returns the sorted population
vector<chromosome> sort_population(vector<chromosome> population) {
	vector<chromosome> sorted_population = population;
	sort(sorted_population.begin(), sorted_population.end(), [](chromosome a, chromosome b) {
		return get_fitness(a) > get_fitness(b);
		});
	return sorted_population;
}

vector<chromosome> sort_population_cached(vector<chromosome> population) {
	vector<chromosome> sorted_population = population;
	sort(sorted_population.begin(), sorted_population.end(), [](chromosome a, chromosome b) {
		return a.fitness > b.fitness;
		});
	return sorted_population;
}


// Tournament selection
// Returns the winners of the tournament
// Input is the total population
chromosome tournament_selection(vector<chromosome> population) {

	vector<chromosome> competitors;

	// Randomly select TOURNAMENT_SIZE * POP_SIZE participants
	int tournament_size = (int)(TOURNAMENT_SIZE * POP_SIZE);

	vector<int> selected;
	for (int i = 0; i < tournament_size; i++) {
		// Randomly select a participant
		int idx = rand() % POP_SIZE;

		// Make sure the participant is not already selected
		while (find(selected.begin(), selected.end(), idx) != selected.end()) {
			idx = rand() % POP_SIZE;
		}

		chromosome participant = population[idx];
		competitors.push_back(participant);
		selected.push_back(idx);
	}

	// Sort the competitors by fitness
	competitors = sort_population_cached(competitors);
	
	chromosome winner = competitors[0];

	return winner;
}

// Crossover operator, uniform crossover
// Takes two parents and returns two children
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

// Crossover operator, one point crossover
// Takes two parents and returns two children
pair<chromosome, chromosome> crossover_one_point(chromosome a, chromosome b) {
	chromosome c1;
	chromosome c2;

	int crossover_point = rand() % chromosome_size;

	for (int i = 0; i < chromosome_size; i++) {
		if (i < crossover_point) {
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

// Mutation operator, mutate each gene with probability MUT_RATE
// Input is a chromosome
// Output is the mutated chromosome
chromosome mutate(chromosome c) {
	// re-randomize the room and timr slot for each gene with probability MUT_RATE
	for (int i = 0; i < chromosome_size; i++) {
		if ((float)rand() / RAND_MAX <= MUT_RATE) {
			c.genes[i].room_idx = rand() % rooms.size();
			c.genes[i].timeslot = rand() % num_slots;
		}
	}

	return c;
}

void print_chromosome(chromosome c) {
	
	// for now just print out all the information in order of timeslot in csv format
	vector<gene> ordered_genes = c.genes;

	// sort by the timeslot id
	sort(ordered_genes.begin(), ordered_genes.end(), [](gene a, gene b) {
		return a.timeslot < b.timeslot;
		});

	for (int i = 0; i < chromosome_size; i++) {
		gene g = ordered_genes[i];
		cout << courses[g.course_idx].name << "," << rooms[g.room_idx].name << "," << timetable_display[g.timeslot].day << "," << timetable_display[g.timeslot].time << endl;
	}

	// print out the data as the raw data
	for (int i = 0; i < chromosome_size; i++) {
		gene g = ordered_genes[i];
		cout << g.course_idx << "," << g.room_idx << "," << timetable_display[g.timeslot].day << "," << timetable_display[g.timeslot].time << endl;
	}

	cout << get_fitness(c) << endl;

}

void solution_found(chromosome c) {
	if (!AUTO_MODE) {
		cout << "Solution found!" << endl;
		cout << "Fitness: " << get_fitness(c) << endl;
		cout << "Chromosome: " << endl;
		print_chromosome(c);
		cout << endl;
	}
	else {
		// Auto mode will print results to a file
	}
}

// Main function
int main(int argc, char* argv[]) {

	// Argument order handling:
	// 1. Automatic mode, no user input, only accepts "-a"
	// 2. Data path
	// 3. Crossover rate, accepts decimal between 0 and 1
	// 3. Mutation rate, accepts decimal between 0 and 1
	// 4. Random seed, accepts uint

	// Automatic mode
	if (argc > 1 && string(argv[1]) == "-a") {
		AUTO_MODE = true;

		// Read data path arg
		string data_path = string(argv[2]);

		// Read crossover rate arg
		CROSS_RATE = stof(argv[3]);

		// Read mutation rate arg
		MUT_RATE = stof(argv[4]);

		// Read random seed arg
		srand(stoi(argv[5]));

		load(data_path);
		chromosome_size = courses.size();

	} else {
		
		// Random seed
		srand(1);

		cout << "Enter the directory of the data files (def: data/t1):" << "\n" << "> ";
		string input;

		// Use getline to accept no imput for default
		getline(cin, input);

		if (input.empty()) {
			input = "data/t1";
		}

		load(input);
		chromosome_size = courses.size();

	}

	// Initialize population
	vector<chromosome> population = init_population();

	int num_elites = (int)(ELITE_RATE * POP_SIZE);

	// Update the pop size to account for the elites
	//POP_SIZE -= num_elites;

	// Main loop
	for (int gen = 0; gen < MAX_GEN; gen++) {

		pop_timetables = build_timetable(population);

		// Rebuild the cache
		for (int i = 0; i < POP_SIZE; i++) {
			population[i].fitness = get_fitness(population[i]);
		}

		// Sort the population by fitness
		population = sort_population_cached(population);

		// Print the best fitness
		for (int i = 0; i < chromosome_size; i++) {
			//cout << population[0][i] << " ";
		}
		cout << "Generation " << gen << " best fitness: " << get_fitness(population[0])
			<< " Average fitness: " << accumulate(population.begin(), population.end(), 0.0, [](double a, chromosome b) {return a + b.fitness; }) / POP_SIZE << endl;

		if (get_fitness(population[0]) == 1.0f) {
			get_fitness(population[0]);
			solution_found(population[0]);
			return 0;
		}
        
		//cout << accumulate(fitness_cache.begin(), fitness_cache.end(), 0.0) / fitness_cache.size() << ",";
		//cout << get_fitness(population[0]) << ",";

		// Elitism
		vector<chromosome> elites;
		for (int i = 0; i < num_elites; i++) {
			elites.push_back(population[i]);
		}

		// Remove the elites from the population
		//population.erase(population.begin(), population.begin() + num_elites);


		// Tournament selection
		vector<chromosome> winners;
		for (int i = 0; i < (int)(TOURNAMENT_WIN_RATE * POP_SIZE); i++) {
			winners.push_back(tournament_selection(population));
		}


		// Crossover enough times to refill the population

		int num_crosses = (POP_SIZE - num_elites) / 2;

		vector<chromosome> new_population;
		for (int i = 0; i < num_crosses; i++) {
			// If we don't cross add the parents back to the population
			if ((float)rand() / RAND_MAX > CROSS_RATE) {
				new_population.push_back(winners[i % winners.size()]);
				new_population.push_back(winners[(i + 1) % winners.size()]);
				continue;
			}

			pair<chromosome, chromosome> children = crossover_one_point(winners[i % winners.size()], winners[(i + 1) % winners.size()]);
			new_population.push_back(children.first);
			new_population.push_back(children.second);
		}

		// Mutation
		for (int i = 0; i < POP_SIZE - num_elites; i++) {
			new_population[i] = mutate(new_population[i]);
		}

		// Add the elites back to the population
		for (int i = 0; i < num_elites; i++) {
			new_population.push_back(elites[i]);
		}

		// Update the population
		population = new_population;

		// Reindex the population
		for (int i = 0; i < POP_SIZE; i++) {
			population[i].multiverse_idx = i;
		}
	}

	// Print the best chromosome
	population = sort_population(population);
	cout << "Best chromosome: ";
	for (int i = 0; i < chromosome_size; i++) {
		//cout << population[0][i] << " ";
	}
	cout << endl;

	return 0;
}

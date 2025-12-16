#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <ctime>
#include <math.h>
#include <iomanip>
#include <conio.h>
#include <fstream>
#include <chrono>

/*****************************************************
* This is a Minimization optimization problem.
* Our fitness function is : 
*		*** Fitness function : 1 / Object function
* 
*	We change Gene in mutation insteed of revers or exchange 
* strategy, because these strategy doesn't give us a new 
* chromosom.
* 
* ________________________________________________________________
* | # | Variables desc			| Vars  		| search key	 |
* ________________________________________________________________
* | 1 | Population size			| POPSIZE		| POPSIZE 		 |
* ________________________________________________________________
* | 2 | Max Generation			| MAXGENS		| MAXGENS		 |
* ________________________________________________________________
* | 3 | Number of Genes			| NGENS			| NGENS			 |
* ________________________________________________________________
* | 4 | Crossover rate			| PXOVER		| PXOVER		 |
* ________________________________________________________________
* | 5 | Mutation rate			| PMUTATION		| PMUTATION		 |
* ________________________________________________________________
* | 6 | Final answer of obF		| final_answer	| final_answer	 |
* ________________________________________________________________
* | 7 | Lower bound of alleles	| lbound		| lbound		 |
* ________________________________________________________________
* | 8 | Upper bound of alleles	| ubound		| ubound		 |
* ________________________________________________________________
* | 9 | popluation				| population	| population	 |
* ________________________________________________________________
* | 10| new popultaion 			| newpopulation	| newpopulation	 |
* |   | (Copy of new population)|               |                |
* ________________________________________________________________
* 
* 
* 
* 
* _______________________________________________________________________________________
* | # | Functions desc			| Function name					| search key			|
* _______________________________________________________________________________________
* | 1 | Generate random int		| random_integer_generator		| #random_integer		|
* |   | between two integer		|								|						|
* |   | number					|								|						|
* _______________________________________________________________________________________
* | 2 | Generate random double	| random_double_generator		| #random_double		|
* _______________________________________________________________________________________
* | 3 | Show current time		| timestamp						| #timestamp			|
* _______________________________________________________________________________________
* | 4 | Show chromosom with 	| report						| #report_chromosom		|
* |   | Custom index			|								|						|
* _______________________________________________________________________________________
* | 5 | Log chromosom into file	| insert_into_file				| #file_logger			|
* |   | with custom index		|								|						|
* _______________________________________________________________________________________
* | 6 | Generate start pop		| initialize					| #starter_population	|
* _______________________________________________________________________________________
* | 7 | Evaluate chromosom fit 	| evaluate						| #fitness_calc			|
* _______________________________________________________________________________________
* | 8 | Keep best of starter pop| keep_the_best					| #first_elitism		|
* |   | Like we run elitism for |								|						|
* |   | our first population	|								|						|
* _______________________________________________________________________________________
* | 9 | Reproduction the new 	| selector						| #reproduction			|
* |   | new generation			|								|						|
* |   | Roulette Wheel strategy |								|						|
* _______________________________________________________________________________________
* | 10| Crossover function		| crossover						| #crossover			|
* |   | Single point strategy	|								|						|
* |   | Swap left side of point |								|						|
* _______________________________________________________________________________________
* | 11| Do crossover on two		| combination					| #combination			|
* |   | selected parent.		|								|						|
* |   | It's used inside		|								|						|
* |   | crossover function		|								|						|
* _______________________________________________________________________________________
* | 12| Elitism on population	| elitist						| #elitism				|
* |   | Change with a worst		|								|						|
* |   | member of population	|								|						|
* _______________________________________________________________________________________
* | 12| Mutate on population	| mutate						| #mutation				|
* |   | Change genes value with |								|						|
* |   | new random generation	|								|						|
* _______________________________________________________________________________________
* 
* 
* 
* 
* 
******************************************************/
using namespace std;

#define POPSIZE 5
#define MAXGENS 5000
#define NGENS 12
#define PXOVER 0.15	
#define PMUTATION 0.05

double const final_answer = 150;

/***
* Our alleles for genes must be unsigned numbers because genes are a quantity of an order
* 0 <= Gene[i] <= final_answer
* But we knows that if one gene value equals to 0 or final answer, the others should equal to final answer or 0. But we don't want Gene with 0 value.
* Because we don't need order with 0 quantity.
*/
double lbound = 0.01;
double ubound = final_answer / 2;

struct Chromosom
{
	double gene[NGENS];
	double fitness;
	double rfitness;
	double cfitness;
};

struct Chromosom population[POPSIZE + 1];
struct Chromosom newpopulation[POPSIZE + 1];

// #random_integer
int random_integer_generator(int a, int b);

// #random_double
double random_double_generator(double a, double b);	

// #timestamp
void timestamp();
// #report_chromosom
void report(int generation);

// #file_logger
void insert_into_file(int generation);

// #starter_population
void initialize();

// #fitness_calc
void evaluate();

// #first_elitism
void keep_the_best();

// #reproduction
void selector();									

// #crossover
void crossover();

// #combination
void combination(int one, int two);

// #elitism
void elitist();

// #mutation
void mutate();


int main()
{
	int generation;
	int i;

	timestamp();

	auto start = chrono::high_resolution_clock::now();

	srand((unsigned)time(NULL));

	cout << "Σx[i] = N : " << final_answer << endl;

	//Initialize the starter population
	initialize();
	evaluate();
	keep_the_best();
	//insert_into_file(0);

	for (generation = 0; generation < MAXGENS; generation++)
	{
		selector();
		crossover();
		mutate();
		//report(generation);
		//insert_into_file(generation);
		evaluate();
		elitist();
	}

	auto stop = chrono::high_resolution_clock::now();

	cout << "Exec time after genetic : " << (stop - start).count() << "\n";
	cout << "\n";
	cout << "Best member after" << MAXGENS << "generations:\n";
	cout << "\n";

	for (i = 0; i < NGENS; i++)
	{
		cout << fixed << "var(" << i << ") = " << population[POPSIZE].gene[i] << "\n";
	}

	cout << "\n";
	cout << "Best fitness =" << population[POPSIZE].fitness << "\n";

	cout << "Normal end of execution.\n";
	cout << "\nΣx[i] : ";
	double sum = 0;

	for (i = 0; i < NGENS; i++)
	{
		sum += population[POPSIZE].gene[i];
	}

	cout << sum << endl<<endl;

	timestamp();

	_getch();
	return 0;
}

// #timestamp
void timestamp()
{
# define TIME_SIZE 40

	static char time_buffer[TIME_SIZE];
	const struct tm* tm;
	size_t len;
	time_t now;

	now = time(NULL);
	tm = localtime(&now);
	len = strftime(time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm);

	cout << time_buffer << "\n";
	return;
# undef TIME_SIZE
}


// #starter_population
void initialize()
{
	int i;
	int j;

	//Initialize genes within the bounds 
	for (i = 0; i < NGENS; i++)//Foreach Chromosom
	{
		for (j = 0; j < POPSIZE; j++)//Foreach Genes
		{
			population[j].fitness = 0;
			population[j].rfitness = 0;
			population[j].cfitness = 0;
			population[j].gene[i] = random_double_generator(lbound, ubound);
		}
	}

	return;
}

// #random_double
double random_double_generator(double a, double b)
{
	double tmp;
	tmp = a + (static_cast<double>(rand()) / RAND_MAX) * (b - a);
	return tmp;
}

// #fitness_calc
void evaluate()
{
	int member;
	int i;
	//The current function is: Σ x[i]
	for (member = 0; member < POPSIZE; member++)
	{
		double sum = 0;
		for (i = 0; i < NGENS; i++)
		{
			sum += population[member].gene[i];
		}
		population[member].fitness = 1 / (1 + abs(sum - final_answer)); //fitness function = 1 / (1 + object function)
	}
}

// #first_elitism
void keep_the_best()
{
	int best_member;
	int mem;
	int i;

	best_member = 0;

	for (mem = 0; mem < POPSIZE; mem++)
	{
		if (population[POPSIZE].fitness < population[mem].fitness)//minimize fitness_1 < fitness_2 then fitness_2 is better
		{
			best_member = mem;
			population[POPSIZE].fitness = population[mem].fitness;
		}
	}

	for (i = 0; i < NGENS; i++)
	{
		population[POPSIZE].gene[i] = population[best_member].gene[i];
	}

	return;
}

// #reproduction
void selector()
{
	const double a = 0.0;
	const double b = 1.0;
	int i;
	int j;
	int mem;
	double p;
	double total_fitness;

	/*
	 * Find the total fitness of the population.
	*/
	total_fitness = 0.0;
	for (mem = 0; mem < POPSIZE; mem++)
	{
		total_fitness = total_fitness + population[mem].fitness;
	}

	/*
	 * Calculate the relative fitness of each member.
	*/
	for (mem = 0; mem < POPSIZE; mem++)
	{
		population[mem].rfitness = population[mem].fitness / total_fitness;
	}

	/*
	 * Calculate the cumulative fitness.
	*/
	population[0].cfitness = population[0].rfitness;
	for (mem = 1; mem < POPSIZE; mem++)
	{
		population[mem].cfitness = population[mem - 1].cfitness +
			population[mem].rfitness;
	}

	/*
	 * Select survivors using cumulative fitness.  Rollet wheel
	*/
	for (i = 0; i < POPSIZE; i++)
	{
		p = random_double_generator(a, b);
		if (p < population[0].cfitness) // if p is lower than first population we pick first 
		{
			newpopulation[i] = population[0];
		}
		else // else we should check every population the population[j].cfitness >= p && p <population[j + 1].cfitness
		{
			for (j = 0; j < POPSIZE; j++)
			{
				if (population[j].cfitness >= p && p < population[j + 1].cfitness)
				{
					newpopulation[i] = population[j + 1];
					break;
				}
			}
		}
	}

	/*
	 * Overwrite the old population with the new one.
	*/
	for (i = 0; i < POPSIZE; i++)
	{
		population[i] = newpopulation[i];
	}

	return;
}


// #crossover
void crossover()
{
	const double a = 0.0;
	const double b = 1.0;
	int mem;
	int one;
	int first = 0;
	double p;

	for (mem = 0; mem < POPSIZE; ++mem)
	{
		p = random_double_generator(a, b);

		if (p < PXOVER)
		{
			++first;

			if (first % 2 == 0) // we need  2 parents for combination. if first is even this means we pick first one and need the second one
			{
				combination(one, mem);
			}
			else
			{
				one = mem;
			}

		}
	}
	return;
}

// #combination
void combination(int one, int two)
{
	int i;
	int point;
	double t;
	//Select the crossover point.(single point)
	point = random_integer_generator(0, NGENS - 1);
	//
	//Swap genes in positions 0 through POINT-1. (Left side of point)
	//
	for (i = 0; i < point; i++)
	{
		t = population[one].gene[i];
		population[one].gene[i] = population[two].gene[i];
		population[two].gene[i] = t;
	}

	return;
}

// #random_integer
int random_integer_generator(int a, int b)
{
	int tmp;
	tmp = (rand() % (b - a + 1)) + a;
	return tmp;
}

// #mutation
void mutate()
{
	const double a = 0.0;
	const double b = 1.0;
	int i;
	int j;
	double x;

	for (i = 0; i < POPSIZE; i++)
	{
		for (j = 0; j < NGENS; j++)
		{
			x = random_double_generator(a, b);
			if (x < PMUTATION)
			{
				population[i].gene[j] = random_double_generator(lbound, ubound);
			}
		}
	}

	return;
}

// #report_chromosom
void report(int generation)
{
	double avg;
	double best_val;
	int i;
	double square_sum;
	double stddev;
	double sum;
	double sum_square;

	if (generation == 0)
	{
		cout << "\n";
		cout << "Generation Best Average Standard/n";
		cout << "number value fitness deviation/n";
		cout << "\n";
	}

	sum = 0.0;
	sum_square = 0.0;

	for (i = 0; i < POPSIZE; i++)
	{
		sum = sum + population[i].fitness;
		sum_square = sum_square + population[i].fitness * population[i].fitness;
	}

	avg = sum / (double)POPSIZE;
	square_sum = avg * avg * POPSIZE;
	stddev = sqrt((sum_square - square_sum) / (POPSIZE - 1));
	best_val = population[POPSIZE].fitness;

	cout << "" << setw(8) << generation
		<< "" << setw(14) << best_val
		<< "" << setw(14) << avg
		<< "" << setw(14) << stddev << "\n";

	return;
}

// #elitism
void elitist()
{
	int i;
	double best;
	int best_mem;
	double worst;
	int worst_mem;

	best = population[0].fitness;
	worst = population[0].fitness;

	for (i = 0; i < POPSIZE - 1; ++i)
	{
		if (population[i + 1].fitness < population[i].fitness)
		{

			if (best <= population[i].fitness)
			{
				best = population[i].fitness;
				best_mem = i;
			}

			if (population[i + 1].fitness <= worst)
			{
				worst = population[i + 1].fitness;
				worst_mem = i + 1;
			}

		}
		else
		{

			if (population[i].fitness <= worst)
			{
				worst = population[i].fitness;
				worst_mem = i;
			}

			if (best <= population[i + 1].fitness)
			{
				best = population[i + 1].fitness;
				best_mem = i + 1;
			}

		}

	}
	//
	//If the best individual from the new population is better than 
	//the best individual from the previous population, then 
	//copy the best from the new population; else replace the 
	//worst individual from the current population with the 
	//best one from the previous generation                     
	//
	if (population[POPSIZE].fitness <= best)
	{
		for (i = 0; i < NGENS; i++)
		{
			population[POPSIZE].gene[i] = population[best_mem].gene[i];
		}
		population[POPSIZE].fitness = population[best_mem].fitness;
	}
	else
	{
		for (i = 0; i < NGENS; i++)
		{
			population[worst_mem].gene[i] = population[POPSIZE].gene[i];
		}
		population[worst_mem].fitness = population[POPSIZE].fitness;
	}

	return;
}

// #file_logger
void insert_into_file(int generation) {

	fstream MyFile;

	MyFile.open("filename.txt", std::ios_base::app | std::ios_base::in);
	MyFile << "#" << generation << " : \n";

	for (int i = 0; i < POPSIZE; i++)
	{
		double sum = 0;
		for (int j = 0; j < NGENS; j++)
		{
			MyFile << "[" << population[i].gene[j] << "] ";
			sum += population[i].gene[j];
		}

		MyFile << " ==> " << sum << "\n";
	}

	MyFile.close();

}



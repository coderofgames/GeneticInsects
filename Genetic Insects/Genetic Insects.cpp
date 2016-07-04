// Genetic Insects.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//#include "Utils.h"
#include <iostream>
#include <vector>
#include <string>
#include "Utils.h"

using std::vector;



class BitChromosome : public BlockIndexBitField
{
public:
	BitChromosome(int numBits) : BlockIndexBitField(numBits)
	{
	}



	
};


void SetBit(int &gene, int idx)
{
	gene |= (1 << idx);
}

bool CheckBit(int &gene, int idx)
{
	return gene & (1 << idx);
}

void ResetBit(int &gene, int idx)
{
	gene &= ~(1 << idx);
}







namespace SIMPLE_GA {

	class InsectChromosome1
	{
	public:

		int gene = 0;

		float fitness = 0.0f;

		int target = 0;
		int numTraits = 8;
		int numGenes = 25;

		int ANTENNAE_MASK = 0;
		int HEAD_MASK = 0;
		int WINGS_MASK = 0;
		int BODY_MASK = 0;
		int FEET_MASK = 0;
		int BODY_COLOR_MASK = 0;
		int SIZE_MASK = 0;
		int HEAD_COLOR_MASK = 0;

		int antennae_start = 0;
		int head_start = 4;
		int wing_start = 6;
		int body_start = 10;
		int feet_start = 16;
		int body_color_start = 17;
		int size_start = 20;
		int head_color_start = 22;
		int head_color_end = 25;

		InsectChromosome1()
		{
			for (int i = antennae_start; i < head_start; i++) SetBit(ANTENNAE_MASK, i);
			for (int i = head_start; i < wing_start; i++) SetBit(HEAD_MASK, i);
			for (int i = wing_start; i < body_start; i++) SetBit(WINGS_MASK, i);
			for (int i = body_start; i < feet_start; i++) SetBit(BODY_MASK, i);
			for (int i = feet_start; i < body_color_start; i++) SetBit(FEET_MASK, i);
			for (int i = body_color_start; i < size_start; i++) SetBit(BODY_COLOR_MASK, i);
			for (int i = size_start; i < head_color_start; i++) SetBit(SIZE_MASK, i);
			for (int i = head_color_start; i < head_color_end; i++) SetBit(HEAD_COLOR_MASK, i);
		}

		void SetAntennae(int choice)
		{
			gene |= (ANTENNAE_MASK & (choice));

		}

		void SetHead(int choice)
		{
			gene |= (HEAD_MASK & (choice << head_start));

		}

		void SetWing(int choice)
		{
			gene |= (WINGS_MASK & (choice << wing_start));

		}

		void SetBody(int choice)
		{
			gene |= (BODY_MASK & (choice << body_start));
		}

		void SetFeet(int choice)
		{
			gene |= (FEET_MASK & (choice << feet_start));
		}
		void SetBodyColor(int choice)
		{
			gene |= (BODY_COLOR_MASK & (choice << body_color_start));
		}
		void SetSize(int choice)
		{
			gene |= (SIZE_MASK & (choice << size_start));
		}
		void SetHeadColor(int choice)
		{
			gene |= (HEAD_COLOR_MASK & (choice << head_color_start));
		}



		int GetAntennae()
		{
			return gene & (ANTENNAE_MASK);

		}

		int GetHead()
		{
			return ((gene & HEAD_MASK) >> head_start);

		}

		int GetWing()
		{
			return ((gene & WINGS_MASK) >> wing_start);

		}

		int GetBody()
		{
			return ((gene & BODY_MASK) >> body_start);
		}

		int GetFeet()
		{
			return ((gene & FEET_MASK) >> feet_start);
		}
		int GetBodyColor()
		{
			return ((gene & BODY_COLOR_MASK) >> body_color_start);
		}
		int GetSize()
		{
			return ((gene & SIZE_MASK) >> size_start);
		}
		int GetHeadColor()
		{
			return ((gene & HEAD_COLOR_MASK) >> head_color_start);
		}



		void Fitness() {
			int score = 0;
			for (int i = 0; i < this->numGenes; i++) {
				//Is the character correct ?
				if (CheckBit(gene, i))
				{
					if (CheckBit(target, i))
						score++;
				}
				else
				{
					if (!CheckBit(target, i))
						score++;
				}

			}
			//Fitness is the percentage correct.
			fitness = float(score) / (float)numGenes;
			//fitness = fitness * fitness;
		}

		void mutate(float mutationRate) {
			
			if (RandomFloat(0.0, 1.0) < mutationRate) {
				int gene_to_mutate = RandomInt(0, 25);
				
				if (gene & (1 << gene_to_mutate))
				{
					gene |= (0 << gene_to_mutate);
				}
				else gene |= (1 << gene_to_mutate);
			}
		}

		int crossoverSinglePoint(int geneB)
		{
			int cross_point = RandomInt(0, 25);

			int output = 0;

			for (int i = 0; i < cross_point; i++)
			{
				if (CheckBit( gene, i ))
					SetBit(output, i);
			}
			for (int i = cross_point; i < 32; i++)
			{
				if (CheckBit(geneB, i))
					SetBit(output, i);
			}

			return output;
		}

		int crossoverTwoPoint(int geneB)
		{
			int cross_point_1 = RandomInt(0, 25);
			int cross_point_2 = 0;

			if (cross_point_1 > 12)
			{
				cross_point_2 = RandomInt(0, 12);
				// swap them
				int temp = cross_point_1;
				cross_point_1 = cross_point_2;
				cross_point_2 = temp;
			}
			else
			{
				cross_point_2 = RandomInt(12, 25);
			}

			int output = 0;

			for (int i = 0; i < cross_point_1; i++)
			{
				if (CheckBit(gene, i))
					SetBit(output, i);
			}

			for (int i = cross_point_1; i < cross_point_2; i++)
			{
				if (CheckBit(geneB, i))
					SetBit(output, i);
			}
			for (int i = cross_point_2; i < 32; i++)
			{
				if (CheckBit(gene, i))
					SetBit(output, i);
			}

			return output;

		}

		int uniformCrossover(int geneB)
		{
			int mask = 0;
			for (int i = 0; i < 25; i++)
			{
				SetBit( mask, RandomInt(0, 2) );
			}

			int output = 0;

			output = (gene & mask) & (geneB & ~mask);
			
			return output;
		}

		int heuristicCrossover(int geneB, float alpha)
		{
			int output = 0;

			output = (int) ((float)gene * alpha) + (1 - alpha) * (float)geneB;

			return output;
		}

		void SetTarget(int t){ target = t; }

	};

	class Chromosome
	{
	public:



		char *genes;// = new char[18];
		char *target;
		int num_genes = 18;

		float fitness;

		Chromosome()
		{

		}

		void SetTarget(char* buff)
		{
			bool check = false; // don#t bother 
			if (check)
			{
				int sz = sizeof(buff);
				if (sz != num_genes) return;
			}

			for (int i = 0; i < num_genes; i++)
				target[i] = buff[i];

		}

		void Initialize()
		{
			genes = new char[num_genes];
			target = new char[num_genes];

			for (int i = 0; i < num_genes; i++) {
				//Picking randomly from a range of characters with ASCII values between 32 and 128. For more about ASCII : http ://en.wikipedia.org/wiki/ASCII
				genes[i] = (char)RandomInt(32, 128);
			}
		}


		void Fitness() {
			int score = 0;
			for (int i = 0; i < num_genes; i++) {
				//Is the character correct ?
				if (genes[i] == target[i]) {
					//If so, increment the score.
					score++;
				}
			}
			//Fitness is the percentage correct.
			fitness = float(score) / (float)num_genes;
			//fitness = fitness * fitness;
		}

		Chromosome* crossover(Chromosome *partner) {
			Chromosome *child = new Chromosome();
			child->num_genes = this->num_genes;
			child->Initialize();
			child->SetTarget(this->target);
			int midpoint = int(RandomInt(0, num_genes));
			for (int i = 0; i < num_genes; i++) {
				if (i > midpoint) child->genes[i] = genes[i];
				else   {
					if (partner != 0)
						child->genes[i] = partner->genes[i];
				}
			}
			return child;
		}


		void crossover2(Chromosome *partnerA, Chromosome* partnerB) {

			//child->num_genes = this->num_genes;
			//child->Initialize();
			//child->SetTarget(this->target);
			this->fitness = 0.0f;

			int midpoint = int(RandomInt(0, num_genes));
			for (int i = 0; i < num_genes; i++) {
				if (i > midpoint) genes[i] = partnerA->genes[i];
				else   {
					if (partnerB != 0)
						genes[i] = partnerB->genes[i];
				}
			}
		}

		//Mutation
		void mutate(float mutationRate) {
			for (int i = 0; i < num_genes; i++) {
				if (RandomFloat(0.0, 1.0) < mutationRate) {
					genes[i] = (char)RandomInt(32, 128);
				}
			}
		}
	};

	class Population
	{
	public:

		vector<InsectChromosome1*> population;
		unsigned int population_size = 200;

		float mutationRate = 0.03;

		float sumFitness;

		float best_fitness = 0.0f;
		int best_index = 0;

		bool match = false;

		int target = 0;
		//char* target = "to be or not to be";

		Population()
		{
			InsectChromosome1 c1;

			c1.SetHead(2);
			c1.SetAntennae(4);
			c1.SetBody(34);
			c1.SetWing(5);
			c1.SetFeet(1);
			c1.SetBodyColor(3);
			c1.SetSize(2);
			c1.SetHeadColor(1);

			target = c1.gene;
		}

		Population(int sz){
			population_size = sz;

			InsectChromosome1 c1;

			c1.SetHead(2);
			c1.SetAntennae(4);
			c1.SetBody(34);
			c1.SetWing(5);
			c1.SetFeet(1);
			c1.SetBodyColor(3);
			c1.SetSize(2);
			c1.SetHeadColor(1);

			target = c1.gene;
		}
		/*
		std::string  GetBest()
		{
			std::string the_best(population[best_index]->gene);

			return the_best;
		}*/

		int GetBest(){ return population[best_index]->gene; }
		void Initialize()
		{
			for (int i = 0; i < population_size; i++) {
				//Initializing each member of the population

				InsectChromosome1* dna = new InsectChromosome1();
				
				//dna->Initialize();
				dna->SetTarget(target);
				population.push_back(dna);


			}
		}

		
		void Update()
		{
			std::vector<InsectChromosome1*> mating_pool;

			int best = 0;
			best_index = 0;
			best_fitness = 0.0f;
			sumFitness = 0.0f;
			//evaluate fitness
			for (int i = 0; i < population.size(); i++) {
				//population[i]->Fitness();
				population[i]->Fitness();

				sumFitness += population[i]->fitness;

				if (population[i]->fitness > best_fitness)
				{
					best_fitness = population[i]->fitness;
					best_index = i;
				}

				InsectChromosome1 *b = new InsectChromosome1();
				b->gene = population[i]->gene;

				mating_pool.push_back(b);
			}



			int best_value = this->GetBest();
			//strcpy_s(best_string, population[best_index]->genes);
			//for (int i = 0; i < 18; i++)
			//	best_string[i] = population[best_index]->genes[i];


			for (int i = 0; i < population.size(); i++) {


				if (i == this->best_index) continue;

				int a = this->ChooseParent();
				int b = this->ChooseParent();

				if (a == b) if (a <= population.size() - 2)b = a + 1; else b = 0;

				InsectChromosome1 *partnerA = mating_pool[a];
				InsectChromosome1 *partnerB = mating_pool[b];

				//Step 3a: Crossover
				//DNA *child = partnerA->crossover(partnerB);
				population[i]->gene = partnerA->crossoverSinglePoint(partnerB->gene);
				//Step 3b: Mutation

				population[i]->mutate(mutationRate);
			}

			for (int i = 0; i < population.size(); i++)
			{
				delete mating_pool[i];
				

			}

			mating_pool.clear();
		}

		int ChooseParent()
		{
			int randSelector = (int)RandomFloat(0, sumFitness) * 100;

			// Translate this number to the corresponding member**
			int memberID = 0;
			int partialSum = 0;

			for (int i = 0; i < population.size(); i++) {
				int n = (int)(population[i]->fitness * 100);
				if (partialSum < randSelector && partialSum + n >= randSelector)
				{
					return i;
				}
				partialSum += n;
				/*			for (int j = 0; j < n; j++)
				{
				partialSum += 1;
				if (partialSum == randSelector)
				{
				return i;
				}
				}*/
			}

			int alternate = RandomInt(0, 99);

			return this->best_index;

			/*while (randSelector  > partialSum)
			{
			partialSum += population[memberID]->fitness;// / sumFitness;// fitness(memberID);
			memberID++;
			}*/

			//return memberID;
		}

		void Old_Breeding()
		{
			best_fitness = 0.0f;
			sumFitness = 0.0f;
			//evaluate fitness
			for (int i = 0; i < population.size(); i++) {
				//population[i]->Fitness();
				population[i]->Fitness();

				sumFitness += population[i]->fitness;

				if (population[i]->fitness > best_fitness)
				{
					best_fitness = population[i]->fitness;
					best_index = i;
				}
			}

			// Find the sum of fitnesses. The function fitness(i) should 
			//return the fitness value   for member i**


			//for (int i = 0; i < nmembers; i++)
			//	sumFitness += fitness(i);


			// Get a floating point number in the interval 0.0 ... sumFitness**
			//float randomNumber = (float(rand() % 10000) / 9999.0f) * sumFitness;




			for (int i = 0; i < population.size(); i++) {
				//int a = int(RandomInt(0, mating_pool.size() - 1));
				//int b = int(RandomInt(0, mating_pool.size() - 1));
				//if (a == b){ if (a < mating_pool.size() - 2)b = a + 1; else b = 0; }
				//if (population[i]->fitness  > 0.8) continue;

				if (i == this->best_index) continue;

				int a = this->ChooseParent();
				int b = this->ChooseParent();

				if (a == b) if (a <= population.size() - 2)b = a + 1; else b = 0;

				InsectChromosome1 *partnerA = population[a];
				InsectChromosome1 *partnerB = population[b];

				//Step 3a: Crossover
				//DNA *child = partnerA->crossover(partnerB);
				population[i]->gene = partnerA->crossoverSinglePoint(partnerB->gene);
				//Step 3b: Mutation

				population[i]->mutate(mutationRate);


			}
		}


		void Delete()
		{
			for (int i = 0; i < population.size(); i++)
			{
				delete population[i];
				population[i] = 0;
			}

			population.clear();
		}
	};
};

using namespace SIMPLE_GA;

class SimpleExample
{
protected:
	bool m_bDeleted = false;
public:

	SimpleExample(){}
	~SimpleExample(){
		if (m_bDeleted == false)
			this->Delete();
	}

	virtual void Initialize(){}
	virtual void Draw(){};
	virtual void Delete(){};
	virtual void keyboard(unsigned char key, int x, int y){};
	virtual void keyboardup(unsigned char key, int x, int y){};
	virtual void Special(int key, int x, int y){};

	virtual void mouse(int button, int state, int x, int y){}
};

class SimpleGA : public SimpleExample
{
public:

	int population_size = 100;

	char* target = "to be or not to be";
	float mutationRate = 0.01;


	Population population3;

	void SetPopulationSize(int n){ this->population_size = n; }

	void Initialize()
	{
		//population = new DNA*[population_size];
		population3.Initialize();


	}
	bool solved = false;
	void print_gene(int g)
	{
		for (int i = 0; i < 32; i++)
		{
			if (CheckBit(g, i)) std::cout << "1";
			else std::cout << "0";
		}
	}
	void Draw() {

		if (solved == false)
		{
			population3.Update();

			static int generation_count = 0;
			std::cout << "generation" << generation_count++ << std::endl;//
			
			print_gene(population3.GetBest());
			std::cout << std::endl;
			print_gene(population3.target);

			
			std::cout << std::endl;
			
			//<< population3.GetBest() << std::endl;


			if (population3.best_fitness > 0.97f)solved = true;
		}
	}

	void keyboard(unsigned char key, int x, int y){}
	void keyboardup(unsigned char key, int x, int y){}
	void Special(int key, int x, int y){};

	void mouse(int button, int state, int x, int y){}

	void Delete(){
		/*		for (int i = 0; i < population_size; i++)
		{
		delete population2[i];
		population2[i] = 0;
		}

		population2.clear();
		*/
		population3.Delete();
		//mating_pool.clear();
		this->m_bDeleted = true;

	}
};




/*
#define ANTENNAE_MASK 00001111111111111111111111111111
#define HEAD_MASK	  11110001111111111111111111111111
#define WINGS_MASK	  11111110000111111111111111111111
#define BODY_MASK	  11111111111000000111111111111111
#define FEET_MASK	  11111111111111111011111111111111
#define BCOLOR_MASK	  11111111111111111100011111111111
#define SIZE_MASK	  11111111111111111111100111111111
#define HCOLOR_MASK	  11111111111111111111111000111111
*/



int _tmain(int argc, _TCHAR* argv[])
{

	SIMPLE_GA::InsectChromosome1 c1;
	

	

	RandomInt(0, 16);

	int antennae_selection = RandomInt(0, 16);
	int head_selection = RandomInt(0, 4);
	int wing_selection = RandomInt(0, 4);
	int body_selection = RandomInt(0, 64);
	int feet_selection = RandomInt(0, 2);
	int body_color_selection = RandomInt(0, 3);
	int size_selection = RandomInt(0, 4);
	int head_color_selection = RandomInt(0, 4);

	


	std::cout << "printing antennae type: " << antennae_selection << std::endl;
	std::cout << "printing head type: " << head_selection << std::endl;
	std::cout << "printing wing type: " << wing_selection << std::endl;
	std::cout << "printing body type: " << body_selection << std::endl;
	std::cout << "printing feet type: " << feet_selection << std::endl;
	std::cout << "printing body color type: " << body_color_selection << std::endl;
	std::cout << "printing size type: " << size_selection << std::endl;
	std::cout << "printing head color type: " << head_color_selection << std::endl;

	std::cout << std::endl;

	//int gene = 0;


	c1.SetAntennae(antennae_selection);
	c1.SetHead(head_selection);
	c1.SetWing(wing_selection);
	c1.SetBody(body_selection);
	c1.SetFeet(feet_selection);
	c1.SetBodyColor(body_color_selection);
	c1.SetSize(size_selection);
	c1.SetHeadColor(head_color_selection);
	
	//print_gene(gene);

	
	
	std::cout << std::endl;

	std::cout << "printing antennae type: " << c1.GetAntennae() << std::endl;
	std::cout << "printing head type: " << c1.GetHead() << std::endl;
	std::cout << "printing wing type: " << c1.GetWing() << std::endl;
	std::cout << "printing body type: " << c1.GetBody() << std::endl;
	std::cout << "printing feet type: " << c1.GetFeet() << std::endl;
	std::cout << "printing body color type: " << c1.GetBodyColor() << std::endl;
	std::cout << "printing size type: " << c1.GetSize() << std::endl;
	std::cout << "printing head color type: " << c1.GetHeadColor() << std::endl;

	std::cout << std::endl;

	SimpleGA *example = new  SimpleGA();
	example->Initialize();
	while (example->solved == false)
		example->Draw();
	example->Delete();
	
	return 0;
}


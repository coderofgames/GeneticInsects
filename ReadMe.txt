========================================================================
    CONSOLE APPLICATION : Genetic Insects Project Overview
========================================================================

 This code is based on the example in chapter 1 of 
"Genetic Algorithms", K.F.Man, K.S. Tang and S.Kwong

Please view the ipython notebook in the Genetic Insects directory.

https://github.com/coderofgames/GeneticInsects/blob/master/Genetic%20Insects/Stats_Display.ipynb

Stats_Display.ipynb

Very similar to the Shakespeare monkeys program, this features improvements
a) the population has a sub-population, named in the code as "mating pool"
b) the mutation operator mutates 1 bit of the chromosome
c) the InsectChromosome is 1 integer in size, however uses bitfield operations to set genes
d) The program will breed an insect survivor based on initial parameters (hard coded atm)

Still fixing bugs ... 
/*
        NEAT Visualizer
        Copyright (C) 2012-2014 Eric Laukien

        This software is provided 'as-is', without any express or implied
        warranty.  In no event will the authors be held liable for any damages
        arising from the use of this software.

        Permission is granted to anyone to use this software for any purpose,
        including commercial applications, and to alter it and redistribute it
        freely, subject to the following restrictions:

        1. The origin of this software must not be misrepresented; you must not
                claim that you wrote the original software. If you use this
   software in a product, an acknowledgment in the product documentation would
   be appreciated but is not required. 2. Altered source versions must be
   plainly marked as such, and must not be misrepresented as being the original
   software. 3. This notice may not be removed or altered from any source
   distribution.
*/

#include "ImageEvolver.h"
#include "../neat/NetworkGenotype.h"
#include "../neat/UtilFuncs.h"
#include <iostream>

void ImageEvolver::train(const neat::NetworkGenotype &genotype,
                         size_t populationSize, size_t numGenerations,
                         std::mt19937 &generator) {
    assert(populationSize >= 2);

    const float normalDistance = 1.0f;
    const float normalDistanceInfluence = 3.0f;
    const float normalDistanceExp = 3.0f;
    const float unconnectedNodeDistance = 1.0f;
    const float unconnectedNodeDistanceInfluence = 3.0f;
    const float unconnectedNodeDistanceExp = 3.0f;
    const float mutationRate = 0.6f;
    const float maxMutationPerturbationPosition = 0.1f;
    const float reproduceRatio = 0.1f;
    const float unevenConnectionAnglesPunishment = 3.0f;
    const float unevenConnectionAnglesExp = 3.0f;

    std::uniform_real_distribution<float> distPositionPerturbation(
        -maxMutationPerturbationPosition, maxMutationPerturbationPosition);

    const size_t numNodes = genotype.getNumHidden() + genotype.getNumInputs() +
                            genotype.getNumOutputs();

    _bestFitness = -999999.0f;

    _activeNodeIndexToNodeIndex.clear();
    _nodeIndexToActiveNodeIndex.clear();

    for (size_t i = 0; i < numNodes; i++)
        if (!genotype.getNodeData(i)
                 ._connections.empty()) { // Node is in use (has connections)
            _activeNodeIndexToNodeIndex.push_back(i);
            _nodeIndexToActiveNodeIndex.push_back(
                _activeNodeIndexToNodeIndex.size() - 1);
        } else
            _nodeIndexToActiveNodeIndex.push_back(-1); // No active node flag

    const size_t numActiveNodes = _activeNodeIndexToNodeIndex.size();

    std::uniform_int_distribution<int> distActiveNodes(
        0, _activeNodeIndexToNodeIndex.size() - 1);

    std::vector<CandidateSolution> population(populationSize);

    std::uniform_real_distribution<float> distNormalized(-1.0f, 1.0f);

    // Generate random init population
    for (size_t i = 0; i < populationSize; i++) {
        population[i]._positions.resize(numNodes);

        for (size_t j = 0; j < numActiveNodes; j++) {
            sf::Vector2f(distNormalized(generator), distNormalized(generator));
            population[i]._positions[j].x = distNormalized(generator);
            population[i]._positions[j].y = distNormalized(generator);
        }
    }

    std::uniform_real_distribution<float> dist01(0.0f, 1.0f);

    // Run cycles
    for (size_t g = 0; g < numGenerations; g++) {
        // ----------------------------- Fitness Evaluation
        // -----------------------------

        // Temp list used later
        struct IndexAndFitness {
            size_t _index;
            float _fitness;
        };

        std::list<IndexAndFitness> fitnesses;

        for (size_t i = 0; i < populationSize; i++) {
            IndexAndFitness fitnessAndIndex;

            fitnessAndIndex._fitness = 0.0f;
            fitnessAndIndex._index = i;

            sf::Vector2f directionSum(0.0f, 0.0f);

            // Evaluate fitness
            for (size_t j = 0; j < numActiveNodes; j++) {
                // Distance from connect nodes
                neat::NetworkGenotype::NodeData nodeData =
                    genotype.getNodeData(_activeNodeIndexToNodeIndex[j]);

                size_t numConnectionsForNode = nodeData._connections.size();

                for (std::list<std::shared_ptr<neat::ConnectionGene>>::iterator
                         it = nodeData._connections.begin();
                     it != nodeData._connections.end(); it++) {
                    assert((*it)->_inIndex != -1);

                    if ((*it)->_inIndex == j)
                        continue;

                    const size_t inIndex =
                        _nodeIndexToActiveNodeIndex[(*it)->_inIndex];

                    // Add difference from a distance of 1 between connected
                    // node as cost
                    float distanceCost = pow(
                        std::abs(normalDistance -
                                 distance(population[i]._positions[j],
                                          population[i]._positions[inIndex])),
                        normalDistanceExp);

                    directionSum +=
                        normalize(population[j]._positions[inIndex] -
                                  population[i]._positions[j]);

                    fitnessAndIndex._fitness -=
                        normalDistanceInfluence * distanceCost;
                }

                // Add all position costs
                for (size_t k = 0; k < numActiveNodes; k++)
                    fitnessAndIndex._fitness -=
                        unconnectedNodeDistanceInfluence *
                        pow(std::abs(unconnectedNodeDistance -
                                     distance(population[i]._positions[j],
                                              population[i]._positions[k])),
                            unconnectedNodeDistanceExp);
            }

            if (directionSum.x != 0.0f || directionSum.y != 0.0f)
                fitnessAndIndex._fitness -=
                    unevenConnectionAnglesPunishment *
                    powf(magnitude(directionSum), unevenConnectionAnglesExp);

            fitnesses.push_back(fitnessAndIndex);
        }

        // ----------------------------- Get Best Performers
        // -----------------------------

        assert(fitnesses.size() == populationSize);

        const int numReproduce = std::max(
            2, static_cast<int>(
                   reproduceRatio * static_cast<float>(populationSize) + 0.5f));

        std::vector<size_t> bestIndices(numReproduce);

        for (int i = 0; i < numReproduce; i++) {
            std::list<IndexAndFitness>::iterator it = fitnesses.begin();
            it++;

            std::list<IndexAndFitness>::iterator best = fitnesses.begin();

            for (; it != fitnesses.end(); it++)
                if (it->_fitness > best->_fitness)
                    best = it;

            if (i == 0) // Most fit one in order to keep track of best across
                        // all generations
                if (best->_fitness > _bestFitness) {
                    _best = population[best->_index];
                    _bestFitness = best->_fitness;
                }

            // Add best
            bestIndices.push_back(best->_index);

            // Remove from list to avoid finding same one again
            fitnesses.erase(best);
        }

        // ----------------------------- Reproduction
        // -----------------------------

        // Randomly repoduce top performers until have a large enough population
        // again
        std::vector<CandidateSolution> newPopulation(populationSize);

        std::uniform_int_distribution<int> distNumReproduce(0,
                                                            numReproduce - 1);

        for (size_t i = 0; i < populationSize; i++) {
            // Get 2 random members of best group
            CandidateSolution &solution1 =
                population[bestIndices[distNumReproduce(generator)]];
            CandidateSolution &solution2 =
                population[bestIndices[distNumReproduce(generator)]];

            CandidateSolution childSolution;

            childSolution._positions.resize(numActiveNodes);

            // Get crossover point (1 point crossover)
            size_t crossoverPoint = distActiveNodes(generator);

            size_t crossIndex = 0;

            for (; crossIndex < crossoverPoint; crossIndex++)
                childSolution._positions[crossIndex] =
                    solution1._positions[crossIndex];

            for (; crossIndex < numActiveNodes; crossIndex++)
                childSolution._positions[crossIndex] =
                    solution1._positions[crossIndex];

            // Mutate
            for (size_t j = 0; j < numActiveNodes; j++)
                if (dist01(generator) < mutationRate) {
                    childSolution._positions[j].x +=
                        distPositionPerturbation(generator);
                    childSolution._positions[j].y +=
                        distPositionPerturbation(generator);
                }

            newPopulation[i] = childSolution;
        }

        population = newPopulation;
    }

    // ----------------------------- Final Fitness Evaluation
    // -----------------------------

    // Temp list used later
    struct IndexAndFitness {
        size_t _index;
        float _fitness;
    };

    std::list<IndexAndFitness> fitnesses;

    for (size_t i = 0; i < populationSize; i++) {
        IndexAndFitness fitnessAndIndex;

        fitnessAndIndex._fitness = 0.0f;
        fitnessAndIndex._index = i;

        sf::Vector2f directionSum(0.0f, 0.0f);

        // Evaluate fitness
        for (size_t j = 0; j < numActiveNodes; j++) {
            // Distance from connect nodes
            neat::NetworkGenotype::NodeData nodeData =
                genotype.getNodeData(_activeNodeIndexToNodeIndex[j]);

            size_t numConnectionsForNode = nodeData._connections.size();

            for (std::list<std::shared_ptr<neat::ConnectionGene>>::iterator it =
                     nodeData._connections.begin();
                 it != nodeData._connections.end(); it++) {
                assert((*it)->_inIndex != -1);

                if ((*it)->_inIndex == j)
                    continue;

                const size_t inIndex =
                    _nodeIndexToActiveNodeIndex[(*it)->_inIndex];

                // Add difference from a distance of 1 between connected node as
                // cost
                float distanceCost =
                    pow(std::abs(normalDistance -
                                 distance(population[i]._positions[j],
                                          population[i]._positions[inIndex])),
                        normalDistanceExp);

                directionSum += normalize(population[j]._positions[inIndex] -
                                          population[i]._positions[j]);

                fitnessAndIndex._fitness -=
                    normalDistanceInfluence * distanceCost;
            }

            // Add all position costs
            for (size_t k = 0; k < numActiveNodes; k++)
                fitnessAndIndex._fitness -=
                    unconnectedNodeDistanceInfluence *
                    pow(std::abs(unconnectedNodeDistance -
                                 distance(population[i]._positions[j],
                                          population[i]._positions[k])),
                        unconnectedNodeDistanceExp);
        }

        if (directionSum.x != 0.0f || directionSum.y != 0.0f)
            fitnessAndIndex._fitness -=
                unevenConnectionAnglesPunishment *
                powf(magnitude(directionSum), unevenConnectionAnglesExp);

        fitnesses.push_back(fitnessAndIndex);
    }

    // Find the most fit
    std::list<IndexAndFitness>::iterator it = fitnesses.begin();
    it++;

    std::list<IndexAndFitness>::iterator best = fitnesses.begin();

    assert(fitnesses.size() == populationSize);

    for (; it != fitnesses.end(); it++)
        if (it->_fitness > best->_fitness)
            best = it;

    // Found best in the final generation. Compare to overall best
    if (best->_fitness > _bestFitness)
        _best = population[best->_index];

    sf::Vector2f lower(_best._positions[0]);
    sf::Vector2f upper(_best._positions[0]);

    for (size_t i = 1; i < numActiveNodes; i++) {
        if (_best._positions[i].x < lower.x)
            lower.x = _best._positions[i].x;
        if (_best._positions[i].y < lower.y)
            lower.y = _best._positions[i].y;

        if (_best._positions[i].x > upper.x)
            upper.x = _best._positions[i].x;
        if (_best._positions[i].y > upper.y)
            upper.y = _best._positions[i].y;
    }

    _bounds.left = lower.x;
    _bounds.top = lower.y;
    _bounds.width = upper.x - lower.x;
    _bounds.height = upper.y - lower.y;
}

float distance(const sf::Vector2f &v1, const sf::Vector2f &v2) {
    float dx2 = v1.x - v2.x;
    dx2 *= dx2;
    float dy2 = v1.y - v2.y;
    dy2 *= dy2;
    return sqrtf(dx2 + dy2);
}

float magnitude(const sf::Vector2f &v) {
    return sqrtf(v.x * v.x + v.y * v.y);
}

sf::Vector2f normalize(const sf::Vector2f &v) {
    return v / magnitude(v);
}
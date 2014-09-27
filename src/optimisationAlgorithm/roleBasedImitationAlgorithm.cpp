#include <hop_bits/optimisationAlgorithm/roleBasedImitationAlgorithm.hpp>

namespace hop {
  RoleBasedImitationAlgorithm::optimiseImplementation() {

  }
}

    actions = 0;
    populationSize = additionalParameter.populationSize;
    neighboursSize = additionalParameter.neighboursSize;

    stepSize           = abs(upperBounds - lowerBounds) / 100;
    stepSize           = stepSize .^ 2;

    if startDistribution
        parameters = startDistribution;
    else
        parameters = repmat(lowerBounds, populationSize, 1) + repmat((upperBounds - lowerBounds), populationSize, 1) .* rand(populationSize, parameterSize);
    end

    fitness            = ones(1, populationSize) .* Inf;
    agents             = 1:populationSize;

    fitnessEvaluations = 0;
    breaking = 0;
    for agent = agents
        fitnessFunctionParameter.lastSolution = 0;
        fitness(agent) = fitnessFunction(parameters(agent, :), fitnessFunctionParameter);

        fitnessEvaluations    = fitnessEvaluations + 1;

        if any(strcmp(stopCriteria, 'acceptableFitness')) && fitness(agent) <= acceptableFitness
            breaking = 1;
            break;
        end

        if any(strcmp(stopCriteria, 'maxFunctionEvaluations')) && fitnessEvaluations >= maxFunctionEvaluations
            breaking = 1;
            break;
        end
    end

    while(breaking == 0)
        for agent = agents
            previousParameters = parameters(agent, :);
            previousFitness    = fitness(agent);

            neighbours = randperm(populationSize - 1, neighboursSize);
            neighbours(neighbours >= agent) = neighbours(neighbours >= agent) + 1;

            parametersToMutateSize = randi(parameterSize);
            parametersToMutate = randperm(parameterSize, parametersToMutateSize);

            % meanParameters = mean(parameters(neighbours, parametersToMutate));
            meanParameters = sum(parameters(neighbours, parametersToMutate)) / neighboursSize;

            neighboursFitness     = fitness(neighbours);
            neighboursBetter      = neighbours(neighboursFitness <  previousFitness);
            neighboursBetterSize  = size(neighboursBetter, 2);

            static = false;

            if neighboursBetterSize > 0
                stdDev = std(parameters(neighbours, parametersToMutate));
                % stdDev = numberOfneighbours / (numberOfneighbours - 1) * (1 / numberOfneighbours * sum(parameters(neighbours, parametersToMutate) .^ 2) - meanParameters);

                converged             = stdDev < stepSize(parametersToMutate);
                convergedSize         = nnz(converged);

                if convergedSize > 0

                    convergedParameter = parametersToMutate(converged);

                    % notNeighbours = setdiff(agents, [neighbours(:); agent]);
                    notNeighbours = agents(~ismembc(agents, sort([neighbours(:); agent])));

                    factor = ones(1, convergedSize);
                    factor(randperm(convergedSize, randi(convergedSize))) = -1;

                    p = rand(1, convergedSize);

                    other_k = parameters(sub2ind(size(parameters), notNeighbours(randi(size(notNeighbours, 2), 1, convergedSize)), convergedParameter));
                    other_j = parameters(sub2ind(size(parameters), notNeighbours(randi(size(notNeighbours, 2), 1, convergedSize)), convergedParameter));
                    x = other_k .* (1 - p) + other_j .* p;

                    parametersCandidate = x + factor .* abs(x - factor .* max(factor .* other_k, factor .* other_j)) .* rand(1, convergedSize) * 2;
                    withinBoundary = parametersCandidate >= lowerBounds(convergedParameter) & parametersCandidate <= upperBounds(convergedParameter);
                    parameters(agent, convergedParameter(withinBoundary)) = parametersCandidate(withinBoundary);
                else
                    static = true;
                end

                notConverged          = ~converged;
                notConvergedSize      = parametersToMutateSize - convergedSize;

                if notConverged > 0
                    notConvergedParameter = parametersToMutate(notConverged);

                    %meanFitnessBetter   = mean(fitness(neighboursBetter), 1);
                    meanFitnessBetter   = sum(fitness(neighboursBetter), 1) / neighboursBetterSize;
                    stdDevFitnessBetter = std(fitness(neighboursBetter));
                    % stdDevFitnessBetter = neighboursBetterSize / (neighboursBetterSize - 1) * (1 / neighboursBetterSize * sum(fitness(neighboursBetter) .^ 2) - meanFitnessBetter);

                    if previousFitness > meanFitnessBetter + stdDevFitnessBetter * 1.5

                        % meanParametersBetter   = mean(parameters(neighboursBetter, notConvergedParameter), 1);
                        meanParametersBetter = sum(parameters(neighboursBetter, notConvergedParameter), 1) / neighboursBetterSize;

                        factor = ones(1, notConvergedSize);
                        factor(parameters(agent, notConvergedParameter) <  meanParametersBetter) = -1;

                        parametersCandidate = parameters(agent, notConvergedParameter) - factor .* abs(parameters(agent, notConvergedParameter) - meanParametersBetter) .* rand(1, notConvergedSize) * 2;
                        withinBoundary = parametersCandidate >= lowerBounds(notConvergedParameter) & parametersCandidate <= upperBounds(notConvergedParameter);
                        parameters(agent, notConvergedParameter(withinBoundary)) = parametersCandidate(withinBoundary);
                    else
                        % neighboursWorse     = setdiff(neighbours, neighboursBetter);
                        neighboursWorse = neighbours(~ismembc(neighbours, sort(neighboursBetter)));
                        neighboursWorseSize = size(neighboursWorse, 2);

                        % Falls ein Agent schlechter ist als alle anderen,
                        % jedoch die Fitniss nicht deutlich stärker als die
                        % Standardabweichung abweicht, so wird wird der Wert
                        % von meanParametersWorse auf 0 gesetzt, entsprechend
                        % der Initialisierung des original Java-Quelltextes.
                        if neighboursWorseSize > 0
                            % meanParametersWorse = mean(parameters(neighboursWorse, notConvergedParameter), 1);
                            meanParametersWorse = sum(parameters(neighboursWorse, notConvergedParameter), 1) / neighboursWorseSize;
                        else
                            meanParametersWorse = 0;
                        end

                        factor = ones(1, notConvergedSize);
                        factor(parameters(agent, notConvergedParameter) >  meanParametersWorse) = -1;

                        parametersCandidate = parameters(agent, notConvergedParameter) - factor .* abs(parameters(agent, notConvergedParameter) - meanParametersWorse) .* rand(1, notConvergedSize) * 2;
                        withinBoundary = parametersCandidate >= lowerBounds(notConvergedParameter) & parametersCandidate <= upperBounds(notConvergedParameter);
                        parameters(agent, notConvergedParameter(withinBoundary)) = parametersCandidate(withinBoundary);
                    end
                end
            else

                factor = ones(1, parametersToMutateSize);
                factor(parameters(agent, parametersToMutate) >=  meanParameters) = -1;

                parametersCandidate = parameters(agent, parametersToMutate) + factor .* abs(parameters(agent, parametersToMutate) - meanParameters) .* rand(1, parametersToMutateSize) * 2;
                withinBoundary = parametersCandidate >= lowerBounds(parametersToMutate) & parametersCandidate <= upperBounds(parametersToMutate);
                parameters(agent, parametersToMutate(withinBoundary)) = parametersCandidate(withinBoundary);
            end

            fitnessFunctionParameter.lastSolution = fitness(agent);
            if ~static
                fitness(agent) = fitnessFunction(parameters(agent, :), fitnessFunctionParameter);
            end
            fitnessEvaluations = fitnessEvaluations + 1;


            if fitness(agent) > previousFitness
                parameters(agent, :) = previousParameters;
                fitness(agent) = previousFitness;
            end

            if any(strcmp(stopCriteria, 'acceptableFitness')) && fitness(agent) <= acceptableFitness
                breaking = 1;
                break;
            end

            if any(strcmp(stopCriteria, 'maxFunctionEvaluations')) && fitnessEvaluations >= maxFunctionEvaluations
                breaking = 1;
                break;
            end
        end
        if breaking
            break;
        end
    end

    minFitness     = min(fitness);
    bestParameters = parameters(fitness == minFitness, :);
end

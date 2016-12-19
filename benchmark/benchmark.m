dimensions = [2 3 4 5 6 7 8 9 10 15 20 25 30 35 40];
problems = {'Ackley function', 'Rastrigin function', 'Rosenbrock function', 'Sphere function', 'Sum of different powers function'};
repetitions = 100;

mantella = [];
if exist('./Mantella/benchmark.mat', 'file')
  mantella = dlmread('./Mantella/benchmark.mat');
end

for problem_id = 1:length(problems)
  problem = problems(problem_id);

  colors = [
    0, 114, 189
    217, 83, 25
    237, 177, 32
    126, 47, 142
    119, 172, 48
    77, 190, 238
    162, 20, 47
  ] ./ 255; 
  figure;
  set(gcf, 'defaultAxesFontName', 'Helvetica');
  set(gcf, 'defaultAxesFontSize', 10.0);
  set(gcf, 'defaultTextFontName', 'Helvetica');
  set(gcf, 'defaultAxesFontSize', 10.0);
  title(problem{1});
  xlabel('Number of dimensions');
  ylabel('Duration [us]');
  hold on;

  if ~isempty(mantella)
    optimisers = {'Hill climbing', 'Hooke-Jeeves algorithm', 'Nelder-Mead method', 'Particle swarm optimisation', 'Random search'};
    for optimiser_id = 1:length(optimisers)
      optimiser = optimisers(optimiser_id);

      timings = zeros(repetitions, length(dimensions));
      for dimension_id = 1:length(dimensions)
        timings(:, dimension_id) = mantella(mantella(:, 1) == dimensions(dimension_id) & mantella(:, 2) == problem_id - 1 & mantella(:, 3) == optimiser_id - 1, 4);
      end
      plot(dimensions, median(timings, 1) ./ 1000, 'LineWidth', 2, 'DisplayName', optimiser{1}, 'Color', colors(mod(optimiser_id, length(colors)) + 1, :));
    end
  end

  axis([min(dimensions), max(dimensions), 0, 1e4]);
  set(gca, 'yscale','log');
  legend('show', 'Location', 'southeast');
  saveas(gcf, ['benchmark_', problem{1}, '.png']);
end

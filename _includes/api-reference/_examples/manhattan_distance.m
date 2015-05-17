% Loading samples
samples = dlmread('samples.mat');

% Generates a scatter plot
scatter(samples(1, :), samples(2, :));
title('Sampling of the manhattan distance - scatter plot');
xlabel('x_1');
ylabel('x_2');
print('manhattan_distance','-dpng');
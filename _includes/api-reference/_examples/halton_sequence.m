% Loading samples
halton_sequence = dlmread('halton_sequence.mat');
pseudo_random_sequence = dlmread('pseudo_random_sequence.mat');

% Generates a scatter plot of a halton sequence
scatter(halton_sequence(1, :), halton_sequence(2, :));
xlabel('x_1');
ylabel('x_2');
title('Halton sequence - scatter plot');
print('halton_sequence', '-dpng');

% Generates a scatter plot of a pseudo random sequence
scatter(pseudo_random_sequence(1, :), pseudo_random_sequence(2, :));
xlabel('x_1');
ylabel('x_2');
title('Pseudo random sequence - scatter plot');
print('pseudo_random_sequence', '-dpng');
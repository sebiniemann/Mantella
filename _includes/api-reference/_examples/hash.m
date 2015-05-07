% Loading samples
X = dlmread('X.mat');
Y = dlmread('Y.mat');
Z = dlmread('Z.mat');

% Generates a filled contour plot
contourf(X, Y, Z);
shading flat;
title('Hash function - collision probability');
xlabel('x_1');
ylabel('x_2');
colorbar;
print('hash','-dpng');
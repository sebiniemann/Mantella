% Load samples
X = dlmread('X.mat');
Y = dlmread('Y.mat');
Z = dlmread('Z.mat');

% Generate surface plot
surf(X, Y, Z);
shading interp;
title('BBOB Rosenbrock function, rotated - surface plot');
xlabel('x_1');
ylabel('x_2');
zlabel('f(x_1, x_2)');
view(-35, 35);
colorbar;
print('bbob_rosenbrock_function_rotated_surface','-dpng');

% Generate contour plot
contour(X, Y, Z);
title('BBOB Rosenbrock function, rotated - contour plot');
xlabel('x_1');
ylabel('x_2');
colorbar;
print('bbob_rosenbrock_function_rotated_contour','-dpng');
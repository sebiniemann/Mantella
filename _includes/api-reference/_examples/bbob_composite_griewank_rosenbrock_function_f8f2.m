% Loading samples
X = dlmread('X.mat');
Y = dlmread('Y.mat');
Z = dlmread('Z.mat');

% Generates a surface plot
surf(X, Y, Z);
shading interp;
title('BBOB Composite Griewank-Rosenbrock function f8f2 - surface plot');
xlabel('x_1');
ylabel('x_2');
zlabel('f(x_1, x_2)');
view(-35, 35);
colorbar;
print('bbob_composite_griewank_rosenbrock_function_f8f2_surface','-dpng');

% Generates a contour plot
contour(X, Y, Z);
title('BBOB Composite Griewank-Rosenbrock function f8f2 - contour plot');
xlabel('x_1');
ylabel('x_2');
colorbar;
print('bbob_composite_griewank_rosenbrock_function_f8f2_contour','-dpng');
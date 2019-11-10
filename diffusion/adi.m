%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%        Alternating direction implicit (ADI) scheme              %%%
%%%        2D diffusion equation: u_{t} = D(u_{xx} + u_{yy})        %%%
%%%        Uses homogeneous Dirichlet boundary conditions           %%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear
close all

% GIF settings
h = figure;
axis tight manual % this ensures that getframe() returns a consistent size
filename = 'diffusion.gif';

D = 1.9;     % diffusion constant
decay = 0.1; % percent to decay at each timestep

N = 50;  % size of grid (N x N)
dx = 1; dy = 1; dt = 0.1;
tstart = 0; tend = 1 * dt; tstep = (tend - tstart)/dt;

% initial condition
u = zeros(N, N);
u(25, 25) = 20;

% scaling constants
ax = D*dt/dx^2;
ay = D*dt/dy^2;

% matrices for LHS
Ax = diag((1 + ax) * ones(N - 2, 1)) - diag(ax/2 * ones(N - 3, 1), 1) ...
        - diag(ax/2 * ones(N - 3, 1), -1);

Ay = diag((1 + ay) * ones(N - 2, 1)) - diag(ay/2 * ones(N - 3, 1), 1) ...
        - diag(ay/2 * ones(N - 3, 1), -1);

% time loop
for t = 1 : tstep
    % grid at timestep t + dt/2
    % BCs are implicitly set
    u05 = zeros(N, N);
    
    if (mod(t, 10) == 0)
       u(randi(N), randi(N)) = 20;
    end
    
    % x sweep
    for j = 2 : N - 1
        b = zeros(N - 2, 1);  % righthand matrix
        for i = 2 : N - 1
            b(i - 1) = ay/2 * (u(i, j - 1) + u(i, j + 1)) + ...
                       (1 - ay - dt * decay/2) * u(i, j);
        end
        
        % solve the system and assign to intermediate grid
        soln = Ax\b;
        for i = 2 : N - 1
           u05(i, j) = soln(i - 1);
        end
    end
    
    % y sweep
    for i = 2 : N - 1
        b = zeros(N - 2, 1);  % righthand matrix
        for j = 2 : N - 1
            b(j - 1) = ax/2 * (u05(i - 1, j) + u05(i + 1, j)) + ...
                       (1 - ax - dt * decay/2) * u05(i, j);
        end
        
        % solve the system and assign to original grid
        soln = Ay\b;
        for j = 2 : N - 1
           u(i, j) = soln(j - 1); 
        end
    end
    
    % boundary conditions
    for i = 1 : N
        for j = 1 : N
            if (i == 1 || i == N || j == 1 || j == N)
                u(i, j) = 0;
            end
        end
    end
    
    figure(1)
    imagesc(u)
    caxis([0 1])
    drawnow
    
    if (mod(t, 2) == 0)
        % Capture the plot as an image 
        frame = getframe(h); 
        im = frame2im(frame); 
        [imind,cm] = rgb2ind(im,256); 
        % Write to the GIF File 
        if t == 1
            imwrite(imind,cm,filename,'gif', 'Loopcount',inf); 
        else 
            imwrite(imind,cm,filename,'gif','WriteMode','append'); 
        end 
    end
end
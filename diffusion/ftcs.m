%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% Solving 2D diffusion equation using FTCS scheme  %%%
%%% with homogeneous Dirichlet boundary conditions   %%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear
%close all

N = 50;  dx = 1;  dy = 1;  % Ndx x Ndy grid
tstart = 0;  dt = 0.1;  tend = 2 * dt; % time interval
tsteps = floor((tend-tstart)/dt);
D = 1.9;  % diffusion constant
decay = 0.0; % percent to decay at each timestep
ax = D * dt / dx^2;
ay = D * dt / dy^2;

% initial conditions
u = zeros(N, N);
u(25, 25) = 50;

for t = 1:tsteps
    ut = zeros(N, N);
    for i = 1:N
       for j = 1:N
           if (i == 1 || i == N || j == 1 || j == N)  % BCs
               ut(i,j) = 0;
           else  % interior point
               ut(i,j) = u(i,j) + ax*(u(i,j-1)-2*u(i,j)+u(i,j+1)) + ...
                                  ay*(u(i-1,j)-2*u(i,j)+u(i+1,j));
           end
       end
    end
    
    % Reassign new concentrations to original
    for i = 1:N
        for j = 1:N
            u(i,j) = (1 - dt * decay) * ut(i,j);
        end
    end
end

figure(2)
imagesc(u)
colorbar
drawnow
% This file solves the 1-D diffusion equation with growth/decay
% with Dirichlet BCs using Crank Nicholson
clear
L = 100; % length of the spatial domain
dx = 1;  % set space step for space grid
N = L/dx + 1;  % set number of space grid points to include 0 and L
dt = 0.5;   % set time step. Generally setting dt=dx/2 is a reasonable choice
D = 1; % diffusion coefficient
alpha = D*dt/dx^2;

g = 0.1;  % growth/decay coefficient

% initial condition
X = dx*(0:N-1)';
V0 = exp(-(X-dx*N/2).^2./(20));  % initial gaussian
V0(1)=0;
V0(end)=0;
V = V0;
V2 = V0;

% set time interval for simulation
t=0;
tend=1
kstep = round(tend/dt);

% uses Crank Nicolson to solve the diffusion equation
% set up Crank-Nicolson matrix Am
% Dirichlet BCs
Atm = [1; (1 + alpha)*ones(N-2,1); 1];
Atm_u = alpha/2*ones(N-1,1);
Atm_u(1) = 0;
Atm_d = alpha/2*ones(N-1,1);
Atm_d(N-1) = 0;
Am = diag(Atm) - diag(Atm_u,1) - diag(Atm_d,-1);

% The algorithm
for k = 1:kstep
    % construct RHS vector
    bm = V + alpha/2*(-2*[0;V(2:N-1);0] + [0;V(3:N);0] + [0;V(1:N-2);0]);
    bm2 = V2 + alpha/2*(-2*[0;V2(2:N-1);0] + [0;V2(3:N);0] + [0;V2(1:N-2);0]) - dt*g*V2;
    V = Am\bm;  %solve linear system
    V2 = Am\bm2;  %solve linear system
    t = t + dt;
    if mod(k,1)==0  %plot every 10th time step
        figure(10)
        plot(X,V,X,V2)
        axis([0 L  min(V0) max(V0)])
    end
    pause(0.05)
end
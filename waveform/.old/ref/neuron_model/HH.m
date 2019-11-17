function Y = HH(t, X)
global appcurr
% Note these equations use the HH scaling so that rest is 0 rather than -
%70mV.
%Thus, the voltage has been increased by 70mV. You can revert back to the
%original equations by using the commented line in the Bn function.
gna = 120; ena = 115; gk  = 36; ek = -12; gl = 0.3; el = 10.613;
V = X(1);
M = X(2);
N = X(3);
H = X(4);
% The follow code corrects for removable singularities at 10, 25 and 30.
if V == 10
V = 10.000001;
end
if V == 25
V = 25.000001;
end
if V == 30
V = 30.000001;
end
Y(1) = appcurr + gna*M*M*M*H*(ena - V) + gk*(N^4)*(ek - V) + gl*(el-V);
Y(2) = Am(V)*(1-M) - Bm(V)*M;
Y(3) = An(V)*(1-N) - Bn(V)*N;
Y(4) = Ah(V)*(1-H) - Bh(V)*H;
Y = Y';
function y = Ah(V)
y = 0.07.*exp(-V./20.0);
end
function y =  Am(V)
y = (25.0 - V)./(10.0.*(exp((25.0 - V)./10.0) - 1.0));
end
function y =  An(V)
y =  (10.0 - V)./(100.0.*(exp((10.0 - V)./10.0) - 1.0));
end
function y =  Bh(V)
y =  1.0./(exp((30.0-V)./10.0)+1.0);
end
function y = Bm(V)
y =  4.0.*exp(-V./18.0);
end
function y = Bn(V)
%y = 0.125.*exp(-V./80.0);
y = 0.125.*exp(-V./19.7);%Corrected HH model
end
end
%[T, X] = ode45(@HH, 0:0.01:100, [4.21 0.0858 0.3838 0.4455]);  This goes
%to a steady state with the original model
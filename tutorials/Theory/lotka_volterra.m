%% Lotka-Volterra Simulation (agent-based)
%

clear all; clc;

a = 0; % enable animation
N = 200; % size of space
S = zeros(N**2); % initial empty map
T = 1000; % number of iterations to run

p = 0.2; % probability of prey being born
q = 1.0;% probability of prey eaten by predator
r = 0.2; % probability of predator dying

% data collection buffers
X = zeros(1,T);
Y = zeros(1,T);

% directions
dir = [-1,0 ; 0,-1 ; 0,1 ; 1,0];

% initial population
m = randperm(N^2);
nX = 10000;
nY = 5000;
S(m(1:nX)) = 1;
S(m(nX+(1:nY))) = 2;

if a == 1
    figure(1); clf; set(gcf,'windowstyle','docked');
end
for t = 1:T
    
    % gather population counts
    X(t) = length(find(S==1));
    Y(t) = length(find(S==2));
    
    % generate a random interaction map
    m = randperm(N^2)';
    
    % rule 1 - prey born with probability p if empty cell matches prey
    z = find(S(:)==0 & S(m)==1); 
    n = find(rand(1,length(z))<p);
    S(z(n)) = 1;
    
    % rule 2 - prey become predator with probability q if cell matches
    % predator
    x = find(S(:)==1 & S(m)==2); 
    n = find(rand(1,length(x))<q);
    S(x(n)) = 2;
    
    % rule 3 - predator dies with probability r
    y = find(S(:)==2); % predator cell
    n = find(rand(1,length(y))<r);
    S(y(n)) = 0;
end

figure(3); clf; set(gcf,'windowstyle','docked');

plot(X(1:t)/1000,Y(1:t)/1000,'g',mean(X)/1000,mean(Y)/1000,'bo');
hold on;
lims = axis;
plot(mean(X)/1000*ones(2,1),lims([3 4])/1000,'--b');
plot(lims([1 2])/1000,mean(Y)/1000*ones(2,1),'--b');
xlabel('Prey (x1000)');
ylabel('Predator (x1000)');
title('Agent-based Lotka-Volterra')
axis(lims);
grid('on')
axis square;

saveas(gcf,'lotka_volterra.png');

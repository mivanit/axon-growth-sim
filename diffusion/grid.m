close all
concentrations = load('/Users/connorpuritz/Desktop/output.csv');
figure(3)
imagesc(concentrations)
colorbar
drawnow
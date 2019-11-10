%gridfiles = dir('outputs/*.csv');
colormap(hot);

for k=1:length(gridfiles)
    file = strcat(gridfiles(k).folder, '\', gridfiles(k).name);
    grid = csvread(file);
    [num_rows, num_cols] = size(grid);
    subplot(3, 4, k)
    p = pcolor(grid);
    set(p, 'EdgeColor', 'none');
    title(sprintf('Time=%d',(k-1)*50));
end
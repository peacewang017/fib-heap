figure(1);
x = 1:1:10;
a = [2.624, 5.228, 7.660, 10.202, 12.778, 15.200, 17.672, 20.288, 22.800, 25.366];
b = [2.350, 4.760, 7.210, 9.623, 12.107, 14.572, 17.000, 19.478, 21.943, 24.436];
c = [2.050, 4.125, 6.270, 8.509, 10.764, 13.134, 15.635, 18.141, 20.666, 23.645];
plot(x, a, '-*b', x, b, '-or', x, c, '-pg');
axis([0, 11, 0, 30])
set(gca, 'XTick', [0:1:11]);
set(gca, 'YTick', [0:2:32]);
legend({'Binary Tree', 'ZIGZAG', 'Fibonacci Heap'}, 'Location', 'northwest');
xlabel('Node Insert(thousand)');
ylabel('Operation Count(thousand)');
title('Insert');
saveas(gcf, 'insert.png');

figure(2);
x = 1:1:10;
a = [3.104	6.208	9.305	12.401	15.494	18.588	21.671	24.754	27.832	30.901];
b = [2.340	5.100	7.700	10.290	12.600	15.180	17.710	20.180	22.560	25.030];
c = [2.252	4.520	6.964	9.375	11.716	14.010	16.376	18.650	20.926	23.170];
plot(x, a, '-*b', x, b, '-or', x, c, '-pg');
axis([0, 11, 0, 32])
set(gca, 'XTick', [0:1:11]);
set(gca, 'YTick', [0:2:32]);
legend({'Binary Tree', 'ZIGZAG', 'Fibonacci Heap'}, 'Location', 'northwest');
xlabel('Node Delete(hundred)');
ylabel('Operation Count(thousand)');
title('Delete');
saveas(gcf, 'delete.png');

figure(3);
x = 1:1:10;
a = [5.161	6.012	6.697	7.095	7.429	7.731	8.153	8.321	8.609	8.744];
b = [2.736	2.936	3.040	3.111	3.189	3.204	3.217	3.232	3.315	3.377];
c = [3.427	3.791	4.032	4.214	4.330	4.481	4.577	4.663	4.793	4.888];
plot(x, a, '-*b', x, b, '-or', x, c, '-pg');
axis([0, 11, 0, 10])
set(gca, 'XTick', [0:1:11]);
set(gca, 'YTick', [0:1:10]);
legend({'Binary Tree', 'ZIGZAG', 'Fibonacci Heap'}, 'Location', 'northwest');
xlabel('Node Number(thousand)');
ylabel('Delay(thousand)');
title('Delay');
saveas(gcf, 'delay.png');

figure(4);
x = 1:1:10;
a = [48	51	51	50	50	51	51	51	51	51];
b = [21	27	28	27	26	25	25	25	25	25];
c = [97	99	98	98	96	98	96	98	98	98];
plot(x, a, '-*b', x, b, '-or', x, c, '-pg');
axis([0, 11, 0, 100])
set(gca, 'XTick', [0:1:11]);
set(gca, 'YTick', [0:10:100]);
legend({'Binary Tree', 'ZIGZAG', 'Fibonacci Heap'}, 'Location', 'northwest');
xlabel('Node Number(thousand)');
ylabel('Bandwidth Utilization(percentage)');
title('Bandwidth Utilization');
saveas(gcf, 'bandwidth-util.png');
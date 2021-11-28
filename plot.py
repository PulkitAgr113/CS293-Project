import matplotlib.pyplot as plt

# File containing runtimes of different algorithms
file = open("runtimes.txt", 'r')
unoptimized = []
optimized = []
periodic = []
continuous = []
histogram = []
# Experiment has been done N (=10) times
N = 10
iteration = list(range(1, N + 1))

# Making list of runtimes for the 5 algorithms
for line in file:
	if line == "":
		continue
	line = line.split(" ")
	unoptimized.append(float(line[0]))
	optimized.append(float(line[1]))
	periodic.append(float(line[2]))
	continuous.append(float(line[3]))
	histogram.append(float(line[4]))

# Plotting line charts to compare the 5 alghorithms
plt.plot(iteration, unoptimized, label = "Unoptimized Escape Time")
plt.plot(iteration, optimized, label = "Optimized Escape Time")
plt.plot(iteration, periodic, label = "Periodically Checked Optimized Escape Time")
plt.plot(iteration, continuous, label = "Continuous Coloring")
plt.plot(iteration, histogram, label = "Histogram Coloring")
plt.legend()
plt.xlabel("Iteration")
plt.ylabel("Time (in seconds)")
plt.title("Runtimes for various Mandelbrot Algorithms")
plt.savefig("runtime.png")
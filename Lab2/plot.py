import matplotlib.pyplot as plt

# Read probabilities from a text file
filename = 'floorplan/Prob.txt'  # replace with your file name
with open(filename, 'r') as file:
    probabilities = [float(line.strip()) for line in file]

# Define x (iteration) and y (probabilities)
iterations = range(1, len(probabilities) + 1)  # Iterations are the x-axis values (1, 2, 3, ...)
y = probabilities  # Probabilities are the y-axis values

# Plotting only points with smaller markers
plt.plot(iterations, y, marker='o', linestyle='', color='b', markersize=2, label='Probability')

# Labels and title
plt.xlabel('Iteration')
plt.ylabel('Probability')
plt.title('Probability per Iteration')
plt.ylim(0, 1)  # Since the probabilities are between 0 and 1

# Show grid and legend
plt.grid(True)
plt.legend()

# Save the plot as a PNG file
plt.savefig('probability.png', format='png')  # Save to 'probability_points_small.png'

# Optionally, you can show the plot
# plt.show()
